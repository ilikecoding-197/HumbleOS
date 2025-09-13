/*
    ps2.c - code for ps2 devices

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



#include <ps2/ps2.h>
#include <port.h>
#include <stddef.h>
#include <console.h>

ps2_device ps2_device_first;
ps2_device ps2_device_second;
ps2_device *ps2_keyboard;

void _ps2_print_device(ps2_device *device, int id) {
    if (!device->present) {
        klogf("PS2", "device %d is not present", id);
    } else {
        klogf("PS2", "device %d: %s", id, device->name);
    }
}

void ps2_detect_devices() {
    // we will expect the first ps2 to be keyboard. yeah, i know, its bad but..
    // eventually we will add real device dectection
    klog("PS2", "detecting devices...");
    ps2_keyboard = NULL;

    ps2_device_first.type = PS2_DEVICE_KEYBOARD;
    ps2_device_first.name = "PS/2 Keyboard";
    ps2_device_first.present = true;
    ps2_keyboard = &ps2_device_first;

    ps2_device_second.type = PS2_DEVICE_NONE;
    ps2_device_second.name = "No Device";
    ps2_device_second.present = false;

    klog("PS2", "done. here are the devices:");
    
    _ps2_print_device(&ps2_device_first, 1);
    _ps2_print_device(&ps2_device_second, 2);
}

#define PS2_TIMEOUT 100000

static int ps2_wait_input_clear() {
    int timeout = PS2_TIMEOUT;
    while((controller_read_status() & 0x02) && --timeout);
    return timeout != 0; // Returns 0 on timeout, 1 if clear
}

static int ps2_wait_output_full() {
    int timeout = PS2_TIMEOUT;
    while(!(controller_read_status() & 0x01) && --timeout);
    return timeout != 0; // returns 0 on timeout, 1 if data ready
}

void ps2_device_send_command(ps2_device *device, u8 command) {
    if(device == &ps2_device_second) {
        if(!ps2_wait_input_clear()) return;
        outb(0x64, 0xD4); // tell controller next byte is for second port
    }

    if(!ps2_wait_input_clear()) return;
    outb(0x60, command);
}

void ps2_device_send_command_with_argument(ps2_device *device, u8 command, u8 argument) {
    ps2_device_send_command(device, command);

    if(!ps2_wait_input_clear()) return;
    outb(0x60, argument);
}

u8 ps2_device_send_command_with_output(ps2_device *device, u8 command) {
    ps2_device_send_command(device, command);

    if(!ps2_wait_output_full()) return 0xFF; // timeout
    return inb(0x60);
}

u8 ps2_device_send_command_with_output_and_argument(ps2_device *device, u8 command, u8 argument) {
    ps2_device_send_command_with_argument(device, command, argument);

    if(!ps2_wait_output_full()) return 0xFF; // timeout
    return inb(0x60);
}

u8 ps2_device_read(ps2_device *device) {
    
    if(!ps2_wait_output_full()) return 0xFF;
    return inb(0x60);
}
