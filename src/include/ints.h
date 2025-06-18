// HumbleOS file: ints.h
// Purpose: Integer types. I could use stdint.h, but I perfer the "[u/s][bits]" format.

#ifndef INTS_H
#define INTS_H

#include <stdint.h> // This is using stdint under the hood

typedef uint8_t u8; // Unsigned 8-bit integer
typedef uint16_t u16; // Unsigned 16-bit integer
typedef uint32_t u32; // Unsigned 32-bit integer
typedef uint64_t u64; // Unsigned 64-bit integer
typedef int8_t s8; // Signed 8-bit integer
typedef int16_t s16; // Signed 16-bit integer
typedef int32_t s32; // Signed 32-bit integer
typedef int64_t s64; // Signed 64-bit integer

typedef unsigned int uint; // Unsigned integer

#endif // INTS_H