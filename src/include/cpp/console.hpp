#pragma once

extern "C" {
    #include <console.h>
}

#include <cstdarg>
#include "../../include/printf.h"

namespace console {
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
}
