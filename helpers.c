#include "includes/helpers.h"
#include "includes/config.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <ctype.h>
#include <libgen.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char* to_lower(char* s) {
    if (!s) return NULL;

    for (char* p = s; *p; p++) {
        *p = (char)tolower((unsigned char)*p);
    }
    return s;
}

char* get_storage_path(const char* filename) {
    static char db_path[100];
    char exe_path[100];
    uint32_t size = sizeof(exe_path);

    if (_NSGetExecutablePath(exe_path, &size) != 0) {
        fprintf(stderr, "Executable path too long\n");
        return NULL;
    }

    char* dir_path = strdup(exe_path);
    if (!dir_path) {
        fprintf(stderr, "Strdup failed\n");
        return NULL;
    }

    char* dir = dirname(dir_path);
    snprintf(db_path, sizeof(db_path), "%s/.cg/%s", dir, filename);
    free(dir_path);

    return db_path;
}

int file_exists(char* filename) {
    if (access(filename, F_OK) != -1) return true;

    return false;
}