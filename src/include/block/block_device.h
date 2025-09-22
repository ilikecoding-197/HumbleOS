/*
    block_device.h - defines a block device struct and a collection of those

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
#ifndef BLOCK_DEVICE_H
#define BLOCK_DEVICE_H

#include <ints.h>
#include <stddef.h>
#include <stdbool.h>

struct block_device;

// read function for a block device
typedef bool (*block_read_fn)(struct block_device *dev, u64 lba, void *buffer, size_t count);

// write function for a block device
typedef bool (*block_write_fn)(struct block_device *dev, u64 lba, const void *buffer, size_t count);

// a block device
typedef struct block_device {
    const char name[32]; // device name, e.g., "ata0"
    u32 block_size; // usually 512 or 4096
    void *driver_data; // pointer to driver-specific struct

    block_read_fn read;
    block_write_fn write;
} block_device_t;

#define MAX_BLOCK_DEVICES 16 // amount of block devices

// block devices
extern block_device_t *block_devices[MAX_BLOCK_DEVICES];

// register a block devices. returns true on success.
bool register_block_device(block_device_t *dev);

// get a block device by name.
block_device_t *get_block_device(const char *name);

// read from a block device
bool block_read(block_device_t *dev, u64 lba, void *buffer, size_t count);

// write to a block device
bool block_write(block_device_t *dev, u64 lba, const void *buffer, size_t count);

#endif /* BLOCK_DEVICE_H */