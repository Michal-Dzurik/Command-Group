#include "includes/helpers.h"
#include "includes/config.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// PROGRAM
char* to_lower(char* s) {
    if (!s) return NULL;

    for (char *p = s; *p; p++) {
        *p = (char)tolower((unsigned char)*p);
    }
    return s;
}

// TESTS
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