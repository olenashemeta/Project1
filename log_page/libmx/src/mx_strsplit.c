#include "libmx.h"

char **mx_strsplit(const char *s, char c) {
    if (!s) {
        return NULL;
    }

    int count_words = mx_count_words(s, c);
    char **result = (char **)malloc((count_words + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
   int start = 0;
    int end;

    for (int i = 0; i < count_words; i++) {
        end = mx_get_char_index(s, c);
        if (end == -1) {
            end = mx_strlen(s);
        }

        result[i] = (char *)malloc((end - start + 1) * sizeof(char));
        if (!result[i]) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        mx_strncpy(result[i], s + start, end - start);
        result[i][end - start] = '\0';
        
        s += end;
        if (*s == c) { 
            s++;
        }
        start = 0;
    }
    result[count_words] = NULL;
    return result;
}


