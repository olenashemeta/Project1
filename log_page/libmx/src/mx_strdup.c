#include "libmx.h"

char *mx_strdup(const char *str){
int length = mx_strlen(str);

    char *dup = (char*)malloc(length+1);
    if (dup) {
        mx_strcpy(dup, str);
    }
    return dup;
}


