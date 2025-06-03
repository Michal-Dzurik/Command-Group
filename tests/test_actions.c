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

Test(actions, action_create_group_dry) {
    // NULL group_name
    int result = add_group(NULL);

    cr_assert_eq(result,FAIL);
}

Test(actions, action_create_group) {
    // not NULL group_name
    int result = add_group(GROUP_NAME);

    cr_assert_eq(result,SUCCESS);
}

Test(actions, action_remove_group_dry) {
    // NULL group_name
    int result = remove_group(NULL);

    cr_assert_eq(result,FAIL);
}

Test(actions, action_remove_group) {
    db_add_group(GROUP_NAME);

    // not NULL group_name that exists in db
    int result = remove_group(GROUP_NAME);
    cr_assert_eq(result,SUCCESS);

    // not NULL group_name that doesn't exists in db
    result = remove_group(NON_EXISTING_NAME);
    cr_assert_eq(result,SUCCESS);
}

Test(actions, actions_list_group_dry) {
    int result = list_groups();

    // no groups in db
    cr_assert_eq(result, FAIL);
    assert_stderr_equals(ERROR_NO_GROUPS);
}

Test(actions, actions_list_group) {
    db_add_group(GROUP_NAME);
    db_add_group(GROUP_NAME_1);

    // some groups in db
    int result = list_groups();
    cr_assert_eq(result, SUCCESS);
}

Test(actions, actions_create_command_dry) {
    // NULL arguments 
    int result = add_command(NULL, NULL, NULL);

    cr_assert_eq(result, FAIL);
}

Test(actions, actions_create_command) {
    // group doesn't exists in db
    int result = add_command(NON_EXISTING_NAME,COMMAND_NAME,LS);
    cr_assert_eq(result,SUCCESS);  

    // group exists in db
    db_add_group(GROUP_NAME);
    result = add_command(GROUP_NAME,COMMAND_NAME,LS);
    cr_assert_eq(result,SUCCESS); 
}

Test(actions, actions_remove_command_dry) {
    // NULL arguments
    int result = remove_command(NULL, NULL);
    cr_assert_eq(result,FAIL);

    // group doesn't exists in db
    result = remove_command(NON_EXISTING_NAME,COMMAND_NAME);
    cr_assert_eq(result,FAIL);

    // group exists in db but command doesn't
    db_add_group(GROUP_NAME);
    result = remove_command(GROUP_NAME,NON_EXISTING_NAME);
    cr_assert_eq(result,FAIL);
}

Test(actions, actions_remove_command) {
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);

    // command and group exist in db
    int result = remove_command(GROUP_NAME,COMMAND_NAME);
    cr_assert_eq(result,SUCCESS);
}


Test(actions, actions_list_commands_dry) {
    int result = list_commands_by_group(NULL);
    cr_assert_eq(result, FAIL);

    result = list_commands_by_group(GROUP_NAME);
    cr_assert_eq(result, FAIL);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(actions, actions_remove_group_and_commands) {
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);

    // Group name exists with one command
    int result = remove_group(GROUP_NAME);
    cr_assert_eq(result,SUCCESS);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME),NULL);

    // Group name exists with many commands
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_1,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_2,LS);

    result = remove_group(GROUP_NAME);

    cr_assert_eq(result,SUCCESS);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME),NULL);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME_1),NULL);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME_2),NULL);
}

Test(actions, actions_list_commands) {
    // commands exists in db
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);

    int result = list_commands_by_group(GROUP_NAME);
    cr_assert_eq(result,SUCCESS);
}

Test(actions, actions_execute_dry){
    // NULL arguments
    int result = execute(NULL,NULL);
    cr_assert_eq(result,FAIL);
    
    // group doesn't exists in db
    result = execute(NON_EXISTING_NAME,COMMAND_NAME);
    cr_assert_eq(result,FAIL);
    assert_stderr_equals(ERROR_NON_EXISTING_GROUP);
}

Test(actions, actions_execute_partial_dry){
    // group doesn't exists in db
    db_add_group(GROUP_NAME);
    int result = execute(GROUP_NAME,NON_EXISTING_NAME);
    cr_assert_eq(result,FAIL);
    assert_stderr_equals(ERROR_NON_EXISTING_COMMAND);
}

Test(actions, actions_help){
    int result = help();

    cr_assert_eq(result, SUCCESS);
}

Test(actions, actions_rename_group_dry){
    // NULL arguments
    int result = rename_group(NULL, NULL);
    cr_assert_eq(result, FAIL);

    // group doesn't exists in db
    result = rename_group(GROUP_NAME, GROUP_NAME_1);
    cr_assert_eq(result, FAIL);
}

Test(actions, actions_rename_group){
    // groups exists in db
    db_add_group(GROUP_NAME);
    int result = rename_group(GROUP_NAME, GROUP_NAME_1);
    cr_assert_eq(result, SUCCESS);
}

Test(actions, actions_rename_command_dry){
    // NULL arguments
    int result = rename_command(NULL, NULL, NULL);
    cr_assert_eq(result, FAIL);

    // groups doesn't exists in db
    result = rename_command(NON_EXISTING_NAME, COMMAND_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, FAIL);

    // command doesn't exists in db
    db_add_group(GROUP_NAME);
    result = rename_command(GROUP_NAME, NON_EXISTING_NAME, COMMAND_NAME_1);
    cr_assert_eq(result, FAIL);
}

Test(actions, actions_rename_command){
    // command exists in db
    db_add_command(GROUP_NAME,COMMAND_NAME,ECHO);
    int result = rename_command(GROUP_NAME, COMMAND_NAME, COMMAND_NAME_1);

    cr_assert_eq(result, SUCCESS);
}