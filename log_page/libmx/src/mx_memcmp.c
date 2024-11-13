#include "libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *compare1 = s1;
    const unsigned char *compare2 = s2;

    for (size_t i = 0; i < n; i++) {
        if (compare1[i] != compare2[i]) {
            return (int)(compare1[i] - compare2[i]);
        }
    }

    return 0;
}




