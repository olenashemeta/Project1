#include "libmx.h"


void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    unsigned char *res = dst;
    const unsigned char *sours = src;

    for (size_t i = 0; i < n; ++i) {
        res[i] = sours[i];
    }

    return dst;
}



