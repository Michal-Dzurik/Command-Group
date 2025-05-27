#include "../includes/database.h"

#include <criterion/criterion.h>

#define DATABASE_NAME "db_test.db"

#define GROUP_NAME "group_name"
#define GROUP_NAME_1 "example_group1"
#define GROUP_NAME_2 "example_group2"
#define NON_EXISTING_NAME "non-existing"

#define COMMAND_NAME "example_command"
#define COMMAND_NAME_1 "example_command1"
#define COMMAND_NAME_2 "example_command2"

#define LS "ls"

void prepare(){
    db_init("");
    db_set_test_name(DATABASE_NAME);
}

void destroy(){
    db_close();
}

TestSuite(database, .init = prepare, .fini = destroy);

// GROUPS
Test(database, create_group_dry) {
    int result = db_add_group(NULL);

    cr_assert_eq(result,-1);
}

Test(database, create_group) {
    int result = db_add_group(GROUP_NAME);

    cr_assert_neq(result,0);
}

Test(database, remove_group_dry) {
    int result = db_remove_group(NULL);

    cr_assert_eq(result,-1);
}

Test(database, remove_group) {
    db_add_group(GROUP_NAME);

    int result = db_remove_group(GROUP_NAME);
    cr_assert_eq(result,1);

    result = db_remove_group(NON_EXISTING_NAME);
    cr_assert_eq(result,1);
}

Test(database, list_group_dry) {
    int count = 0;
    char **result = db_get_group_list(&count);

    cr_assert_eq(result, NULL);
}

Test(database, list_group) {
    db_add_group(GROUP_NAME);
    db_add_group(GROUP_NAME_1);

    char *expected[2] = {
        GROUP_NAME,
        GROUP_NAME_1
    };

    int count = 0;
    char **result = db_get_group_list(&count);
    cr_assert_eq(count, 2);
    
    for (int i = 0; i < count; i++){
        cr_assert_str_eq(result[i],expected[i]);
    }
    
    free_group_list(result, count);
}

// COMMANDS
Test(database, create_command_dry) {
    int result = db_add_command(NULL, NULL, NULL);

    cr_assert_eq(result,-1);
}

Test(database, create_command) {
    db_add_group(GROUP_NAME);

    int result = db_add_command(GROUP_NAME,COMMAND_NAME,LS);
    cr_assert_eq(result,1);

    result = db_add_command(NON_EXISTING_NAME,COMMAND_NAME,LS);

    cr_assert_eq(result,1);   
}

Test(database, get_command_dry) {
    char *result = db_get_command(NULL, NULL);

    cr_assert_eq(result,NULL);
}

Test(database, get_command) {
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);

    char* result = db_get_command(GROUP_NAME,COMMAND_NAME);
    cr_assert_str_eq(result,LS);
    free(result);

    result = db_get_command(GROUP_NAME,COMMAND_NAME_1);
    cr_assert_eq(result,NULL);
    free(result);

    result = db_get_command(GROUP_NAME_1,COMMAND_NAME_1);
    cr_assert_eq(result,NULL);
    free(result);
}

Test(database, remove_command_dry) {
    int result = db_remove_command(NULL, NULL);

    cr_assert_eq(result,-1);
}

Test(database, remove_command) {
    db_add_group(GROUP_NAME);

    int result = db_add_command(GROUP_NAME,COMMAND_NAME,LS);

    result = db_remove_command(GROUP_NAME,COMMAND_NAME);
    cr_assert_eq(result,1);

    result = db_remove_command(NON_EXISTING_NAME,COMMAND_NAME);
    cr_assert_eq(result,1);

    result = db_remove_command(NON_EXISTING_NAME,NON_EXISTING_NAME);
    cr_assert_eq(result,1);
}

// Realtions
Test(database, remove_group_and_commands) {
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_1,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_2,LS);

    int result = db_remove_group(GROUP_NAME);
    cr_assert_eq(result,1);

    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME),NULL);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME_1),NULL);
    cr_assert_eq(db_get_command(GROUP_NAME,COMMAND_NAME_2),NULL);
}

Test(database, list_commands_in_group_dry) {
    int count = 0;
    char **result = db_get_command_list(NULL,&count);
    cr_assert_eq(result, NULL);

    db_add_group(GROUP_NAME);
    result = db_get_command_list(GROUP_NAME,&count);
    cr_assert_eq(result, NULL);
}

Test(database, list_commands_in_group) {
    db_add_group(GROUP_NAME);
    db_add_command(GROUP_NAME,COMMAND_NAME,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_1,LS);
    db_add_command(GROUP_NAME,COMMAND_NAME_2,LS);

    char *expected[3] = {
        COMMAND_NAME,
        COMMAND_NAME_1,
        COMMAND_NAME_2
    };

    int count = 0;
    char **result = db_get_command_list(GROUP_NAME,&count);
    cr_assert_eq(count, 3);
    
    for (int i = 0; i < count; i++){
        cr_assert_str_eq(result[i],expected[i]);
    }
    
    free_group_list(result, count);
}