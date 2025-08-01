// HumbleOS file: cpuid.c
// Purpose: Code for cpuid.h

#include <cpuid.h>
#include <ints.h>
#include <panic.h>
#include <console.h>
#include <sys_info.h>
#include <stdbool.h>
#include <serial.h>

// Variables from cpuid.asm
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

typedef struct {
    u32 eax;
    u32 ebx;
    u32 ecx;
    u32 edx;
} cpuid_result;
static cpuid_result cpuid(u32 leaf) {
    cpuid_result result;

    __asm__ volatile (
        "cpuid"
        : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx), "=d"(result.edx)
        : "a"(leaf)
    );

    return result;
}

static cpuid_result cpuid_with_subleaf(u32 leaf, u32 subleaf) {
    cpuid_result result;

    __asm__ volatile (
        "cpuid"
        : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx), "=d"(result.edx)
        : "a"(leaf), "c"(subleaf)
    );

    return result;
}

enum {
    CPUID_GET_VENDOR_ID,
    CPUID_GET_FEATURES
};

u32 _cpuid_feat_ecx, _cpuid_feat_edx;

#define GET_BYTE(val, index) ((val >> (index * 8)) & 0xFF)
void cpuid_init()
{
    klog("CPUID", "checking for cpuid...");
    __asm__("call _cpuid_check"); // Check for CPUID
    if (!_cpuid_supported)
    {
        PANIC("CPUID NOT SUPPPORTED");
    }

    klog("CPUID", "cpuid is supported, getting features...");

    cpuid_result result;
    result = cpuid(CPUID_GET_FEATURES);
    _cpuid_feat_ecx = result.ecx; // Store ECX features
    _cpuid_feat_edx = result.edx; // Store EDX features

    klog("CPUID", "features gotten.");

    klogf("CPUID", "ECX: %08x", _cpuid_feat_ecx);
    klogf("CPUID", "EDX: %08x", _cpuid_feat_edx);

    klog_prefix("CPUID");
    print("that means this processor supports the following features: ");
    serial_print("that means this processor supports the following features: ");
    for (long unsigned i = 0; i < sizeof(cpuid_feat_texts) / sizeof(cpuid_feat_texts[0]); i++) {
        if (cpuid_get_feat(cpuid_feat_texts[i].feat)) {
            print(cpuid_feat_texts[i].text);
            serial_print(cpuid_feat_texts[i].text);
            print(" ");
            serial_print(" ");
        }
    }
    print("\n");
    serial_print("\r\n");

    klog("CPUID", "moving on to cpu vendor...");

    result = cpuid(CPUID_GET_VENDOR_ID);

    // So the vendor ID is stored in EBX, ECX, and EDX. Below is a table where each character is.
    // MSB = Most Significant Byte
    // LSB = Least Significant Byte
    // Cell in table - character position
    //       MSB         LSB
    // EBX = '3' '2' '1' '0'
    // EDX = '7' '6' '5' '4'
    // ECX = 'B' 'A' '9' '8'
    //
    // So this part gets each character.
    sys_info.cpu_vendor_id[0] =  GET_BYTE(result.ebx, 0);
    sys_info.cpu_vendor_id[1] =  GET_BYTE(result.ebx, 1);
    sys_info.cpu_vendor_id[2] =  GET_BYTE(result.ebx, 2);
    sys_info.cpu_vendor_id[3] =  GET_BYTE(result.ebx, 3);
    sys_info.cpu_vendor_id[4] =  GET_BYTE(result.edx, 0);
    sys_info.cpu_vendor_id[5] =  GET_BYTE(result.edx, 1);
    sys_info.cpu_vendor_id[6] =  GET_BYTE(result.edx, 2);
    sys_info.cpu_vendor_id[7] =  GET_BYTE(result.edx, 3);
    sys_info.cpu_vendor_id[8] =  GET_BYTE(result.ecx, 0);
    sys_info.cpu_vendor_id[9] =  GET_BYTE(result.ecx, 1);
    sys_info.cpu_vendor_id[10] = GET_BYTE(result.ecx, 2);
    sys_info.cpu_vendor_id[11] = GET_BYTE(result.ecx, 3);
    sys_info.cpu_vendor_id[12] = '\0';

    klogf("CPUID", "CPU vendor ID gotten: %s", sys_info.cpu_vendor_id);
}

u8 cpuid_get_feat(cpuid_feat feat) {
    if (feat < 32) {
        return (_cpuid_feat_ecx >> feat) & 1;
    } else {
        return (_cpuid_feat_edx >> (feat - 32)) & 1;
    }
}

bool cpuid_supports_rdseed() {
    cpuid_result result = cpuid_with_subleaf(7, 0);
    return (result.ebx >> 18) & 1; // Check the RDSEED
}