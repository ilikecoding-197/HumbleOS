#include <stdbool.h>
#include <stdint.h>
#ifndef CONSOLE_H
#define CONSOLE_H
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
void console_move_cursor(unsigned int x, unsigned int y);
void console_scroll_up(unsigned int amt);
void console_handle_carriage_return();
void console_handle_line_feed();
void console_handle_newline();
void console_set_color(uint8_t colorToSet);
void console_update_cursor();
void console_advance_cursor(int amt);
void putchar(char c);
void print(char *str);
void console_init();
void console_clear_screen();
void put_char_at(unsigned int x, unsigned int y, char ch);
void put_color_at(unsigned int x, unsigned int y, char color);
#endif
