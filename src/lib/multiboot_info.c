// HumbleOS file: multiboot_info.c
// Purpose: Handles the multiboot information struct passed in by GRUB

#include <multiboot_info.h>
#include <ints.h>
#include <stddef.h>

struct multiboot_info *multiboot_info;

struct multiboot_tag *multiboot_info_get_tag(u32 type) {
    struct multiboot_tag *tag = multiboot_info->tags;
    while (tag->type != 0) {
        if (tag->type == type) {
            return tag;
        }
        tag = (struct multiboot_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
    }
    return NULL;
}

void *multiboot_info_get_tag_data(u32 type) {
    struct multiboot_tag *tag = multiboot_info_get_tag(type);
    if (tag == NULL) {
        return NULL;
    }
    return (void *)((u8 *)tag + 8);
}

int multiboot_info_get_tag_size(u32 type) {
    struct multiboot_tag *tag = multiboot_info_get_tag(type);
    if (tag == NULL) {
        return 0;
    }
    return tag->size - 8;
}