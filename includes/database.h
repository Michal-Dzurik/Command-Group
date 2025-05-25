#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

void db_set_test_name(char *name);

int db_init(const char *filename);
void db_close(void);
int db_add_group(const char *group);
int db_add_command(const char *group, const char *command_name, const char *command);
char* db_get_command(const char *group, const char *command_name);
int db_remove_command(const char *group, const char *command_name);
int db_remove_group(const char *group);

#endif
