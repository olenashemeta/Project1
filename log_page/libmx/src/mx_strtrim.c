#include "libmx.h"

char *mx_strtrim(const char *str) {

    int size = mx_strlen(str);
    int start = 0;
    int end = size - 1;

    while (start <= end && mx_isspace(str[start])) {
        start++;
    }

    while (end >= start && mx_isspace(str[end])) {
        end--;
    }

    int trimmed_length = end - start + 1;

    char *trimmed_str = mx_strnew(trimmed_length);

    for (int i = 0; i < trimmed_length; i++) {
        trimmed_str[i] = str[start + i];
    }

    return trimmed_str;
}


