#include "libmx.h"

char *mx_file_to_str(const char *file) {
    if (!file) {
        return NULL;
    }
    
    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    
    char buffer[512];
    int total_size = 0; 
    char *result_file = NULL;
    
    result_file = mx_strnew(0);
    if (!result_file) {
        close(fd);
        return NULL;
    }

    int reads_data;
    while ((reads_data = read(fd, buffer, sizeof(buffer))) > 0) {
        char *temp = mx_strnew(total_size + reads_data);
        if (!temp) {
            free(result_file);
            close(fd);
            return NULL;
        }

        for (int i = 0; i < total_size; i++) {
            temp[i] = result_file[i];
        }

        for (int i = 0; i < reads_data; i++) {
            temp[total_size + i] = buffer[i];
        }

        total_size += reads_data;
        free(result_file);
        result_file = temp;
    }
    
    if (reads_data < 0) {
        free(result_file);
        close(fd);
        return NULL;
    }

    if (result_file) {
        result_file[total_size] = '\0';
    }
    
    close(fd);
    return result_file;
}

