#include "../includes/helpers.h"

#include <criterion/criterion.h>

TestSuite(helpres);

Test(helpers, null_lower_case) {
    char *str = to_lower(NULL);

    cr_assert_eq(str,NULL);
}

Test(helpers, string_lower_case) {
    char *input = strdup("I like One Direction");
    char *str = to_lower(input);

    cr_assert_str_eq(str,"i like one direction");

    free(input);
}