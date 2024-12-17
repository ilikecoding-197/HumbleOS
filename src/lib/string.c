#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <heap.h>

void memcpy(char *destination, const char *source, size_t num) {
    for (unsigned int i = 0; i < num; i++) {
        destination[i] = source[i];
    }
}

int strlen(char *str) {
    int len = 0;

    for (char *ch = str; *ch; ch++);

    return len;
}

void strcpy(char *dest, char *src) {
    size_t len = strlen(src);

    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

char *strdup(char *str) {
    char *out = heap_malloc(strlen(str));
    if (out == NULL) return NULL;
    
    strcpy(out, str);
}