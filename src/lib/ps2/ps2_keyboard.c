// HumbleOS file: ps2_keyboard.c
// Purpose: PS2 keyboard code

#include <ps2/ps2_keyboard.h>
#include <ps2/ps2.h>
#include <stddef.h>
#include <console.h>
#include <idt.h>
#include <serial.h>
#include <time.h>
#include <port.h>
#include <ints.h>

static volatile u8 keyboard_scancode_buffer[256];
static volatile u8 keyboard_scancode_buffer_read = 0;
static volatile u8 keyboard_scancode_buffer_write = 0;

// really simple!
static void ps2_interrupt()
{
    u8 scancode = inb(0x60);
    keyboard_scancode_buffer[keyboard_scancode_buffer_write++] = scancode;
}

static u8 wait_for_scancode()
{
    while (keyboard_scancode_buffer_read == keyboard_scancode_buffer_write)
    {
        asm volatile("hlt");
    }

    u8 scancode = keyboard_scancode_buffer[keyboard_scancode_buffer_read++];
    return scancode;
}

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

static struct
{
    bool shift;
    bool caps_lock;
} flags;

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

static inline int isalpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static char numbers_shift[] = ")!@#$%^&*(";

static char shift(char input)
{
    if (input >= '0' && input <= '9')
        return numbers_shift[input - '0'];
    return input; // only handle numbers for now
}

u16 kbd_getch()
{
    while (1)
    {
        u8 scancode = wait_for_scancode();

        if (scancode == 0xF0)
        {
            u8 released = wait_for_scancode();
            if (released == 0x12)
                flags.shift = false;
            continue; /* skip all releases */
        }

        if (scancode == 0x58)
        { /* caps lock */
            flags.caps_lock = !flags.caps_lock;
            continue;
        }

        if (scancode == 0x12)
        { /* shift press */
            flags.shift = true;
            continue;
        }

        if (scancode == 0xE0)
        {
            scancode = wait_for_scancode();

            if (scancode == 0xF0)
            {
                wait_for_scancode();
                continue;
            }

            return 0xE000 | scancode;
        }

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

        return (u16)character;
    }
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