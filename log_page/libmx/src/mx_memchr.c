#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n){

    unsigned char *copy = (unsigned char *)s;
    unsigned char simbol = (unsigned char)c;

    for (size_t i = 0; i < n; i++){
        if (copy[i] == simbol){
             return (void *)(copy + i);
        }
    }
    return NULL;
}



