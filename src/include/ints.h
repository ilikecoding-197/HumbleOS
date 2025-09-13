/*
    ints.h - integers (u32, s16, etc). we could use stdint.h but i perfer this format.

    Part of HumbleOS

    Copyright 2025 Thomas Shrader

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#ifndef INTS_H
#define INTS_H

typedef char s8; // signed 8-bit integer
typedef short s16; // signed 16-bit integer
typedef int s32; // signed 32-bit integer
typedef long long s64; // signed 64-bit integer
typedef unsigned char u8; // unsigned 8-bit integer
typedef unsigned short u16; // unsigned 16-bit integer
typedef unsigned int u32; // unsigned 32-bit integer
typedef unsigned long long u64; // unsigned 64-bit integer
typedef unsigned int uint; // unsigned integer
typedef u32 uintptr_t; // integer version of a pointer
typedef u64 intmax_t; // maximum int value
typedef typeof((char*)0 - (char*)0) ptrdiff_t; // the type of the difference of two pointers

#endif /* INTS_H */