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

struct {
    cpuid_feat feat;
    char *text;
} cpuid_feat_texts[] = {
    {SSE3, "SSE3"},
    {PCLMUL, "PCLMUL"},
    {DTES64, "DTES64"},
    {MONITOR, "MONITOR"},
    {DS_CPL, "DS_CPL"},
    {VMX, "VMX"},
    {SMX, "SMX"},
    {EST, "EST"},
    {TM2, "TM2"},
    {SSSE3, "SSSE3"},
    {CID, "CID"},
    {SDBG, "SDBG"},
    {FMA, "FMA"},
    {CX16, "CX16"},
    {XTPR, "XTPR"},
    {PDCM, "PDCM"},
    {PCID, "PCID"},
    {DCA, "DCA"},
    {SSE4_1, "SSE4_1"},
    {SSE4_2, "SSE4_2"},
    {X2APIC, "X2APIC"},
    {MOVBE, "MOVBE"},
    {POPCNT, "POPCNT"},
    {TSC1, "TSC"},
    {AES, "AES"},
    {XSAVE, "XSAVE"},
    {OSXSAVE, "OSXSAVE"},
    {AVX, "AVX"},
    {F16C, "F16C"},
    {RDRAND, "RDRAND"},
    {HYPERVISOR, "HYPERVISOR"},
    {VME, "VME"},
    {DE, "DE"},
    {PSE, "PSE"},
    {TSC2, "TSC"},
    {MSR, "MSR"},
    {PAE, "PAE"},
    {MCE, "MCE"},
    {CX8, "CX8"},
    {APIC, "APIC"},
    {SEP, "SEP"},
    {MTRR, "MTRR"},
    {PGE, "PGE"},
    {MCA, "MCA"},
    {CMOV, "CMOV"},
    {PAT, "PAT"},
    {PSE36, "PSE36"},
    {PSN, "PSN"},
    {CLFLUSH, "CLFLUSH"},
    {DS, "DS"},
    {ACPI, "ACPI"},
    {MMX, "MMX"},
    {FXSR, "FXSR"},
    {SSE, "SSE"},
    {SSE2, "SSE2"},
    {SS, "SS"},
    {HTT, "HTT"},
    {TM, "TM"},
    {IA64, "IA64"},
    {PBE, "PBE"},
};

void cpuid_init()
{
    klog("CPUID", "checking for cpuid...");
    __asm__("call _cpuid_check"); // Check for CPUID
    if (!_cpuid_supported)
    {
        PANIC("CPUID NOT SUPPPORTED");
    }

    klog("CPUID", "cpuid is supported, getting features...");
    __asm__("call _cpuid_get_features");
    klog("CPUID", "features gotten.");

    klogf("CPUID", "ECX: %x", _cpuid_feat_ecx);
    klogf("CPUID", "EDX: %x", _cpuid_feat_edx);

    klog_prefix("CPUID");
    print("that means this processor supports the following features: ");
    for (int i = 0; i < sizeof(cpuid_feat_texts) / sizeof(cpuid_feat_texts[0]); i++) {
        if (cpuid_get_feat(cpuid_feat_texts[i].feat)) {
            print(cpuid_feat_texts[i].text);
            print(" ");
        }
    }
    print("\n");
}


u8 cpuid_get_feat(cpuid_feat feat) {
    if (feat < 32) {
        return (_cpuid_feat_ecx >> feat) & 1;
    } else {
        return (_cpuid_feat_edx >> (feat - 32)) & 1;
    }
}