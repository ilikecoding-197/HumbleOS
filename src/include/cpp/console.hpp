/*
    console.hpp - wrapper around c console

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

#pragma once

extern "C" {
    #include <console.h>

    // undefine the printf.h macros
    #undef vprintf
    #undef printf
    #undef sprintf
    #undef snprintf
}

#include <cstdarg>
#include <climits>
#include "../../include/printf.h"

namespace std {
    // print a string to the console.
    inline void print(char *str) {
        ::print(str);
    }

    // clear the console.
    inline void clear_screen() {
        ::console_clear_screen();
    }

    // kernel message
    inline void klog(char *section, char *str) {
        ::klog(section, str);
    }

    // print formatted message with va_list
    inline int vprintf(const char* format, va_list args) {
        return ::vprintf_(format, args);
    }

    // print formatted message
    inline int printf(const char* format, ...) {
        va_list args;
        va_start(args, format);
        int ret = vprintf(format, args);
        va_end(args);
        return ret;
    }

    // write formatted message to string with character limiting
    inline int snprintf(char *buf, size_t count, char* format, ...) {
        va_list args;
        va_start(args, format);
        int ret = vsnprintf_(buf, count, format, args);
        va_end(args);
        return ret;
    }

    // write formatted message to string
    inline int sprintf(char *buf, char* format, ...) {
        va_list args;
        va_start(args, format);
        int ret = vsnprintf_(buf, ULONG_MAX, format, args);
        va_end(args);
        return ret;
    }

    // set current console color
    inline void set_color(u8 color) {
        ::console_set_color(color);
    }
}
