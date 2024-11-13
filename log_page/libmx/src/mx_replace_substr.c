#include "libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
 
    if (str == NULL || sub == NULL || replace == NULL) {
        return NULL;
    }

   
    int size_str = mx_strlen(str);
    int size_sub = mx_strlen(sub);
    int size_replace = mx_strlen(replace);

   
    int countOfSub = mx_count_words(str, sub[0]) - 1;

  
    int newSize = size_str - (countOfSub * size_sub) + (countOfSub * size_replace) + 1;

    char *result = (char *)malloc(newSize);
    if (result == NULL) {
        return NULL; 
    }

  
    int delimeter = 0;
    int start = 0;

    
    while (start < size_str) {
      
        int found = mx_get_substr_index(str + start, sub);

      
        if (found != -1) {
    
            for (int i = start; i < start + found; i++) {
                if (delimeter < newSize) {
                    result[delimeter++] = str[i];
                } else {
                   
                    free(result);
                    return NULL;
                }
            }

          
            for (int i = 0; i < size_replace; i++) {
                if (delimeter < newSize) {
                    result[delimeter++] = replace[i];
                } else {
               
                    free(result);
                    return NULL;
                }
            }

            start += found + size_sub;
        } else {
           
            for (int i = start; i < size_str; i++) {
                if (delimeter < newSize) {
                    result[delimeter++] = str[i];
                } else {
                   
                    free(result);
                    return NULL;
                }
            }
            break;
        }
    }

   
    result[delimeter] = '\0';

    return result;
}
    
    
    
    
    
    
