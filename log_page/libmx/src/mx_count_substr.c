#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) {
        return -1;
    }

    int count = 0;
    int index = 0;
    int size_sub = mx_strlen(sub);
    int str_len = mx_strlen(str);

    if (size_sub == 0 || str_len < size_sub) {
        return 0; 
    }

    index = mx_get_substr_index(str, sub);

   for (int i = 0; i < str_len; i++){
    if (index == -1){
        break;
    }else{
        
        str += index + size_sub;
        count++;
        index = mx_get_substr_index(str+i, sub);
    }
    
   }
   
  return count;

}



