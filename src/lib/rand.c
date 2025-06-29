// HumbleOS file: rand.c
// Purpose: Random number generator

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

int (*rand)() = NULL; // Function pointer for the random number generator

void rand_init() {
    // Check if RDSEED is supported
    if (cpuid_supports_rdseed()) {
        klog("rand", "RDSEED is supported, using it for randomness.");
        rand = rand_rdseed;
        return;
    }

    // Check if RDRAND is supported
    if (cpuid_get_feat(RDRAND)) {
        klog("rand", "RDRAND is supported, using it for randomness.");
        rand = rand_rdrand;
        return;
    }

    // Fallback to LCG
    klog("rand", "No hardware RNG support, using LCG for randomness.");
    lcg_seed = time_ms; // Seed with current time in milliseconds
    rand = rand_lcg;
}


int rand_in_range(int min, int max) {
    if (min >= max) {
        return 0; // Return 0 for invalid range
    }
    long range = (long)max - (long)min + 1;
    long r = (long)rand();
    if (r < 0) r = -r; // Handle negative random values
    return (int)((r % range) + min); // Get a random number in the range [min, max]
}