// HumbleOS file: ps2_keyboard.h
// Purpose: Header file for PS2 keyboard

#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include <ints.h>

// special key codes

#define KEY_UP 0xE075
#define KEY_DOWN 0xE072
#define KEY_RIGHT 0xE074
#define KEY_LEFT 0xE06B

// Initalize the PS2 keyboard
void ps2_keyboard_init();

u16 kbd_getch();

#endif // PS2_KEYBOARD_H