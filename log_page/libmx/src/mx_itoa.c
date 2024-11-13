#include "libmx.h"

unsigned int mx_length_of_int(int number) {
   unsigned int lengthOfInt = 1;

    if (number < 0) {
        lengthOfInt++; 
        number *= -1; 
    }

    while (number >= 10) {
        lengthOfInt++;
        number /= 10;
    }

    return lengthOfInt;
}

char *mx_itoa(int number) {

	if(number == -2147483648){
	
        char *str = (char *)malloc(12);
        if (str) {
            mx_strcpy(str, "-2147483648"); 
            str[11] = '\0';
        }
        return str;
	}
    unsigned int lengthOfInt = mx_length_of_int(number);

   
    char *str = (char *)malloc(lengthOfInt + 1);
    if (!str) {
        return NULL; 
    }

    str[lengthOfInt] = '\0';

    unsigned int index = lengthOfInt - 1;
    if (number < 0) {
        str[0] = '-'; 
        number *= -1;
    }

    while (number > 0) {
        str[index--] = (number % 10) + '0'; 
        number /= 10;
    }

    if (index == 0 && str[0] != '-') {
        str[0] = '0';
    }

    return str;
}


