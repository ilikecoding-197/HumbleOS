// HumbleOS file: tests.cpp
// Purpose: Tests

#include <console.hpp>
#include "../settings.h"
#include <panic.hpp>
#include <tests/tests_heap.h>

#ifdef PANIC_ON_TEST_FAIL
    #define TEST_ASSERT(x) if (!(x)) PANIC("Test failed: " #x)
#else
    #define TEST_ASSERT(x) if (!(x)) console::klog("tests", "Test failed: " #x)
#endif

extern "C" void tests_run() {
    console::klog("tests", "running " NAME " test suite...");
    test_heap_allocation();
    console::klog("tests", "all tests completed!");
}
