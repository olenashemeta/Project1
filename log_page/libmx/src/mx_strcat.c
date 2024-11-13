#include "libmx.h"

char* mx_strcat(char *dst, const char *src) {
    char *end = dst;
    while (*end) {
        end++;
    }

    while (*src) {
        *end++ = *src++;
    }

    *end = '\0';

    return dst;
}


