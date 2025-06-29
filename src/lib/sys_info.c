// HumbleOS file: sys_info.c
// Purpose: Code for system information

#include <sys_info.h>
#include <console.h>
#include <string.h>
#include <cpuid.h>
#include <multiboot.h>

struct _sys_info sys_info;

void sys_info_gather(multiboot_info_t* mbd) {
    klog("sys_info", "gathering system information...");

    // Bootloader name
    sys_info.bootloaderName = mbd->boot_loader_name ? (char*)mbd->boot_loader_name : "Unknown Bootloader";
    klogf("sys_info", "bootloader name: %s", sys_info.bootloaderName);

    // CPUID
    cpuid_init();
}