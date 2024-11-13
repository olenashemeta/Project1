#include "libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    if (nbr == 0) {
        char *res = mx_strnew(1);
        if (res) {
            res[0] = '0';
        }
        return res;
    }

    unsigned long clone = nbr;
    int length = 0;
    while (clone > 0) {
        clone /= 16;
        length++;
    }

    char *res = mx_strnew(length);
    if (!res) {
        return NULL;
    }

    while (nbr != 0) {
        unsigned long temp = (nbr % 16);
        if (temp < 10) {
            res[--length] = (char)(temp + '0'); 
        } else {
            res[--length] = (char)(temp + 'a' - 10); 
        }
        nbr /= 16;
    }
    return res;
}

