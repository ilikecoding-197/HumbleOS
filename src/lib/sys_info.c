/*
    sys_info.c - code for gathering system information

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

#include <sys_info.h>
#include <console.h>
#include <string.h>
#include <cpuid.h>
#include <multiboot.h>
#include <panic.h>
#include <ints.h>
struct _sys_info sys_info;

extern void* endKernel;

void sys_info_gather(multiboot_info_t* mbd) {
    klog("sys_info", "gathering system information...");

    // bootloader name
    sys_info.bootloaderName = mbd->boot_loader_name ? (char*)mbd->boot_loader_name : "Unknown Bootloader";
    klogf("sys_info", "bootloader name: %s", sys_info.bootloaderName);

    // check if we have a valid memory map
    if (!(mbd->flags >> 6 & 0x1)) {
        PANIC("INVAILD MEMORY MAP - REFUSING TO CONTINUE")
    }
    klog("sys_info", "vaild memory map found");

    // read the memory map
    u32 mem_amt = 0;
    for (unsigned int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        klogf("sys_info", "memory map entry %d: addr=0x%08x%08x, len=0x%08x%08x, type=%d",
              i / sizeof(multiboot_memory_map_t), mmmt->addr_high, mmmt->addr_low, mmmt->len_high,  mmmt->len_low, mmmt->type);

        // check for kernel memory
        if (mmmt->addr_low == 0x100000) {
            klog("sys_info", "kernel memory found, removing part where kernel is loaded");
            mmmt->len_low -= (u32) &endKernel;
            klogf("sys_info", "new length: 0x%08x%08x", mmmt->len_high, mmmt->len_low);
        }

        if (mmmt->type != 1) {
            klogf("sys_info", "ignoring memory map entry %d, type is not 1 so its unusable", i / sizeof(multiboot_memory_map_t));
            continue;
        }
        mem_amt += mmmt->len_low; // we assume that len_high is 0 (this is 32-bit, and i dont care if we have more than 4GB of memory)
    }
    if (mem_amt < 1024) klogf("sys_info", "total memory: %d bytes", mem_amt);
    else if (mem_amt < 1024 * 1024) klogf("sys_info", "total memory: %dK", mem_amt / 1024);
    else if (mem_amt < 1024 * 1024 * 1024) klogf("sys_info", "total memory: %dM", mem_amt / 1024 / 1024);
    else klogf("sys_info", "total memory: %dG", mem_amt / 1024 / 1024 / 1024);
    sys_info.mem_amt = mem_amt;

    // CPUID
    cpuid_init();
    
    // kernel end
    sys_info.kernel_end = (uintptr_t) &endKernel;
    klogf("sys_info", "kernel end: 0x%08x", sys_info.kernel_end);
}