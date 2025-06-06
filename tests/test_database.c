#include "../includes/config.h"
#include "../includes/database.h"
#include "../includes/errors.h"

#include "./includes/helpers.h"
#include "./includes/test_config.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

void prepare_database() {
    db_set_test_name(TEST_DATABASE_NAME);
    cr_assert(db_init(EMPTY_STRING));

    cr_redirect_stderr();
    cr_redirect_stdout();
}

void destroy_database() {
    db_close();
}

TestSuite(database, .init = prepare_database, .fini = destroy_database);

// GROUPS

Test(database, create_group_with_null_name) {
    int result = db_add_group(NULL);
    cr_assert_eq(result, NULL_CODE);
}

Test(database, create_valid_group) {
    int result = db_add_group(GROUP_NAME);
    cr_assert_eq(result, true);
}

Test(database, remove_group_with_null_name) {
    int result = db_remove_group(NULL);
    cr_assert_eq(result, NULL_CODE);
}

Test(database, remove_existing_group) {
    db_add_group(GROUP_NAME);
    int result = db_remove_group(GROUP_NAME);
    cr_assert_eq(result, true);
}

Test(database, remove_non_existing_group) {
    int result = db_remove_group(NON_EXISTING_NAME);
    cr_assert_eq(result, true);
}

Test(database, list_groups_when_empty) {
    int count = 0;
    char** result = db_get_group_list(&count);
    cr_assert_eq(result, NULL);
}

Test(database, list_existing_groups) {
    db_add_group(GROUP_NAME);
    db_add_group(GROUP_NAME_1);

    char* expected[2] = {GROUP_NAME, GROUP_NAME_1};
    int count = 0;
    char** result = db_get_group_list(&count);
    cr_assert_eq(count, 2);

    for (int i = 0; i < count; i++) {
        cr_assert_str_eq(result[i], expected[i]);
    }

    free_group_list(result, count);
}

Test(database, rename_group_with_nulls) {
    int result = db_rename_group(NULL, NULL);
    cr_assert_eq(result, NULL_CODE);
}

Test(database, rename_non_existing_group) {
    int result = db_rename_group(NON_EXISTING_NAME, GROUP_NAME_1);
    cr_assert_eq(result, false);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(database, rename_existing_group) {
    db_add_group(GROUP_NAME);
    int result = db_rename_group(GROUP_NAME, GROUP_NAME_1);
    cr_assert_eq(result, true);
}

// COMMANDS

Test(database, create_command_with_nulls) {
    int result = db_add_command(NULL, NULL, NULL);
    cr_assert_eq(result, NULL_CODE);
}

Test(database, create_command_in_existing_group) {
    db_add_group(GROUP_NAME);
    int result = db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    cr_assert_eq(result, true);
}

Test(database, create_command_in_non_existing_group) {
    int result = db_add_command(NON_EXISTING_NAME, COMMAND_NAME, LS);
    cr_assert_eq(result, true);
}

Test(database, get_command_with_nulls) {
    char* result = db_get_command(NULL, NULL);
    cr_assert_eq(result, NULL);
}

Test(database, get_existing_command) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    char* result = db_get_command(GROUP_NAME, COMMAND_NAME);
    cr_assert_str_eq(result, LS);
    free(result);
}

Test(database, get_non_existing_command_in_existing_group) {
    char* result = db_get_command(GROUP_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, NULL);
    free(result);
}

Test(database, get_command_in_non_existing_group) {
    char* result = db_get_command(GROUP_NAME_1, COMMAND_NAME_1);
    cr_assert_eq(result, NULL);
    free(result);
}

Test(database, remove_command_with_nulls) {
    int result = db_remove_command(NULL, NULL);
    cr_assert_eq(result, NULL_CODE);
}

Test(database, remove_command_from_non_existing_group) {
    int result = db_remove_command(NON_EXISTING_NAME, COMMAND_NAME);
    cr_assert_eq(result, false);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(database, remove_non_existing_command_from_existing_group) {
    db_add_group(GROUP_NAME);
    int result = db_remove_command(GROUP_NAME, NON_EXISTING_NAME);
    cr_assert_eq(result, false);
    assert_stderr_equals(ERROR_NON_EXISTING_COMMAND);
}

Test(database, remove_existing_command) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    int result = db_remove_command(GROUP_NAME, COMMAND_NAME);
    cr_assert_eq(result, true);
}

Test(database, list_commands_with_null_group) {
    int count = 0;
    char** result = db_get_command_list(NULL, &count);
    cr_assert_eq(result, NULL);
}

Test(database, list_commands_in_empty_group) {
    db_add_group(GROUP_NAME);
    int count = 0;
    char** result = db_get_command_list(GROUP_NAME, &count);
    cr_assert_eq(result, NULL);
}

Test(database, list_commands_in_non_existing_group) {
    int count = 0;
    char** result = db_get_command_list(GROUP_NAME_1, &count);
    cr_assert_eq(result, NULL);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(database, list_existing_commands_in_group) {
    db_add_group(GROUP_NAME);
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    db_add_command(GROUP_NAME, COMMAND_NAME_1, LS);
    db_add_command(GROUP_NAME, COMMAND_NAME_2, LS);

    char* expected[3] = {
        COMMAND_NAME,
        COMMAND_NAME_1,
        COMMAND_NAME_2};

    int count = 0;
    char** result = db_get_command_list(GROUP_NAME, &count);
    cr_assert_eq(count, 3);

    for (int i = 0; i < count; i++) {
        cr_assert_str_eq(result[i], expected[i]);
    }

    free_group_list(result, count);
}

Test(database, rename_command_with_nulls) {
    int result = db_rename_command(NULL, NULL, NULL);
    cr_assert_eq(result, NULL_CODE);
}

Test(database, rename_command_in_non_existing_group) {
    int result = db_rename_command(NON_EXISTING_NAME, COMMAND_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, false);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(database, rename_non_existing_command) {
    db_add_group(GROUP_NAME);
    int result = db_rename_command(GROUP_NAME, NON_EXISTING_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, false);
    assert_stderr_equals(ERROR_NON_EXISTING_COMMAND);
}

Test(database, rename_existing_command) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    int result = db_rename_command(GROUP_NAME, COMMAND_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, true);
}

// RELATIONS (GROUPS + COMMANDS)

Test(database, remove_group_also_removes_all_commands) {
    db_add_command(GROUP_NAME, COMMAND_NAME, LS);
    db_add_command(GROUP_NAME, COMMAND_NAME_1, LS);
    db_add_command(GROUP_NAME, COMMAND_NAME_2, LS);

    int result = db_remove_group(GROUP_NAME);
    cr_assert_eq(result, true);

    cr_assert_eq(db_get_command(GROUP_NAME, COMMAND_NAME), NULL);
    cr_assert_eq(db_get_command(GROUP_NAME, COMMAND_NAME_1), NULL);
    cr_assert_eq(db_get_command(GROUP_NAME, COMMAND_NAME_2), NULL);
}
