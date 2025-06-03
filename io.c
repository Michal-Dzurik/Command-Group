#include "includes/io.h"

#include <stdarg.h>
#include <stdio.h>

void error(const char *msg) {
    fprintf(stderr, "%s%s%s", RED, msg, RESET);
}

void error_args(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, RED);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, RESET);
    va_end(args);
}