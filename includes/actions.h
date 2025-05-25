#ifndef ACTIONS_H
#define ACTIONS_H

int help();

int add_group(char* group_name);
int add_command(char* group_name, char* command_name, char* command);

int remove_group(char* group_name);
int remove_command(char* group_name, char* command_name);

int execute(char* group_name, char* command_name);

#endif