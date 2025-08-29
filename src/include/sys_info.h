// HumbleOS file: sys_info.h
// Purpose: Header file for system information

#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <multiboot.h>
#include <ints.h>

struct _sys_info {
    /// @brief The name of the bootloader that started HumbleOS.
    char *bootloaderName;

    /// @brief The CPU vendor ID, gotten from CPUID.
    char cpu_vendor_id[13];

    /// @brief End of the kernel.
    uintptr_t kernel_end;

    /// @brief After boot, stores the time_ms value after boot (boot time).
    uint boot_time;

    /// @brief Amount of memory avaiable.
    uint mem_amt;
};

/// @brief The system information.
extern struct _sys_info sys_info;

/// @brief Gather system information.
void sys_info_gather(multiboot_info_t* mbd);

#endif // SYS_INFO_H