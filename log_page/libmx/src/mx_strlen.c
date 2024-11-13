#include "libmx.h"

int mx_strlen(const char *s){
    int sizeOfStr = 0;
    if(!s){
        return 0;
    }

    while(s[sizeOfStr]){
        sizeOfStr++;
    }

    return sizeOfStr;
}



