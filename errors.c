#include "includes/errors.h"

const char* ERROR_CREATE_GROUP_ARGUMENTS = "Not enough arguments for adding group\n";
const char* ERROR_CREATE_COMMAND_ARGUMENTS = "Not enough arguments for adding command\n";

const char* ERROR_REMOVE_GROUP_ARGUMENTS = "Not enough arguments for removing group\n";
const char* ERROR_REMOVE_COMMAND_ARGUMENTS = "Not enough arguments for removing command\n";

const char* ERROR_RENAME_GROUP_ARGUMENTS = "Not enough arguments for renaming group\n";
const char* ERROR_RENAME_COMMAND_ARGUMENTS = "Not enough arguments for renaming command\n";

const char* ERROR_EXECUTE_ARGUMENTS = "Can't execute that\n";

const char* ERROR_COMMAND_EXECUTION = "Command has failed\n";

const char* ERROR_SQL_OPEN = "Failed to open SQLite: %s\n";
const char* ERROR_SQL_INIT = "Failed to init SQLite: %s\n";
const char* ERROR_SQL_COMMAND_IS_NULL = "Command is NULL\n";

const char* ERROR_NO_GROUPS = "There are no groups recorded\n";
const char* ERROR_NO_COMMANDS_IN_GROUP = "There are no commands within group \"%s\"\n";
const char* ERROR_NON_EXISTING_GROUP = "Group doesn't exists\n";
const char* ERROR_NON_EXISTING_COMMAND = "Command doesn't exists\n";

const char* ERROR_NO_GROUP_PROVIDED = "No group name was provided\n";
