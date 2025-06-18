// HumbleOS file: cpuid.c
// Purpose: Code for cpuid.h

#include <cpuid.h>
#include <stdint.h>
#include <panic.h>
#include <console.h>

// Variables for cpuid.asm (and from)
uint32_t _cpuid_feat_ecx;
uint32_t _cpuid_feat_edx;
extern uint8_t _cpuid_supported;

void cpuid_init() {
    klog("CPUID", "checking for cpuid...");
    __asm__ ("call _cpuid_check"); // Check for CPUID
    if (!_cpuid_supported) {
        PANIC("CPUID NOT SUPPPORTED");
    }
    
    klog("CPUID", "cpuid is supported, getting features...");
    __asm__ ("call _cpuid_get_features");
    klog("CPUID", "features gotten.");
}