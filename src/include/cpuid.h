// HumbleOS file: cpuid.h
// Purpose: Header file for cpuid.c

#ifndef CPUID_H
#define CPUID_H

#include <ints.h>
#include <stdbool.h>

/// @brief Initalize CPUID
/// @note  If CPUID is not supported, a PANIC will happen.
void cpuid_init();

// CPUID features
typedef enum {
    SSE3, PCLMUL, DTES64, MONITOR, DS_CPL, VMX, SMX, EST, TM2, SSSE3, CID, SDBG, FMA, CX16, XTPR, PDCM, PCID,
    DCA, SSE4_1, SSE4_2, X2APIC, MOVBE, POPCNT, TSC1, AES, XSAVE, OSXSAVE, AVX, F16C, RDRAND, HYPERVISOR, VME,
    DE, PSE, TSC2, MSR, PAE, MCE, CX8, APIC, SEP, MTRR, PGE, MCA, CMOV, PAT, PSE36, PSN, CLFLUSH, DS, ACPI, MMX,
    FXSR, SSE, SSE2, SS, HTT, TM, IA64, PBE
} cpuid_feat;

/// @brief Get CPUID feature
/// @param feat Feature to get.
/// @return 1 if the feature is supported, 0 otherwise.
u8 cpuid_get_feat(cpuid_feat feat);

/// @brief Check if this CPU supports RDSEED
/// @return true if supported, false otherwise.
bool cpuid_supports_rdseed();

#endif // CPUID_H