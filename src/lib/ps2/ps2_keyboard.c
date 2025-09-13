/*
    ps2_keyboard.c - ps2 keyboard code

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

#include <ps2/ps2_keyboard.h>
#include <ps2/ps2.h>
#include <stddef.h>
#include <console.h>
#include <idt.h>
#include <serial.h>
#include <time.h>
#include <port.h>
#include <ints.h>
#include <events.h>
#include <cpu.h>

// keyboard flags
static struct
{
    bool shift;
    bool caps_lock;
} flags;

// scancode buffer
static volatile u8 keyboard_scancode_buffer[256];
static volatile u8 keyboard_scancode_buffer_read = 0;
static volatile u8 keyboard_scancode_buffer_write = 0;

// scan code set 2
static char scan_code_set[256] = {
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\t', '`', '\0',
    '\0', '\0', '\0', '\0', '\0', 'q', '1', '\0',
    '\0', '\0', 'z', 's', 'a', 'w', '2', '\0',
    '\0', 'c', 'x', 'd', 'e', '4', '3', '\0',
    '\0', ' ', 'v', 'f', 't', 'r', '5', '\0',
    '\0', 'n', 'b', 'h', 'g', 'y', '6', '\0',
    '\0', '\0', 'm', 'j', 'u', '7', '8', '\0',
    '\0', ',', 'k', 'i', 'o', '0', '9', '\0',
    '\0', '.', '/', 'l', ';', 'p', '-', '\0',
    '\0', '\0', '\'', '\0', '[', '=', '\0', '\0',
    '\0', '\0', '\n', ']', '\0', '\\', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\b', '\0',
    '\0', '1', '\0', '4', '7', '\0', '\0', '\0',
    '0', '.', '2', '5', '6', '8', '\0', '\0',
    '\0', '+', '3', '-', '*', '9', '\0', '\0',
    '\0', '\0', '\0', '\0'};

// flip a character's case
char flip_case(char input)
{
    if (input >= 'A' && input <= 'Z')
    {
        return 'a' + (input - 'A');
    }
    else if (input >= 'a' && input <= 'z')
    {
        return 'A' + (input - 'a');
    }
    else
        return input;
}

// check if a character is a letter (a-z OR A-Z)
static inline int isalpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// the number keys, shifted
static char numbers_shift[] = ")!@#$%^&*(";

// transform a character when the shidt key is held
static char shift(char input)
{
    if (input >= '0' && input <= '9')
        return numbers_shift[input - '0'];
    return input; // only handle numbers for now
}

bool has_key()
{
    return keyboard_scancode_buffer_read != keyboard_scancode_buffer_write;
}

// ps2 interrupt handler
static void ps2_interrupt()
{
    u8 scancode = inb(0x60);

    Event e;

    // handle special keys first 
    if (scancode == 0xF0)
    {
        serial_print("a");
        u8 released = inb(0x60); // next byte
        e.type = EVENT_KEY_UP;
        e.event.keycode = released;
        trigger_event(&e);
        if (released == 0x12)
            flags.shift = false;
        return;
    }

    if (scancode == 0x58)
    { // caps lock
        flags.caps_lock = !flags.caps_lock;
        return;
    }

    if (scancode == 0x12)
    { // shift press
        flags.shift = true;
        e.type = EVENT_KEY_DOWN;
        e.event.keycode = 0x12;
        trigger_event(&e);
        return;
    }

    // extended keys
    if (scancode == 0xE0)
    {
        scancode = inb(0x60);
        if (scancode == 0xF0)
        {
            scancode = inb(0x60);
            e.type = EVENT_KEY_UP;
            e.event.keycode = 0xE000 | scancode;
            trigger_event(&e);
            return;
        }
        else
        {
            e.type = EVENT_KEY_DOWN;
            e.event.keycode = 0xE000 | scancode;
            trigger_event(&e);
            return;
        }
    }

    // normal keys
    char character = scan_code_set[scancode];
    if (isalpha(character))
    {
        if (flags.caps_lock ^ flags.shift)
            character = flip_case(character);
    }
    else if (flags.shift)
    {
        character = shift(character);
    }

    e.type = EVENT_KEY_DOWN;
    e.event.keycode = (u16)character;
    trigger_event(&e);
}

// getch stuff
static u16 getch_value;

static void getch_event(Event *e) {
    if (e->type != EVENT_KEY_DOWN) return; 

    getch_value = e->event.keycode;
}

u16 getch()
{
    getch_value = 0;
    uint event_id = add_event(getch_event, NULL);

    while (getch_value == 0)
        cpu_hlt();

    remove_event(event_id);
    return getch_value;
}

void ps2_keyboard_flush() {
    keyboard_scancode_buffer_read = keyboard_scancode_buffer_write;
}

void ps2_keyboard_init()
{
    if (ps2_keyboard == NULL)
    {
        klog("PS2KBD", "PS2 keyboard not present - skipping");
        return;
    }

    if (ps2_device_send_command_with_output(ps2_keyboard, 0xEE) == 0xEE)
    {
        klog("PS2KBD", "keyboard responded 0xEE to echo command, we should be okay");
    }
    else
    {
        klog("PS2KBD", "keyboard did not respond 0xEE to echo command, failing");
        ps2_keyboard = NULL;
        return;
    }

    klog("PS2KBD", "keyboard typematic - 30hz, after 1 second");
    ps2_device_send_command_with_argument(ps2_keyboard, 0xF3, 0b01100000);

    attach_interrupt(0x21, ps2_interrupt);
    klog("PS2KBD", "attached interrupt");

    flags.caps_lock = false;
    flags.shift = false;
    klog("PS2KBD", "keyboard is working.");
}