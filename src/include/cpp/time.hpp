// HumbleOS file: time.hpp
// Purpose: Wrapper over C time

#ifndef TIME_HPP
#define TIME_HPP

#include <ints.h>

namespace std {
    uint get_time_ms();
    void sleep(uint ms);
}

#endif // TIME_HPP