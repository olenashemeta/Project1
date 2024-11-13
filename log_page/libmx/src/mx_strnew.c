#include "libmx.h"

char *mx_strnew(const int size){
   if(size < 0){
       return NULL;
   }
    const int newSize = size + 1;
      char *string =(char *) malloc(newSize);
      for(int i = 0; i < newSize; i++){
          string[i] = '\0';
      }
      return string;
}


