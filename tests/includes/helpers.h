#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

FILE* switch_stderr_for_temp();
void switch_temp_for_stderr(FILE *original_stderr);
int is_file_empty(char *file_name);
void assert_stderr_equals(const char *expected);
char *get_executable_location_path(const char *filename);

#endif