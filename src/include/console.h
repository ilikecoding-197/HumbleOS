/*
    console.h - console header

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

#include <stdbool.h>
#include <ints.h>
#include <stdarg.h>
#include <vga.h>

#ifndef CONSOLE_H
#define CONSOLE_H

// colors

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

// key codes

#define KEY_UP 0xE075
#define KEY_DOWN 0xE072
#define KEY_RIGHT 0xE074
#define KEY_LEFT 0xE06B

// move console cursor
void console_move_cursor(uint x, uint y);

// scroll up the screen amt times
void console_scroll_up(uint amt);

// handle a carriage return
void console_handle_carriage_return();

// handle a line feed
void console_handle_line_feed();

// handle a newline
void console_handle_newline();

// set the color of the console (use VGA_ATTR)
void console_set_color(u8 colorToSet);

// update the cursor
void console_update_cursor();

// advance the cursor amt times
void console_advance_cursor(int amt);

// hide the cursor
void console_hide_cursor();

// show the cursor
void console_show_cursor();

// set the cursor shape (start and end is 0-15)
void console_set_cursor_shape(u8 start, u8 end);

// print a character to the console
void putchar(char c);

// print a string to the console
void print(char *str);

// initalize the console
void console_init();

// clear the console
void console_clear_screen();

// put a character at a certain position
void put_char_at(uint x, uint y, char ch);

// put a color at a certain position
void put_color_at(uint x, uint y,char color);

// get a character at a certain position
char get_char_at(uint x, uint y);

// get a color at a certain position
char get_color_at(uint x, uint y);

// get a character from the user
u16 getch();

// returns true if you have a key you can get
bool has_key();

// x position of cursor
extern uint console_cursorX;

// y position of cursor
extern uint console_cursorY;

// console color
extern u8 console_color;

// start of cursor shape
extern u8 console_shape_start;

// end of cursor shape
extern u8 console_shape_end;

#include <printf.h>

// log something with the [KRNL] prefix if time isnt ready, or the time in seconds if it is
void klog(char *section, char *str);

// print the prefix for klog
void klog_prefix(char *section);

// log a formatted string with the [KRNL] prefix if time isnt ready, or the time in seconds if it is
void klogf(char *section, char *format, ...);

// set whether to only log klogs to serial
extern int klog_to_serial_only;

#endif /* CONSOLE_H */