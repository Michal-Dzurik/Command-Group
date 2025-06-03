#include "../includes/database.h"
#include "../includes/actions.h"
#include "../includes/config.h"
#include "../includes/errors.h"
#include "./includes/helpers.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#define GROUP_NAME "group_name"
#define GROUP_NAME_1 "example_group1"
#define GROUP_NAME_2 "example_group2"
#define NON_EXISTING_NAME "non-existing"

#define COMMAND_NAME "example_command"
#define COMMAND_NAME_1 "example_command1"
#define COMMAND_NAME_2 "example_command2"

#define LS "ls"
#define ECHO "echo 'test'"

#define EMPTY_STRING ""

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

Test(actions, action_create_group_dry) {
    int result = add_group(NULL);

    cr_assert_eq(result,FAIL);
}

Test(actions, action_create_group) {
    int result = add_group(GROUP_NAME);

    cr_assert_eq(result,SUCCESS);
}

Test(actions, action_remove_group_dry) {
    int result = remove_group(NULL);

    cr_assert_eq(result,FAIL);
}

Test(actions, action_remove_group) {
    db_add_group(GROUP_NAME);

    int result = remove_group(GROUP_NAME);
    cr_assert_eq(result,SUCCESS);

    result = remove_group(NON_EXISTING_NAME);
    cr_assert_eq(result,SUCCESS);
}

Test(actions, actions_list_group_dry) {
    FILE *original_stderr = switch_stderr_for_temp();

    int result = list_groups();

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result, FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
}

Test(actions, actions_list_group) {
    db_add_group(GROUP_NAME);
    db_add_group(GROUP_NAME_1);

    int result = list_groups();
    cr_assert_eq(result, SUCCESS);
}

Test(actions, actions_create_command_dry) {
    FILE *original_stderr = switch_stderr_for_temp();

    int result = add_command(NULL, NULL, NULL);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result, FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
}

Test(actions, actions_create_command) {
    db_add_group(GROUP_NAME);

    int result = add_command(GROUP_NAME,COMMAND_NAME,LS);
    cr_assert_eq(result,SUCCESS);

    result = add_command(NON_EXISTING_NAME,COMMAND_NAME,LS);

    cr_assert_eq(result,SUCCESS);   
}

Test(actions, actions_remove_command_dry) {
    FILE *original_stderr = switch_stderr_for_temp();

    int result = remove_command(NULL, NULL);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result,FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
}

Test(actions, actions_remove_command) {
    db_add_group(GROUP_NAME);
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);

    int result = remove_command(GROUP_NAME,COMMAND_NAME);
    cr_assert_eq(result,SUCCESS);

    result = remove_command(NON_EXISTING_NAME,COMMAND_NAME);
    cr_assert_eq(result,SUCCESS);

    result = remove_command(NON_EXISTING_NAME,NON_EXISTING_NAME);
    cr_assert_eq(result,SUCCESS);
}

Test(actions, actions_remove_group_and_commands) {
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_1,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_2,LS);

    int result = remove_group(GROUP_NAME);
    cr_assert_eq(result,SUCCESS);

    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME),NULL);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME_1),NULL);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME_2),NULL);
}

Test(actions, actions_list_commands_dry) {
    FILE *original_stderr = switch_stderr_for_temp();

    int result = list_commands_by_group(NULL);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result, FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
}

Test(actions, actions_list_commands_command) {
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);

    int result = list_commands_by_group(GROUP_NAME);
    cr_assert_eq(result,SUCCESS);

    FILE *original_stderr = switch_stderr_for_temp();

    result = list_commands_by_group(GROUP_NAME_1);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result,FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
}

Test(actions, actions_execute_dry){
    FILE *original_stderr = switch_stderr_for_temp();
    int result = execute(NULL,NULL);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result,FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);


    original_stderr = switch_stderr_for_temp();
    result = execute(GROUP_NAME,COMMAND_NAME);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result,FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);


    db_add_command(GROUP_NAME,COMMAND_NAME,ECHO);

    original_stderr = switch_stderr_for_temp();
    result = execute(GROUP_NAME,COMMAND_NAME_1);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result,FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
}

Test(actions, actions_help){
    int result = help();

    cr_assert_eq(result, SUCCESS);
}

Test(actions, actions_rename_group_dry){
    int result = rename_group(NULL, NULL);
    cr_assert_eq(result, FAIL);

    FILE *original_stderr = switch_stderr_for_temp();
    result = rename_group(GROUP_NAME, GROUP_NAME_1);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result, FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
}

Test(actions, actions_rename_group){
    db_add_group(GROUP_NAME);
    int result = rename_group(GROUP_NAME, GROUP_NAME_1);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, actions_rename_command_dry){
    int result = rename_command(NULL, NULL, NULL);
    cr_assert_eq(result, FAIL);

    FILE *original_stderr = switch_stderr_for_temp();
    result = rename_command(GROUP_NAME, COMMAND_NAME, COMMAND_NAME_1);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result, FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);

    db_add_group(GROUP_NAME);
    result = rename_command(GROUP_NAME, COMMAND_NAME, COMMAND_NAME_1);

    switch_temp_for_stderr(original_stderr);

    cr_assert_eq(result, FAIL);
    cr_assert(is_file_empty(TEMP_FILE));

    remove(TEMP_FILE);
    
}

Test(actions, actions_rename_command){
    db_add_command(GROUP_NAME,COMMAND_NAME,ECHO);
    int result = rename_command(GROUP_NAME, COMMAND_NAME, COMMAND_NAME_1);

    cr_assert_eq(result, SUCCESS);
}