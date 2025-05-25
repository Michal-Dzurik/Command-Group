#include "../includes/database.h"

#include <criterion/criterion.h>

void prepare(){
    db_init("");
    db_set_test_name("db_test.db");
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
    int result = db_add_group("example_group");

    cr_assert_neq(result,0);
}

Test(database, remove_group_dry) {
    int result = db_remove_group(NULL);

    cr_assert_eq(result,-1);
}

Test(database, remove_group) {
    db_add_group("example_group");

    int result = db_remove_group("example_group");
    cr_assert_eq(result,1);

    result = db_remove_group("non-existing");
    cr_assert_eq(result,1);
}

Test(database, list_group_dry) {
    int count = 0;
    char **result = db_get_group_list(&count);

    cr_assert_eq(result, NULL);
}

Test(database, list_group) {
    db_add_group("example_group");
    db_add_group("example_group2");

    char *expected[2] = {
        "example_group",
        "example_group2"
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
    db_add_group("example_group");

    int result = db_add_command("example_group","example_command","ls");
    cr_assert_eq(result,1);

    result = db_add_command("non-existing","example_command","ls");

    cr_assert_eq(result,1);   
}

Test(database, get_command_dry) {
    char *result = db_get_command(NULL, NULL);

    cr_assert_eq(result,NULL);
}

Test(database, get_command) {
    db_add_command("example_group","example_command","ls");

    char* result = db_get_command("example_group","example_command");
    cr_assert_str_eq(result,"ls");
    free(result);

    result = db_get_command("example_group","f");
    cr_assert_eq(result,NULL);
    free(result);

    result = db_get_command("f","f");
    cr_assert_eq(result,NULL);
    free(result);
}

Test(database, remove_command_dry) {
    int result = db_remove_command(NULL, NULL);

    cr_assert_eq(result,-1);
}

Test(database, remove_command) {
    db_add_group("example_group");

    int result = db_add_command("example_group","example_command","ls");

    result = db_remove_command("example_group","example_command");
    cr_assert_eq(result,1);

    result = db_remove_command("non-existing","example_command");
    cr_assert_eq(result,1);

    result = db_remove_command("non-existing","non-existing");
    cr_assert_eq(result,1);
}

// Realtions
Test(database, remove_group_and_commands) {
    db_add_command("example_group","example_command1","ls");
    db_add_command("example_group","example_command2","ls");
    db_add_command("example_group","example_command3","ls");

    int result = db_remove_group("example_group");
    cr_assert_eq(result,1);

    cr_assert_eq(db_get_command("example_group","example_command1"),NULL);
    cr_assert_eq(db_get_command("example_group","example_command2"),NULL);
    cr_assert_eq(db_get_command("example_group","example_command3"),NULL);
}