#include "libmx.h"

void mx_str_reverse(char *s)
{
    int length = mx_strlen(s);

    char *endChar = s + length - 1;
    for(char *i = s; i < endChar; i++)
    {
        mx_swap_char(i, endChar);
        endChar--;
    }
}



