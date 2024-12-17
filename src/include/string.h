#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void memcpy(char *destination, const char *source, size_t num);
int strlen(char *str);
void strcpy(char *dest, char *src);
char *strdup(char *str);

#endif