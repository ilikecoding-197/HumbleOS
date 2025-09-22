/*
    ramdisk.c - ramdisk block device

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

#include <block/block_device.h>
#include <ints.h>
#include <stdbool.h>
#include <stddef.h>
#include <printf.h>
#include <string.h>

#define MAX_RAMDISK MAX_BLOCK_DEVICES

typedef struct {
    u8 *data;
    size_t size;
} ramdisk_data_t;

static ramdisk_data_t ramdisk_data[MAX_RAMDISK] = { 0 };
static block_device_t ramdisks[MAX_RAMDISK] = { 0 };

static bool ramdisk_read(block_device_t *dev, u64 lba, void *buffer, size_t count) {
    ramdisk_data_t *rd = (ramdisk_data_t *) (dev->driver_data);
    size_t offset = lba * dev->block_size;
    if (offset + count > rd->size) return false;

    memcpy(buffer, rd->data + offset, count);
    return true;
}

static bool ramdisk_write(block_device_t *dev, u64 lba, const void *buffer, size_t count) {
    ramdisk_data_t *rd = (ramdisk_data_t *) (dev->driver_data);
    size_t offset = lba * dev->block_size;
    if (offset + count > rd->size) return false;

    memcpy(rd->data + offset, buffer, count);
    return true;
}

block_device_t *ramdisk_create(u8 *memory, size_t size) {
    for (int i = 0; i < MAX_RAMDISK; i++) {
        block_device_t *dev = &ramdisks[i];

        // check if unused (here by empty name)
        if (dev->name[0] == '\0') {
            memset(dev, 0, sizeof(*dev));
            
            snprintf(dev->name, sizeof(dev->name), "ram%d", i);
            dev->block_size = 512;

            ramdisk_data[i].data = memory;
            ramdisk_data[i].size = size;
            dev->driver_data = &ramdisk_data[i];

            dev->read = ramdisk_read;
            dev->write = ramdisk_write;

            return dev;
        }
    }

    return NULL; // no slots left
}
