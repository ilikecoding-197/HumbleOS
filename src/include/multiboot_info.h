// HumbleOS file: multiboot_info.h
// Purpose: Header for multiboot_info.c

#ifndef MULTIBOOT_INFO_H
#define MULTIBOOT_INFO_H

#include <ints.h>

/// @brief A multiboot tag
struct multiboot_tag {
    u32 type;
    u32 size;
};

/// @brief The multiboot information struct
struct multiboot_info {
    u32 total_size;
    u32 reserved;
    struct multiboot_tag tags[0];
};

/// @brief The pointer to the multiboot information struct passed in by GRUB in the EBX register
extern struct multiboot_info *multiboot_info;

/// @brief Get a multiboot tag by type
/// @param type The type of the tag to get
/// @return The tag, or NULL if not found
struct multiboot_tag *multiboot_info_get_tag(u32 type);

/// @brief Get the data of a multiboot tag by type
/// @param type The type of the tag to get
/// @return The data of the tag, or NULL if not found
void *multiboot_info_get_tag_data(u32 type);

/// @brief Get the size of a multiboot tag by type (the data size, not the full tag size)
/// @param type The type of the tag to get
/// @return The size of the tag, or 0 if not found
int multiboot_info_get_tag_size(u32 type);

#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME 2

#endif // MULTIBOOT_INFO_H