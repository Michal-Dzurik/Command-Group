#include "includes/helpers.h"
#include "includes/config.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

char* to_lower(char* s) {
    if (!s) return NULL;

    for (char *p = s; *p; p++) {
        *p = (char)tolower((unsigned char)*p);
    }
    return s;
}
