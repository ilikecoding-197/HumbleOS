// HumbleOS file: sys_info.h
// Purpose: Header file for system information

#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <multiboot.h>

struct _sys_info {
    /// @brief The name of the bootloader that started HumbleOS.
    char *bootloaderName;
};

/// @brief The system information.
extern struct _sys_info sys_info;

/// @brief Gather system information.
void sys_info_gather(multiboot_info_t* mbd);

#endif // SYS_INFO_H