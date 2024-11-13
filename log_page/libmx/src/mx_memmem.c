#include "libmx.h"


void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len) {
    if (little_len == 0) {
        return (void *)big;
    }
    if (little_len > big_len) {
        return NULL;
    }

    const unsigned char *search = (const unsigned char *)big;
    const unsigned char *sub = (const unsigned char *)little;
    size_t len = big_len - little_len;

    for (size_t i = 0; i <= len; i++) {
          if (search[i] == sub[0] && mx_memcmp(search + i, sub, little_len) == 0) {
            return (void *)(search + i);
        }
    }

    return NULL;
}


