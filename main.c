#include "includes/config.h"
#include "includes/actions.h"
#include "includes/helpers.h"
#include "includes/errors.h"
#include "includes/io.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1) return help();
    if (strcmp(to_lower(argv[1]), "add") == 0) {
        if (argc < 3) {
            printf("- command\n- group\n");
            return SUCCESS;
        }
        if (strcmp(to_lower(argv[2]), "group") == 0){
            // Add group
            if (argc < ADD_GROUP_MIN_ARGS){
                error(ERROR_CREATE_GROUP_ARGUMENTS);
                return FAIL;
            }
            return add_group(argv[3]);
        }
        if (strcmp(to_lower(argv[2]), "command") == 0){
            // Add command
            if (argc < ADD_COMMAND_MIN_ARGS) {
                error(ERROR_CREATE_COMMAND_ARGUMENTS);
                return FAIL;
            }
            return add_command(argv[3],argv[4],argv[5]);
        }
        
        return FAIL;
    }
    if (strcmp(to_lower(argv[1]), "remove") == 0) {
        if (argc < 3) {
            printf("- command\n- group\n");
            return SUCCESS;
        }
        if (strcmp(to_lower(argv[2]), "group") == 0){
            // Remove group
            if (argc < REMOVE_GROUP_MIN_ARGS) {
                error(ERROR_REMOVE_GROUP_ARGUMENTS);
                return FAIL;
            }
            return remove_group(argv[3]);
        }
        if (strcmp(to_lower(argv[2]), "command") == 0){
            // Remove command
            if (argc < REMOVE_COMMAND_MIN_ARGS) {
                error(ERROR_REMOVE_COMMAND_ARGUMENTS);
                return FAIL;
            }
            return remove_command(argv[3],argv[4]);
        }

        return FAIL;
    }

    // Aliases
    if (strcmp(to_lower(argv[1]), "-ag") == 0){
        // Add group
        if (argc < ADD_GROUP_MIN_ARGS -1) {
            error(ERROR_CREATE_GROUP_ARGUMENTS);
            return FAIL;
        }
        return add_group(argv[2]);
    }
    if (strcmp(to_lower(argv[1]), "-ac") == 0){
        // Add command
        if (argc < ADD_COMMAND_MIN_ARGS -1) {
            error(ERROR_CREATE_COMMAND_ARGUMENTS);
            return FAIL;
        }
        return add_command(argv[2],argv[3],argv[4]);
    }
    if (strcmp(to_lower(argv[1]), "-rg") == 0){
        // Remove group
        if (argc < REMOVE_GROUP_MIN_ARGS -1) {
            error(ERROR_REMOVE_GROUP_ARGUMENTS);
            return FAIL;
        }
        return remove_group(argv[2]);
    }
    if (strcmp(to_lower(argv[1]), "-rc") == 0){
        // Remove command
        if (argc < REMOVE_COMMAND_MIN_ARGS -1) {
            error(ERROR_REMOVE_COMMAND_ARGUMENTS);
            return FAIL;
        }
        return remove_command(argv[2],argv[3]);
    }

    if (argc == EXECUTE_COMMAND_MIN_ARGS){
        return execute(argv[1],argv[2]);
    }
    
    error(ERROR_EXECUTE_ARGUMENTS);
    return FAIL;
}