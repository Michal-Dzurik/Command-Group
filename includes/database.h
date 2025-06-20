#ifndef DATABASE_H
#define DATABASE_H

void db_set_test_name(char* name);

int db_init(const char* filename);
void db_close(void);

int db_add_command(const char* group, const char* command_name, const char* command);
int db_rename_command(const char* group, const char* command_name, const char* new_command_name);
char* db_get_command(const char* group, const char* command_name);
int db_remove_command(const char* group, const char* command_name);

int db_add_group(const char* group);
int db_rename_group(const char* group, const char* new_group);
int db_remove_group(const char* group);
char** db_get_group_list(int* out_count);
char** db_get_command_list(const char* group_name, int* out_count);

void free_group_list(char** list, int count);

#endif
