#include "includes/helpers.h"

#include <ctype.h>
#include <string.h>

char* to_lower(char* s) {
    if (!s) return NULL;

    for (char *p = s; *p; p++) {
        *p = (char)tolower((unsigned char)*p);
    }
    return s;
}