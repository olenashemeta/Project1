#include "libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    unsigned char *result = b;
    unsigned char replace = (unsigned char)c; 

    for (size_t i = 0; i < len; i++) {
        result[i] = replace;
    }

    return b;
}



