#include "libmx.h"

void swap(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}

int mx_quicksort(char **arr, int left, int right){
    if(arr == NULL){
        return -1;
    }

    int swapCount = 0;

    int i = left, j = right;
    int middle = left + ((right - left + 1) >> 1);
    int pivotLength = mx_strlen(arr[middle]);

    for(; i <= j; ++i, --j){
        for(;mx_strlen(arr[i]) < pivotLength; ++i);
        for(;mx_strlen(arr[j]) > pivotLength; --j);

        if(i < j && mx_strlen(arr[i]) != mx_strlen(arr[j])){
           swap(arr+i, arr+j);
            ++swapCount;
        }
    }

    if(left < j)
        swapCount += mx_quicksort(arr, left, j);

    if(i < right)
        swapCount += mx_quicksort(arr, i, right);

    return swapCount;
}

