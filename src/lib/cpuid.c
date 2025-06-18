// HumbleOS file: cpuid.c
// Purpose: Code for cpuid.h

#include <cpuid.h>
#include <ints.h>
#include <panic.h>
#include <console.h>

// Variables for cpuid.asm (and from)
u32 _cpuid_feat_ecx;
u32 _cpuid_feat_edx;
extern u8 _cpuid_supported;

void cpuid_init() {
    klog("CPUID", "checking for cpuid...");
    __asm__ ("call _cpuid_check"); // Check for CPUID
    if (!_cpuid_supported) {
        PANIC("CPUID NOT SUPPPORTED");
    }
    
    klog("CPUID", "cpuid is supported, getting features...");
    __asm__ ("call _cpuid_get_features");
    klog("CPUID", "features gotten.");

    klogf("CPUID", "ECX: %x", _cpuid_feat_ecx);
    klogf("CPUID", "EDX: %x", _cpuid_feat_edx);
}