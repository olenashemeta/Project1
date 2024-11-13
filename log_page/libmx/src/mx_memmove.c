#include "libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    unsigned char *copy = (unsigned char *)dst;
    const unsigned char *sours = (const unsigned char *)src;

    for (size_t i = 0; i < len; i++) {
        copy[i] = sours[i];
    }

    return dst;
}



