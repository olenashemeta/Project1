#include "libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    if (fd < 0 || buf_size == 0) {
        return -2;
    }

    char ch;
    static char *str = NULL;
    str = mx_strnew(0);
    int read_count = 0;

    while (read(fd, &ch, 1) == 1) {
        read_count++;
        if (ch == delim) {
            break;
        }
        str = mx_strjoin(str, &ch);
    }

    int count = mx_get_char_index(str, delim);

    char *res = NULL;
    //int str_len = mx_strlen(str);

    if (count != -1) {
        if ((size_t)count < buf_size) {
            res = mx_strnew(buf_size);
        } else {
            res = mx_strnew(count);
        }
        if (res == NULL) {
           free(str);
            return -1;
        }

        for (int i = 0; i < count; i++) {
            res[i] = str[i];
        }

        *lineptr = res;
       free(str);
    }
    *lineptr = str;
    return read_count-1;
}



