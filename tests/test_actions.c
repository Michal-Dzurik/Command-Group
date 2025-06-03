#include "../includes/database.h"
#include "../includes/actions.h"
#include "../includes/config.h"
#include "../includes/errors.h"
#include "../includes/io.h"

#include "./includes/helpers.h"
#include "./includes/test_config.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

void prepare_actions(){
    db_set_test_name(TEST_DATABASE_NAME);
    db_init(EMPTY_STRING);

    cr_redirect_stderr();
    cr_redirect_stdout();
}

void destroy_actions(){
    db_close();
}

TestSuite(actions, .init = prepare_actions, .fini = destroy_actions);

// GROUPS

Test(actions, create_group_null_name) {
    int result = add_group(NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, create_group_valid_name) {
    int result = add_group(GROUP_NAME);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, remove_group_null_name) {
    int result = remove_group(NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, remove_existing_group) {
    db_add_group(GROUP_NAME);
    int result = remove_group(GROUP_NAME);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, remove_non_existing_group) {
    int result = remove_group(NON_EXISTING_NAME);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, list_groups_empty) {
    int result = list_groups();
    cr_assert_eq(result, FAIL);
    assert_stderr_equals(ERROR_NO_GROUPS);
}

Test(actions, list_existing_groups) {
    db_add_group(GROUP_NAME);
    db_add_group(GROUP_NAME_1);
    int result = list_groups();
    cr_assert_eq(result, SUCCESS);
}

Test(actions, rename_group_null_args) {
    int result = rename_group(NULL, NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, rename_non_existing_group) {
    int result = rename_group(GROUP_NAME, GROUP_NAME_1);
    cr_assert_eq(result, FAIL);
}

Test(actions, rename_existing_group) {
    db_add_group(GROUP_NAME);
    int result = rename_group(GROUP_NAME, GROUP_NAME_1);
    cr_assert_eq(result, SUCCESS);
}

// COMMANDS

Test(actions, create_command_null_args) {
    int result = add_command(NULL, NULL, NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, create_command_in_non_existing_group) {
    int result = add_command(NON_EXISTING_NAME, COMMAND_NAME, LS);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, create_command_in_existing_group) {
    db_add_group(GROUP_NAME);
    int result = add_command(GROUP_NAME, COMMAND_NAME, LS);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, remove_command_null_args) {
    int result = remove_command(NULL, NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, remove_command_from_non_existing_group) {
    int result = remove_command(NON_EXISTING_NAME, COMMAND_NAME);
    cr_assert_eq(result, FAIL);
}

Test(actions, remove_non_existing_command_from_group) {
    db_add_group(GROUP_NAME);
    int result = remove_command(GROUP_NAME, NON_EXISTING_NAME);
    cr_assert_eq(result, FAIL);
}

Test(actions, remove_existing_command) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    int result = remove_command(GROUP_NAME, COMMAND_NAME);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, list_commands_null_group) {
    int result = list_commands_by_group(NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, list_commands_non_existing_group) {
    int result = list_commands_by_group(GROUP_NAME);
    cr_assert_eq(result, FAIL);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(actions, list_existing_commands_in_group) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    int result = list_commands_by_group(GROUP_NAME);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, rename_command_null_args) {
    int result = rename_command(NULL, NULL, NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, rename_command_non_existing_group) {
    int result = rename_command(NON_EXISTING_NAME, COMMAND_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, FAIL);
}

Test(actions, rename_non_existing_command) {
    db_add_group(GROUP_NAME);
    int result = rename_command(GROUP_NAME, NON_EXISTING_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, FAIL);
}

Test(actions, rename_existing_command) {
    db_add_command(GROUP_NAME, COMMAND_NAME, ECHO);
    int result = rename_command(GROUP_NAME, COMMAND_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, SUCCESS);
}

// RELATIONS (GROUPS + COMMANDS)

Test(actions, remove_group_removes_its_commands_single) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    int result = remove_group(GROUP_NAME);
    cr_assert_eq(result, SUCCESS);
    cr_assert_eq(db_get_command(GROUP_NAME, COMMAND_NAME), NULL);
}

Test(actions, remove_group_removes_its_commands_multiple) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    db_add_command(GROUP_NAME, COMMAND_NAME_1, LS);
    db_add_command(GROUP_NAME, COMMAND_NAME_2, LS);

    int result = remove_group(GROUP_NAME);
    cr_assert_eq(result, SUCCESS);
    cr_assert_eq(db_get_command(GROUP_NAME, COMMAND_NAME), NULL);
    cr_assert_eq(db_get_command(GROUP_NAME, COMMAND_NAME_1), NULL);
    cr_assert_eq(db_get_command(GROUP_NAME, COMMAND_NAME_2), NULL);
}

// EXECUTION

Test(actions, execute_null_args) {
    int result = execute(NULL, NULL);
    cr_assert_eq(result, FAIL);
}

Test(actions, execute_non_existing_group) {
    int result = execute(NON_EXISTING_NAME, COMMAND_NAME);
    cr_assert_eq(result, FAIL);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(actions, execute_non_existing_command) {
    db_add_group(GROUP_NAME);
    int result = execute(GROUP_NAME, NON_EXISTING_NAME);
    cr_assert_eq(result, FAIL);
    assert_stderr_equals(ERROR_NON_EXISTING_COMMAND);
}

// OTHER

Test(actions, help_command_prints_usage) {
    int result = help();
    cr_assert_eq(result, SUCCESS);
}
