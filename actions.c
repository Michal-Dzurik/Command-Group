#include "includes/actions.h"
#include "includes/config.h"
#include "includes/errors.h"
#include "includes/database.h"
#include "includes/io.h"

#include <stdio.h>
#include <stdlib.h>

int help(){
    system("mdcat README.md");
    return SUCCESS;
}

int add_group(char* group_name){
    db_init(DATABASE_NAME);
    db_add_group(group_name);
    db_close();

    return SUCCESS;
}

int add_command(char* group_name, char* command_name, char* command){
    db_init(DATABASE_NAME);
    db_add_command(group_name, command_name ,command);
    db_close();
    
    return SUCCESS;
}

int remove_group(char* group_name){
    db_init(DATABASE_NAME);
    db_remove_group(group_name);
    db_close();

    return SUCCESS;
}

int remove_command(char* group_name, char* command_name){
    db_init(DATABASE_NAME);
    db_remove_command(group_name, command_name);
    db_close();

    return SUCCESS;
}

int execute(char* group_name,char* command_name){
    db_init(DATABASE_NAME);
    char *command = db_get_command(group_name, command_name);
    db_close();

    if (command == NULL){
        error(ERROR_SQL_COMMAND_IS_NULL);
        return FAIL;
    }

    int ret = system(command);
    if (ret == -1) {
        error(ERROR_COMMAND_EXECUTION);
        free(command);
        return FAIL;
    }

    free(command);
    return SUCCESS;
}