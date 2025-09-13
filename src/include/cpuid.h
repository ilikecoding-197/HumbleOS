/*
    cpuid.h - cpuid header file

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
#ifndef CPUID_H
#define CPUID_H

#include <ints.h>
#include <stdbool.h>

// initalize cpuid
void cpuid_init();

// CPUID features
typedef enum {
    SSE3, PCLMUL, DTES64, MONITOR, DS_CPL, VMX, SMX, EST, TM2, SSSE3, CID, SDBG, FMA, CX16, XTPR, PDCM, PCID,
    DCA, SSE4_1, SSE4_2, X2APIC, MOVBE, POPCNT, TSC1, AES, XSAVE, OSXSAVE, AVX, F16C, RDRAND, HYPERVISOR, VME,
    DE, PSE, TSC2, MSR, PAE, MCE, CX8, APIC, SEP, MTRR, PGE, MCA, CMOV, PAT, PSE36, PSN, CLFLUSH, DS, ACPI, MMX,
    FXSR, SSE, SSE2, SS, HTT, TM, IA64, PBE
} cpuid_feat;

// get cupid feature
u8 cpuid_get_feat(cpuid_feat feat);

// check if this system supports rdseed
bool cpuid_supports_rdseed();

#endif /* CPUID_H */