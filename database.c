#include "includes/database.h"
#include "includes/io.h"
#include "includes/errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static sqlite3 *db = NULL;
static char *testName = NULL;

// For testin only
void db_set_test_name(char *name){
    testName = name;
}

int db_init(const char *filename) {
    if (sqlite3_open(testName == NULL ? filename : testName, &db) != SQLITE_OK) {
        error_args(ERROR_SQL_OPEN, sqlite3_errmsg(db));
        return 0;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS groups ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  group_name TEXT NOT NULL UNIQUE"
        ");"
        "CREATE TABLE IF NOT EXISTS commands ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  group_id INTEGER NOT NULL, "
        "  command_name TEXT NOT NULL, "
        "  command TEXT NOT NULL, "
        "  FOREIGN KEY(group_id) REFERENCES groups(id) ON DELETE CASCADE, "
        "  UNIQUE(group_id, command_name)"
        ");";

    char *err = 0;
    if (sqlite3_exec(db, sql, 0, 0, &err) != SQLITE_OK) {
        error_args(ERROR_SQL_INIT, err);
        sqlite3_free(err);
        return 0;
    }

    return 1;
}

void db_close() {
    if (db) sqlite3_close(db);
}

static int get_or_create_group_id(const char *group_name) {
    if(group_name == NULL) return 0;
    sqlite3_stmt *stmt;
    int group_id = 0;

    const char *sql_select = "SELECT id FROM groups WHERE group_name = ?;";
    if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        group_id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return group_id;
    }
    sqlite3_finalize(stmt);

    const char *sql_insert = "INSERT INTO groups (group_name) VALUES (?);";
    if (sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);

    group_id = (int)sqlite3_last_insert_rowid(db);
    return group_id;
}

int db_add_group(const char *group) {
    if (group == NULL) return -1;
    
    int id = get_or_create_group_id(group);
    return id != 0;
}

int db_add_command(const char *group, const char *command_name, const char *command) {
    if(group == NULL || command_name == NULL || command_name == NULL) return -1;
    int group_id = get_or_create_group_id(group);
    if (group_id == 0) return -1;

    const char *sql = "INSERT INTO commands (group_id, command_name, command) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_text(stmt, 2, command_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, command, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}


char* db_get_command(const char *group, const char *command_name) {
    if(group == NULL || command_name == NULL) return NULL;
    int group_id = get_or_create_group_id(group);
    if (group_id == 0) return NULL;

    const char *sql = "SELECT command FROM commands WHERE group_id = ? AND command_name = ? LIMIT 1;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return NULL;

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_text(stmt, 2, command_name, -1, SQLITE_STATIC);

    char *result = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *cmd = sqlite3_column_text(stmt, 0);
        if (cmd) {
            result = strdup((const char *)cmd); 
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

int db_remove_command(const char *group, const char *command_name) {
    if (group == NULL || command_name == NULL) return -1;
    
    int group_id = get_or_create_group_id(group);
    if (group_id == 0) return 0;

    const char *sql = "DELETE FROM commands WHERE group_id = ? AND command_name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_text(stmt, 2, command_name, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE || rc == SQLITE_OK;
}

int db_remove_group(const char *group) {
    if (group == NULL) return -1;
    
    const char *sql = "DELETE FROM groups WHERE group_name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, group, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE || rc == SQLITE_OK;
}

char **db_get_group_list(int *out_count) {
    sqlite3_stmt *stmt;
    int count = 0;

    const char *sql_count = "SELECT COUNT(1) FROM groups;";
    if (sqlite3_prepare_v2(db, sql_count, -1, &stmt, NULL) != SQLITE_OK)
        return NULL;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        sqlite3_finalize(stmt);
        return NULL;
    }
    sqlite3_finalize(stmt);

    if (count == 0) {
        *out_count = 0;
        return NULL;
    }

    char **group_list = malloc(count * sizeof(char *));
    if (!group_list) return NULL;

    const char *sql_names = "SELECT group_name FROM groups;";
    if (sqlite3_prepare_v2(db, sql_names, -1, &stmt, NULL) != SQLITE_OK) {
        free(group_list);
        return NULL;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && i < count) {
        const unsigned char *group_name = sqlite3_column_text(stmt, 0);
        if (group_name) {
            group_list[i] = strdup((const char *)group_name);
            if (!group_list[i]) {
                for (int j = 0; j < i; ++j) free(group_list[j]);
                free(group_list);
                sqlite3_finalize(stmt);
                return NULL;
            }
            i++;
        }
    }

    sqlite3_finalize(stmt);
    *out_count = i;
    return group_list;
}

char **db_get_command_list(const char *group_name, int *out_count) {
    if (group_name == NULL || out_count == NULL) return NULL;

    sqlite3_stmt *stmt;
    int count = 0;

    const char *sql_count =
        "SELECT COUNT(*) FROM commands WHERE group_id = "
        "(SELECT id FROM groups WHERE group_name = ?);";

    if (sqlite3_prepare_v2(db, sql_count, -1, &stmt, NULL) != SQLITE_OK)
        return NULL;

    sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        sqlite3_finalize(stmt);
        return NULL;
    }
    sqlite3_finalize(stmt);

    if (count == 0) {
        *out_count = 0;
        return NULL;
    }

    char **command_list = malloc(count * sizeof(char *));
    if (!command_list) return NULL;

    const char *sql =
        "SELECT command_name FROM commands WHERE group_id = "
        "(SELECT id FROM groups WHERE group_name = ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        free(command_list);
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);
    int i = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW && i < count) {
        const unsigned char *cmd = sqlite3_column_text(stmt, 0);
        if (cmd) {
            command_list[i] = strdup((const char *)cmd);
            if (!command_list[i]) {
                for (int j = 0; j < i; ++j) free(command_list[j]);
                free(command_list);
                sqlite3_finalize(stmt);
                return NULL;
            }
            i++;
        }
    }

    sqlite3_finalize(stmt);
    *out_count = i;
    return command_list;
}

void free_group_list(char **list, int count) {
    for (int i = 0; i < count; ++i) free(list[i]);
    free(list);
}