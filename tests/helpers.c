#include "includes/helpers.h"
#include "../includes/config.h"
#include "../includes/io.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

FILE* switch_stderr_for_temp() {
    FILE *file = fopen(TEMP_FILE, "w+");
    if (file == NULL) {
        cr_assert_fail("Failed to create temporary file: %s", TEMP_FILE);
        return NULL;
    }

    fclose(file);

    FILE *original_stderr = stderr;
    stderr = file;
    return original_stderr;
}

void switch_temp_for_stderr(FILE *original_stderr){
    if (original_stderr == NULL) return;

    stderr = original_stderr;
}

void assert_stderr_equals(const char *expected) {
    if (expected == NULL) {
        cr_assert_fail("expected is NULL");
    }

    char formated[strlen(RED) + strlen(expected) + strlen(RESET) + 1];
    snprintf(formated, sizeof(formated), "%s%s%s", RED, expected, RESET);

    cr_assert_stderr_eq_str(formated);
}

int is_file_empty(char *file_name){
    FILE *file = fopen(file_name,"w");
    if (file == NULL) {
        cr_assert_fail("Failed to read file: %s", file_name);
        return false;
    }

    rewind(file);
    char buffer[1024] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
    fclose(file);

    return bytes_read == 0;
}