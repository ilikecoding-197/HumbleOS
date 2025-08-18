// HumbleOS file: ps2_keyboard.c
// Purpose: PS2 keyboard code

#include <ps2/ps2_keyboard.h>
#include <ps2/ps2.h>
#include <stddef.h>
#include <console.h>
#include <idt.h>
#include <serial.h>

static void ps2_interrupt() {
    u8 scancode = inb(0x60);

    char buf[256];
    sprintf(buf, "scancode: 0x%02X\n", scancode);

    serial_print(buf);
}

void ps2_keyboard_init() {
    if (ps2_keyboard == NULL) {
        klog("PS2KBD", "PS2 keyboard not present - skipping");
        return;
    }

    if (ps2_device_send_command_with_output(ps2_keyboard, 0xEE) == 0xEE) {
        klog("PS2KBD", "keyboard responded 0xEE to echo command, we should be okay");
    } else {
        klog("PS2KBD", "keyboard did not respond 0xEE to echo command, failing");
        ps2_keyboard = NULL;
        return;
    }

    klog("PS2KBD", "keyboard typematic - 30hz, after 1 second");
    ps2_device_send_command_with_argument(ps2_keyboard, 0xF3, 0b01100000);

    attach_interrupt(0x21, ps2_interrupt);
    klog("PS2KBD", "attached interrupt");
}