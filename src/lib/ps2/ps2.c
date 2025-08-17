// HumbleOS file: ps2.c
// Purpose: PS2 drivers

#include <ps2/ps2.h>

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
    klog("PS2", "detecting devices...");

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
    return timeout != 0; // Returns 0 on timeout, 1 if data ready
}

void ps2_device_send_command(ps2_device *device, u8 command) {
    // Send a single command byte to the device.
    if(device == &ps2_device_second) {
        if(!ps2_wait_input_clear()) return;
        outb(0x64, 0xD4); // Tell controller next byte is for second port
    }

    if(!ps2_wait_input_clear()) return;
    outb(0x60, command);
}

void ps2_device_send_command_with_argument(ps2_device *device, u8 command, u8 argument) {
    // Send a command followed by a single argument to the device.
    ps2_device_send_command(device, command);

    if(!ps2_wait_input_clear()) return;
    outb(0x60, argument);
}

u8 ps2_device_send_command_with_output(ps2_device *device, u8 command) {
    // Send a command and read a single response byte from the device.
    ps2_device_send_command(device, command);

    if(!ps2_wait_output_full()) return 0xFF; // timeout
    return inb(0x60);
}

u8 ps2_device_send_command_with_output_and_argument(ps2_device *device, u8 command, u8 argument) {
    // Send a command with an argument and read a single response byte.
    ps2_device_send_command_with_argument(device, command, argument);

    if(!ps2_wait_output_full()) return 0xFF; // timeout
    return inb(0x60);
}

u8 ps2_device_read(ps2_device *device) {
    // Read a single byte from the device using polling.
    if(!ps2_wait_output_full()) return 0xFF;
    return inb(0x60);
}
