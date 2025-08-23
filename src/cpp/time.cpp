// HumbleOS file: time.cpp
// Purpose: Wrapper over c time

extern "C" {
#include <time.h>
}

#include <ints.h>

namespace std {
    uint get_time_ms() {
        return ::time_ms;
    }

    void sleep(uint ms) {
        ::sleep(ms);
    }
}