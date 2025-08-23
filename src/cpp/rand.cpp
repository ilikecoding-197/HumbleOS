// HumbleOS file: rand.cpp
// Purpose: Wrapper over rand.h

extern "C" {
    #include <rand.h>
}

#include <rand.hpp>

namespace std {
    int rand() {
        return ::rand();
    }
}