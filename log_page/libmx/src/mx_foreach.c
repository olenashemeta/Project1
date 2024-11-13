#include "libmx.h"


void mx_foreach(int *arr, int size, void (*f)(int)) {
    if (size > 0 && arr != NULL && f != NULL) {
        for (int i = 0; i < size; i++) {
            f(arr[i]);
        }
    }
}



