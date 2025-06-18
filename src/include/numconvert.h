// HumbleOS file: numconvert.h
// Purpose: Header file for converting numbers

#ifndef NUMCONVERT_H
#define NUMCONVERT_H

#include <ints.h>

/// @brief Convert a number into a buffer.
/// @param num The number to convert
/// @param buffer The buffer to write the number to (MUST BE ENOUGH SPACE, USE GET_MAX_CHARS_BASE TO GET THE MAX CHARS)
/// @param base The base to convert the number to
/// @param pad The number of characters to pad the number with (0s)
/// @return The number of characters written to the buffer
uint num_to_str(int num, char *buffer, uint base, uint pad);

#define GET_MAX_CHARS_BASE(base) (sizeof(int) * 8 / (base == 10 ? 3 : 4) + 2)

#endif // NUMCONVERT_H