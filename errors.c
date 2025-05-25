#include "includes/errors.h"

const char *ERROR_CREATE_GROUP_ARGUMENTS = "Not enough arguments for adding group\n";
const char *ERROR_CREATE_COMMAND_ARGUMENTS = "Not enough arguments for adding command\n";

const char *ERROR_REMOVE_GROUP_ARGUMENTS = "Not enough arguments for removing group\n";
const char *ERROR_REMOVE_COMMAND_ARGUMENTS = "Not enough arguments for removing command\n";

const char *ERROR_EXECUTE_ARGUMENTS = "Can't execute that\n";

const char *ERROR_COMMAND_EXECUTION = "Command has failed\n";

const char *ERROR_SQL_OPEN = "Failed to open SQLite: %s\n";
const char *ERROR_SQL_INIT = "Failed to init SQLite: %s\n";
const char *ERROR_SQL_COMMAND_IS_NULL = "Command is NULL\n";
