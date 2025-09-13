/*
    ps2.h - header for ps2 functions

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
#ifndef PS2_H
#define PS2_H

#include <ints.h>
#include "ps2/ps2_controller.h"
#include <stdbool.h>

// ps/2 device types
typedef enum {
    PS2_DEVICE_NONE, // none
    PS2_DEVICE_KEYBOARD, // ps2 keyboard
    PS2_DEVICE_MOUSE, // ps2 mouse
    PS2_DEVICE_UNKNOWN // unknown device
} ps2_device_type;

// ps/2 device descriptor
typedef struct {
    ps2_device_type type; // type of the device
    const char *name; // name of the device
    bool present; // if the device is present
} ps2_device;

extern ps2_device ps2_device_first; // the ps2 device on the first port
extern ps2_device ps2_device_second; // the ps2 device on the second port
extern ps2_device *ps2_keyboard; // pointer to the ps2 device for the keyboard

// detect all connected devices
void ps2_detect_devices();

// send a command to a specific device
void ps2_device_send_command(ps2_device *device, u8 command);

// send a command with one argument to a specific device
void ps2_device_send_command_with_argument(ps2_device *device, u8 command, u8 argument);

// send a command and get a response from a specific device
u8 ps2_device_send_command_with_output(ps2_device *device, u8 command);

/// send a command with argument and get response
u8 ps2_device_send_command_with_output_and_argument(ps2_device *device, u8 command, u8 argument);

// generic read for a device (keyboard/mouse/etc.)
u8 ps2_device_read(ps2_device *device);

// commands

#define PS2_DISABLE_SCANNING 0xF5 // disable scanning
#define PS2_ENABLE_SCANNING 0xF4 // enable scanning
#define PS2_IDENTIFY 0xF2 // identify the device

#endif /* PS2_H */