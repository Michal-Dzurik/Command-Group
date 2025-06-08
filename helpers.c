#ifdef __linux__
#define _POSIX_C_SOURCE 200809L
#endif

#include "includes/helpers.h"
#include "includes/config.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <ctype.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

char* to_lower(char* s) {
    if (!s) return NULL;

    for (char* p = s; *p; p++) {
        *p = (char)tolower((unsigned char)*p);
    }
    return s;
}

char* get_storage_path(const char* filename) {
    static char db_path[PATH_MAX];
    char exe_path[PATH_MAX];

#ifdef __APPLE__
    uint32_t size = sizeof(exe_path);
    if (_NSGetExecutablePath(exe_path, &size) != 0) {
        fprintf(stderr, "Executable path too long\n");
        return NULL;
    }
#elif __linux__
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("readlink");
        return NULL;
    }
    exe_path[len] = '\0';
#else
#error "Unsupported OS"
#endif

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