// HumbleOS file: string.c
// Purpose: String functions

#include <string.h>
#include <stddef.h>
#include <ints.h>
#include <heap.h>

void memcpy(char *destination, const char *source, size_t num) {
    for (unsigned int i = 0; i < num; i++) { // For each byte up to num
        destination[i] = source[i]; // Copy memoru
    }
}

size_t strlen(char *str) {
    size_t len = 0; // Length starting at zero

    for (char *ch = str; *ch; ch++) { // Get character
        len++; // Update length
    }

    return len;
}

void strcpy(char *dest, char *src) {
    size_t len = strlen(src); // Get length

    for (uint i = 0; i < (uint)len; i++) {
        dest[i] = src[i]; // Copy each byte
    }
}

char *strdup(char *str) {
    char *out = heap_malloc(strlen(str)); // Allocate enough memory
    if (out == NULL) return NULL;         // Error
    
    strcpy(out, str); // Copy string into copy

    return out;
}