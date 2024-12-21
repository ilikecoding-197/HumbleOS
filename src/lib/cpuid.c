// Humble OS file: cpuid.c
// Purpose: C code for CPUID

#include <cpuid.h>
#include <stdbool.h>
#include <string.h>

// Variables; see cpuid.h for info.
int cpuid_exists;
char cpuid_vector_str[13];
int cpuid_features_ecx;
int cpuid_features_edx;
int cpuid_rdseed;

bool cpuid_init() {
    __asm__ __volatile__ ("call _cpuid_check"); // Check for CPUID

    // If CPUID does not exist, set default values.
    if (!cpuid_exists) {
        cpuid_features_ecx = cpuid_features_edx = cpuid_rdseed = 0;
        strcpy(cpuid_vector_str, "!!NO CPUID!!");

        return false; // Failed
    }

    __asm__ __volatile__ ("call _cpuid_get_vec_str");  // Get vector string
    __asm__ __volatile__ ("call _cpuid_get_features"); // Get features
    
    return true; // Worked
}