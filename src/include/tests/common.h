// HumbleOS file: common.h
// Purpose: Common test functio- wait, macros.

#pragma once

#include "../../settings.h"
#include <console.hpp>
#include <panic.hpp>

#ifdef PANIC_ON_TEST_FAIL
    #define TEST_ASSERT(x, msg, ...) if (!(x)) { PANIC("Test failed: " msg, ##__VA_ARGS__); return; }
#else
    #define TEST_ASSERT(x, msg, ...) if (!(x)) { console::klog("tests", "Test failed: " msg, ##__VA_ARGS__); return; }
#endif

#define TEST_PASS(x) console::klog("tests", "Test passed: " #x)