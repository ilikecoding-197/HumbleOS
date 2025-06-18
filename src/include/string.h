// HumbleOS file: string.h
// Purpose: Header file for string library.

#ifndef STRING_H
#define STRING_H

#include <stddef.h>

/// @brief Copy some part of memory to another.
/// @param destination Destination pointer,
/// @param source Source pointer.
/// @param num Amount of bytes to copy.
void memcpy(char *destination, const char *source, size_t num);

/// @brief Get the length of a string.
/// @param str The string to get the length of.
/// @return The length of the string.
size_t strlen(char *str);

/// @brief Copy one string to another.
/// @param destination Destination pointer.
/// @param source Source pointer.
void strcpy(char *destination, char *source);

/// @brief Duplicate a string.
/// @param str The string to duplicate.
/// @return Returns NULL on failure, copyied string if not.
char *strdup(char *str);

#endif