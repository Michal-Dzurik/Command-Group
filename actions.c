#include "includes/actions.h"
#include "includes/config.h"
#include "includes/database.h"
#include "includes/errors.h"
#include "includes/helpers.h"
#include "includes/io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int help() {
    if (file_exists(README)) {
        system("man .cg/cg.1");
        return SUCCESS;
    }

    system("man cg");

    return SUCCESS;
}

int add_group(char* group_name) {
    if (group_name == NULL) return FAIL;

    db_init(DATABASE_NAME);
    int result = db_add_group(group_name);
    db_close();

    return result ? SUCCESS : FAIL;
}

int add_command(char* group_name, char* command_name, char* command) {
    if (group_name == NULL || command_name == NULL || command == NULL) return FAIL;

    db_init(DATABASE_NAME);
    int result = db_add_command(group_name, command_name, command);
    db_close();

    return result ? SUCCESS : FAIL;
}

int remove_group(char* group_name) {
    if (group_name == NULL) return FAIL;

    db_init(DATABASE_NAME);
    int result = db_remove_group(group_name);
    db_close();

    return result ? SUCCESS : FAIL;
}

int remove_command(char* group_name, char* command_name) {
    if (group_name == NULL || command_name == NULL) return FAIL;

    db_init(DATABASE_NAME);
    int result = db_remove_command(group_name, command_name);
    db_close();

    return result ? SUCCESS : FAIL;
}

int execute(char* group_name, char* command_name) {
    if (group_name == NULL || command_name == NULL) return FAIL;
    db_init(DATABASE_NAME);
    char* command = db_get_command(group_name, command_name);
    db_close();

    if (command == NULL) return FAIL;

    // Redirect system's stderr to /dev/null
    // This is becuse the failed command has it's own error message
    char cmd_with_redirect[256];
    snprintf(cmd_with_redirect, sizeof(cmd_with_redirect), "%s 2>/dev/null", command);
    int ret = system(cmd_with_redirect);
    if (ret != SUCCESS) {
        error(ERROR_COMMAND_EXECUTION);
        free(command);
        return FAIL;
    }

    free(command);
    return SUCCESS;
}

int list_groups() {
    db_init(DATABASE_NAME);
    int count = 0;
    char** list = db_get_group_list(&count);
    db_close();

    if (list == NULL || count == 0) {
        error(ERROR_NO_GROUPS);
        return FAIL;
    }

    for (int i = 0; i < count; i++) {
        printf(" - %s\n", list[i]);
    }

    free_group_list(list, count);

    return SUCCESS;
}

int list_commands_by_group(char* group_name) {
    if (group_name == NULL) return FAIL;

    db_init(DATABASE_NAME);
    int count = 0;
    char** list = db_get_command_list(group_name, &count);
    db_close();

    if (list == NULL) {
        if (count != -1) error_args(ERROR_NO_COMMANDS_IN_GROUP, group_name);
        return FAIL;
    }

    for (int i = 0; i < count; i++) {
        printf(" - %s\n", list[i]);
    }

    free_group_list(list, count);

    return SUCCESS;
}

int rename_group(char* group_name, char* new_group_name) {
    if (group_name == NULL || new_group_name == NULL) return FAIL;

    db_init(DATABASE_NAME);
    int result = db_rename_group(group_name, new_group_name);
    db_close();

    return result ? SUCCESS : FAIL;
}

int rename_command(char* group_name, char* command_name, char* new_command_name) {
    if (group_name == NULL || command_name == NULL || new_command_name == NULL) return FAIL;

    db_init(DATABASE_NAME);
    int result = db_rename_command(group_name, command_name, new_command_name);
    db_close();

    return result ? SUCCESS : FAIL;
}