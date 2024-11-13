#include "libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count){
	if (arr == NULL || size <= 0 || s == NULL || count == NULL) {
        return -1;
    }
	
        int start = 0;
        int end = size - 1;
        int mid;
       

           while (start <= end) {
                (*count)++;
                mid = (end + start) / 2;
                if (mx_strcmp(arr[mid], s) == 0){
                    return mid;
                }else if (mx_strcmp(arr[mid], s) > 0){
                    end = mid - 1;
                }else{
                    start = mid + 1;
                }
            }
	*count = 0;
    return -1;
}



