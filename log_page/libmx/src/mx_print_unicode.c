#include "libmx.h"

void mx_print_unicode(wchar_t c) {
    char buf[4];
    int countOFSimbol = 0;

    if (c < 0x80) {
        countOFSimbol = 1;
        buf[0] = (char)c;
         write(1, buf, countOFSimbol);
    } else if (c < 0x800) {
        countOFSimbol = 2;
        buf[0] = (char)((c >> 6) | 0xC0);
        buf[1] = (char)((c & 0x3F) | 0x80);
        write(1, buf, countOFSimbol);
    } else if (c < 0x10000) { 
        countOFSimbol = 3;
        buf[0] = (char)((c >> 12) | 0xE0);
        buf[1] = (char)(((c >> 6) & 0x3F) | 0x80);
        buf[2] = (char)((c & 0x3F) | 0x80);
        write(1, buf, countOFSimbol);
    } else {
        countOFSimbol = 4;
        buf[0] = (char)((c >> 18) | 0xF0);
        buf[1] = (char)(((c >> 12) & 0x3F) | 0x80);
        buf[2] = (char)(((c >> 6) & 0x3F) | 0x80);
        buf[3] = (char)((c & 0x3F) | 0x80);
        write(1, buf, countOFSimbol);
    }
 
}


