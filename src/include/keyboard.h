// HumbleOS file: keyboard.h
// Purpose: Header file for keyboard.

#ifndef KEYBOARD_H
#define KEYBOARD_H

// Constants for keyboard_init
#define KEYBOARD_INIT_SUCCESS 0
#define KEYBOARD_INIT_SELFTEST_FAIL 1
#define KEYBOARD_INIT_NO_PORTS_LEFT 2

/// @brief Initalize keyboard.
/// @return If it worked.
int keyboard_init();

/// @brief If the PS2 controller supportds dual channel.
extern int ps2_dual_channel;

/// @brief If the first PS2 works.
extern int first_ps2_works;

/// @brief The keyboard buffer.
extern char keyboard_buffer[256];

/// @brief Read pointer for keyboard buffer.
extern char keyboard_buffer_read_ptr;

/// @brief Write pointer for keyboard buffer.
extern char keyboard_buffer_write_ptr;

#endif
