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
void move_cursor(unsigned int x, unsigned int y);
void scroll_up(unsigned int amt);
void scroll_down(unsigned int amt);
void handle_newline();
void set_color(uint8_t colorToSet);
void update_cursor();
void advance_cursor(int amt);
void print(char *str);
void console_init();
void clear_screen();
#endif
