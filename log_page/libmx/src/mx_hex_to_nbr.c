#include "libmx.h"

bool mx_isdigit(int c) {
    bool out = false;
    if (c >= 48 && c <= 57){
        out = true;
    } else {
        out = false;
    }
    return out;
}


unsigned long mx_hex_to_nbr(const char *hex){
   if (hex == NULL) {
      return 0;
   }
	int size = mx_strlen(hex)-1;
	unsigned long result = 0;
	unsigned long temp = 1;
	
	for (int i = 0; i <= size; i++) {
        char current_char = hex[size - i];

        if (mx_isdigit(current_char)) {
            result += (current_char - '0') * temp;
        } else if (current_char >= 'A' && current_char <= 'F') {
            result += (current_char - 'A' + 10) * temp;
        } else if (current_char >= 'a' && current_char <= 'f') {
            result += (current_char - 'a' + 10) * temp;
        } else {
            return 0;
        }
        
        temp *= 16; 
    }
       return result;
}


