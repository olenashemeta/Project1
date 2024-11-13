#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub){

    if (str == NULL)
    {
        return -2;
    }
    if (sub == NULL)
    {
        return -2;
    }
    
    int index = -1;

    int size_str = mx_strlen(str);
    int size_sub = mx_strlen(sub);

    if (size_sub == 0 || size_sub > size_str) {
        return -1;
    }

    for (int i = 0; i < size_str; i++)
    {
        index = i;

            if (str[i] == sub[0]){
                
               for (int j = 1; j < size_sub; j++){
                i++;
                    if (str[i] != sub[j]){
                        return -1;
                    }                    
                }
                return index;
            }
    }
        return -1;
}



