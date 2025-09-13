/*
    rand.c - random support for HumbleOS

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

#include <rand.h>
#include <cpuid.h>
#include <ints.h>
#include <time.h>
#include <console.h>
#include <stdbool.h>
#include <stddef.h>

// LCG
static u32 lcg_seed = 0;
static u32 lcg_a = 1103515245;
static u32 lcg_c = 12345;

int rand_lcg() {
    lcg_seed = (lcg_a * lcg_seed + lcg_c) & 0xFFFFFFFF;
    return lcg_seed;
}

// RDSEED
static int rand_rdseed() {
    u32 result;
    __asm__ volatile ("rdseed %0" : "=r"(result));
    return result;
}

// RDRAND
static int rand_rdrand() {
    u32 result;
    __asm__ volatile ("rdrand %0" : "=r"(result));
    return result;
}

int (*rand)() = NULL; // function pointer for the random number generator

void rand_init() {
    // check if RDSEED is supported
    if (cpuid_supports_rdseed()) {
        klog("rand", "RDSEED is supported, using it for randomness");
        rand = rand_rdseed;
        return;
    }

    // check if RDRAND is supported
    if (cpuid_get_feat(RDRAND)) {
        klog("rand", "RDRAND is supported, using it for randomness");
        rand = rand_rdrand;
        return;
    }

    // fallback to LCG
    klog("rand", "no hardware RNG support, using LCG for randomness");
    lcg_seed = time_ms; // seed with current time in milliseconds
    rand = rand_lcg;
}


int rand_in_range(int min, int max) {
    if (min >= max) {
        return 0; // return 0 for invalid range
    }
    long range = (long)max - (long)min + 1;
    long r = (long)rand();
    if (r < 0) r = -r; // handle negative random values
    return (int)((r % range) + min); // get a random number in the range [min, max]
}