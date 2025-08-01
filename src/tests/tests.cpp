// HumbleOS file: tests.cpp
// Purpose: Tests

#include <console.hpp>
#include "../settings.h"

extern "C" void tests_run() {
    console::klog("tests", "running C++ test suite...");
    
    console::klog("tests", "all tests completed!");
}
