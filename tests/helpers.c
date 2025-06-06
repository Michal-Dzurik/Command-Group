#include "includes/helpers.h"
#include "../includes/config.h"
#include "../includes/io.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void assert_stderr_equals(const char* expected) {
    if (expected == NULL) {
        cr_assert_fail("expected is NULL");
    }

    char formated[strlen(RED) + strlen(expected) + strlen(RESET) + 1];
    snprintf(formated, sizeof(formated), "%s%s%s", RED, expected, RESET);

    cr_assert_stderr_eq_str(formated);
}