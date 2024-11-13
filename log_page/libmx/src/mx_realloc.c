#include "libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    
    if (ptr == NULL) {
        return malloc(size);
    }

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    void *res = malloc(size);
    if (res == NULL) {
        return NULL;
    }

    mx_memcpy(res, ptr, size);

    free(ptr);

    return res;
}

