#include "libmx.h"

void mx_printchar (char c){

    char *char_to_write = &c;

   write(1, char_to_write , 1);
}


