#include "libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL){
            return NULL;
    }   

    if (*needle == '\0') {
        return (char *)haystack; 
    }

    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char *)haystack;
        }

        haystack++;
    }

    return NULL;
}



