#include "libmx.h"

char* mx_strncpy(char *dst, const char *src, int n) {
    int count = 0;

    while (count != n && src[count]) {
        dst[count] = src[count];
        count++;
    }
    return dst;
}

