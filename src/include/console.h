// HumbleOS file: console.h
// Purpose: Header file for console

#include <stdbool.h>
#include <ints.h>
#include <stdarg.h>
#include <vga.h>

#ifndef CONSOLE_H
#define CONSOLE_H

// Colors
#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define YELLOW 0x6
#define LIGHTGRAY 0x7
#define GRAY 0x8
#define LIGHTBLUE 0x9
#define LIGHTGREEN 0xA
#define LIGHTCYAN 0xB
#define LIGHTRED 0xC 
#define LIGHTMAGENTA 0xD
#define LIGHTYELLOW 0xE
#define WHITE 0xF


// Functions

/// @brief Move console cursor.
/// @param x The X position to move to. (0-79)
/// @param y The Y position to move to. (0-24)
void console_move_cursor(uint x, uint y);

/// @brief Scroll up the screen.
/// @param amt Amount to scroll up by.
void console_scroll_up(uint amt);

/// @brief Handle a carriage return.
void console_handle_carriage_return();

/// @brief Handle a line feed.
void console_handle_line_feed();

/// @brief Handle a newline.
void console_handle_newline();

/// @brief Set the color of the console,
/// @param colorToSet The color to set to.
void console_set_color(u8 colorToSet);

/// @brief Update the cursor.
void console_update_cursor();

/// @brief Advance the cursor.
/// @param amt Amount to advance
void console_advance_cursor(int amt);

void console_hide_cursor();
void console_show_cursor();

/// @brief Print a character to the screen.
/// @param c What character to print.
void putchar(char c);

/// @brief Print a string to the console.
/// @param str What string to print
void print(char *str);

/// @brief Initalize the console
void console_init();

/// @brief Clear the screen
void console_clear_screen();

/// @brief Put a character at a certain position.
/// @param x X position. (0-79)
/// @param y Y position. (0-24)
/// @param ch Character to place.
void put_char_at(uint x, uint y, char ch);

/// @brief Put a color at a certain position.
/// @param x X position. (0-79)
/// @param y Y position. (0-24)
/// @param color Color to place.
void put_color_at(uint x, uint y,char color);

/// @brief Get a character at a certain position.
/// @param x X position. (0-79)
/// @param y Y position. (0-24)
char get_char_at(uint x, uint y);

/// @brief Get a color at a certain position.
/// @param x X position. (0-79)
/// @param y Y position. (0-24)
char get_color_at(uint x, uint y);

/// @brief Get a character from the user
u16 getch();

/// @brief Returns true if you have a key you can get.
bool has_key();

/// @brief The current cursor X.
extern uint console_cursorX;

/// @brief The current cursor Y.
extern uint console_cursorY;

/// @brief The current color.
extern u8 console_color;

#include <printf.h>

/// @brief Log something with the [KRNL] prefix if time isnt ready, or the time in seconds if it is.
void klog(char *section, char *str);

/// @brief Print the prefix for klog.
void klog_prefix(char *section);

/// @brief Log a formatted string with the [KRNL] prefix if time isnt ready, or the time in seconds if it is.
void klogf(char *section, char *format, ...);

/// @brief Set whether to only log klogs to serial.
extern int klog_to_serial_only;

// key codes

#define KEY_UP 0xE075
#define KEY_DOWN 0xE072
#define KEY_RIGHT 0xE074
#define KEY_LEFT 0xE06B

#endif // CONSOLE_H