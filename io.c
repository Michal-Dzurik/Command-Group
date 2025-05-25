#include "includes/io.h"

#include <stdarg.h>
#include <stdio.h>

void error(const char *msg) {
    fprintf(stderr, "\033[31m%s\033[0m", msg);
}

void error_args(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "\033[31m");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\033[0m");
    va_end(args);
}