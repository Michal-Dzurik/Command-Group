#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

char* to_lower(char* s);

FILE* switch_stderr_for_temp();
void switch_temp_for_stderr(FILE *original_stderr);
int is_file_empty(char *file_name);

#endif