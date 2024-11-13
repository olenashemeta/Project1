#include "libmx.h"

int mx_count_words(const char *str, char delimiter){

if (str == NULL){
    return -1;
 }
    int countWorld = 0;
    int sizeOfStr = 0;
    
     while(str[sizeOfStr]){
            sizeOfStr++;
        }
        if(sizeOfStr == 0){
                return 0;
        }

    for(int i = 0; i < sizeOfStr-1; i++){
         if(str[i] == delimiter && str[i+1] != delimiter){
             countWorld++;
         }
    }

    if(str[0] != delimiter){
     countWorld++;
    }

       return countWorld;
}



