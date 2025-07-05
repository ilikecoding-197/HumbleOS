// HumbleOS file: ints.h
// Purpose: Integer types. We can't use stdint.h (we dont use any of the default headers),
// but anyway I perfer types like s8, u8, etc. instead of int8_t, uint8_t, etc.

#ifndef INTS_H
#define INTS_H

typedef char s8; // Signed 8-bit integer
typedef short s16; // Signed 16-bit integer
typedef int s32; // Signed 32-bit integer
typedef long long s64; // Signed 64-bit integer
typedef unsigned char u8; // Unsigned 8-bit integer
typedef unsigned short u16; // Unsigned 16-bit integer
typedef unsigned int u32; // Unsigned 32-bit integer
typedef unsigned long long u64; // Unsigned 64-bit integer
typedef unsigned int uint; // Unsigned integer
typedef u32 uintptr_t;
typedef u64 intmax_t;
typedef typeof((char*)0 - (char*)0) ptrdiff_t;

#endif // INTS_H