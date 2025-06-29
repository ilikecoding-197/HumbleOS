// HumbleOS file: sys_info.c
// Purpose: Code for system information

#include <sys_info.h>
#include <console.h>
#include <string.h>
#include <cpuid.h>
#include <multiboot.h>
#include <panic.h>

struct _sys_info sys_info;

void sys_info_gather(multiboot_info_t* mbd) {
    klog("sys_info", "gathering system information...");

    // Bootloader name
    sys_info.bootloaderName = mbd->boot_loader_name ? (char*)mbd->boot_loader_name : "Unknown Bootloader";
    klogf("sys_info", "bootloader name: %s", sys_info.bootloaderName);

    // Check if we have a valid memory map
    if (!(mbd->flags >> 6 & 0x1)) {
        PANIC("INVAILD MEMORY MAP - REFUSING TO CONTINUE")
    }
    klog("sys_info", "vaild memory map found");

    // Read the memory map
    for (int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        klogf("sys_info", "memory map entry %d: addr=0x%x%x, len=0x%x%x, type=%d",
              i / sizeof(multiboot_memory_map_t), mmmt->addr_high, mmmt->addr_low, mmmt->len_high,  mmmt->len_low, mmmt->type);
    }

    // CPUID
    cpuid_init();
}