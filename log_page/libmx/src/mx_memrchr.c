#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n){

    unsigned char *copy = (unsigned char *)s;
    unsigned char simbol = (unsigned char)c;

    for (size_t i = n-1; i > 0; i--){
        if (copy[i] == simbol){
             return (void *)(copy + i);
        }
    }
    return NULL;
}


