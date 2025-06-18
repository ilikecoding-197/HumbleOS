// HumbleOS file: sys_info.c
// Purpose: Code for system information

#include <sys_info.h>
#include <console.h>
#include <multiboot_info.h>
#include "../include/string.h"

struct _sys_info sys_info;

void sys_info_gather() {
    klog("sys_info", "gathering system information...");

    // Bootloader name
    sys_info.bootloaderName = multiboot_info_get_tag_data(MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME);
    char buffer[100];
    const char *BOOTLOADER_PREFIX = "bootloader name: ";
    strcpy(buffer, BOOTLOADER_PREFIX);
    strcpy(buffer+strlen(BOOTLOADER_PREFIX), sys_info.bootloaderName);
    klog("sys_info", buffer);
}