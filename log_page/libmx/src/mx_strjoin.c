#include "libmx.h"

char *mx_strjoin(char const *s1, char const *s2) {
    if (s1 == NULL && s2 == NULL) {
        return NULL;
    }

    char* temp = NULL;
    if (s1 == NULL) {
        temp = mx_strnew(mx_strlen(s2));
        temp = mx_strcat(temp,s2);
        return temp;
    }

    if (s2 == NULL) {
        temp = mx_strnew(mx_strlen(s1));
        temp = mx_strcat(temp,s1);
        return temp;
    }
   
    int sizeOfArr1 = mx_strlen(s1);
    int sizeOfArr2 = mx_strlen(s2);

    int newSize = sizeOfArr1 + sizeOfArr2 + 1;
   char *resOfJoin = (char*)malloc(newSize);
    if (resOfJoin == NULL) {
        return NULL;
    }

    mx_strcat(resOfJoin, s1);
    mx_strcat(resOfJoin, s2);

    return resOfJoin;
}


