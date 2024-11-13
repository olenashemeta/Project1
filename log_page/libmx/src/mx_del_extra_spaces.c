#include "libmx.h"

bool mx_isspace(char c) {

    if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'){
        return true;
    }
    return false;
}

char *mx_del_extra_spaces(const char *str){
 if (str == NULL) {
        return NULL;
    }
    int start = 0;
    int end = mx_strlen(str);
    while (start <= end && mx_isspace(str[start])) {
        start++;
    }

    while (end >= start && mx_isspace(str[end])) {
        end--;
    }

    if (start > end) {
        return NULL;
    }
 
    int len = end - start + 1;
    char* res = mx_strnew(len+1);
    int j = 0;
    for (int i = start; i < end; i++){
        if (!(mx_isspace(str[i]))) {
                res[j] = str[i];
                j++;
            }   
             if (!(mx_isspace(str[i])) && mx_isspace(str[i + 1])) {
                res[j] = ' ';
                j++;
            }     
    }
        res[j] = '\0';
        return res;
}


