/*
    sys_info.h - system info header

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
#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <multiboot.h>
#include <ints.h>

struct _sys_info {
    // the name of the bootloader that started HumbleOS
    char *bootloaderName;

    // the CPU vendor ID, gotten from CPUID
    char cpu_vendor_id[13];

    // end of the kernel
    uintptr_t kernel_end;

    // after boot, stores the time_ms value after boot (boot time)
    uint boot_time;

    // amount of memory avaiable
    uint mem_amt;
};

// the system information
extern struct _sys_info sys_info;

// gather system information
void sys_info_gather(multiboot_info_t* mbd);

#endif /* SYS_INFO_H */