// HumbleOS file: console.hpp
// Purpose: C++ console header

#pragma once

extern "C" {
    #include <console.h>

    #undef vprintf
    #undef printf
    #undef sprintf
    #undef snprintf
}

#include <cstdarg>
#include <climits>
#include "../../include/printf.h"

namespace std {
    inline void print(char *str) {
        ::print(str);
    }

    inline void clear_screen() {
        ::console_clear_screen();
    }

    inline void klog(char *section, char *str) {
        ::klog(section, str);
    }

    inline int vprintf(const char* format, va_list args) {
        return ::vprintf_(format, args);
    }

    inline int printf(const char* format, ...) {
        va_list args;
        va_start(args, format);
        int ret = vprintf(format, args);
        va_end(args);
        return ret;
    }

    inline int snprintf(char *buf, size_t count, char* format, ...) {
        va_list args;
        va_start(args, format);
        int ret = vsnprintf_(buf, count, format, args);
        va_end(args);
        return ret;
    }

    inline int sprintf(char *buf, char* format, ...) {
        va_list args;
        va_start(args, format);
        int ret = vsnprintf_(buf, ULONG_MAX, format, args);
        va_end(args);
        return ret;
    }

    inline void set_color(u8 color) {
        ::console_set_color(color);
    }
}
