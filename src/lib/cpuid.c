#include <cpuid.h>
#include <stdbool.h>

int cpuid_exists;
char cpuid_vector_str[13];
int cpuid_features_ecx;
int cpuid_features_edx;
int cpuid_rdseed;

bool cpuid_init() {
    __asm__ __volatile__ ("call _cpuid_check");

    if (!cpuid_exists) {
        cpuid_features_ecx = cpuid_features_edx = 0;

        return true;
    }

    __asm__ __volatile__ ("call _cpuid_get_vec_str");
    __asm__ __volatile__ ("call _cpuid_get_features");
    
    return true;
}