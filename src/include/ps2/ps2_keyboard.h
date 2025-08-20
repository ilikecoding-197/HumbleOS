// HumbleOS file: ps2_keyboard.h
// Purpose: Header file for PS2 keyboard

#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include <ints.h>

// special key codes



// Initalize the PS2 keyboard
void ps2_keyboard_init();

// Flush the keyboard buffer
void ps2_keyboard_flush();

#endif // PS2_KEYBOARD_H