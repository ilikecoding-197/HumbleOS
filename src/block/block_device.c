/*
    block_device.c - code for block_device.h

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
#include <string.h>

block_device_t *block_devices[MAX_BLOCK_DEVICES] = { NULL };

bool register_block_device(block_device_t *dev) {
    for (int i = 0; i < MAX_BLOCK_DEVICES; i++) {
        if (!block_devices[i]) {
            block_devices[i] = dev;
            return true;
        }
    }
    return false;
}

block_device_t *get_block_device(const char *name) {
    for (int i = 0; i < MAX_BLOCK_DEVICES; i++) {
        if (block_devices[i] && strcmp(block_devices[i]->name, name) == 0) {
            return block_devices[i];
        }
    }
    return NULL;
}

bool block_read(block_device_t *dev, u64 lba, void *buffer, size_t count) {
    return dev->read(dev, lba, buffer, count);
}

bool block_write(block_device_t *dev, u64 lba, const void *buffer, size_t count) {
    return dev->write(dev, lba, buffer, count);
}