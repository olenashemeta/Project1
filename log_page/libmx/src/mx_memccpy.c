#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    unsigned char *res = dst;
    const unsigned char *sours = src;
    unsigned char lastSimbol = (unsigned char)c;

    for (size_t i = 0; i < n; i++){
       res[i] = sours[i];
        if (sours[i] == lastSimbol) {
            return (res + i + 1);
        }
    }

    return NULL;
}



