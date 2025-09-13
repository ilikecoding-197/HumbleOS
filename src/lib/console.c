/*
	console.c - console code

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



#include <console.h>
#include <port.h>
#include <ints.h>
#include "../include/time.h"
#include "../config.h"
#include <stdarg.h>
#include <serial.h>

#define INITIAL_COLOR WHITE

// vga buffer
char *console_vgaBuff = (char *)0xB8000;

uint console_cursorX;
uint console_cursorY;
u8 console_color;

u8 console_shape_start;
u8 console_shape_end;

int klog_to_serial_only = 0;

void console_move_cursor(uint x, uint y)
{
	// limits
	x = (x > VGA_TEXT_MODE_WIDTH - 1 ? VGA_TEXT_MODE_WIDTH - 1 : x); 
	y = (y > VGA_TEXT_MODE_HEIGHT - 1 ? VGA_TEXT_MODE_HEIGHT - 1 : y);

	console_cursorX = x;
	console_cursorY = y;

	console_update_cursor();
}

void console_scroll_up(uint amt)
{
	if (amt == 0 || amt >= VGA_TEXT_MODE_HEIGHT)
		return; // if amount is out of bounds, it does nothing.

	// scroll everything up
	for (uint row = amt; row < VGA_TEXT_MODE_HEIGHT; row++)
	{
		for (uint col = 0; col < VGA_TEXT_MODE_WIDTH; col++)
		{
			console_vgaBuff[((row - amt) * VGA_TEXT_MODE_WIDTH + col) * 2] = 
				console_vgaBuff[(row * VGA_TEXT_MODE_WIDTH + col) * 2]; // character

			console_vgaBuff[((row - amt) * VGA_TEXT_MODE_WIDTH + col) * 2 + 1] = 
				console_vgaBuff[(row * VGA_TEXT_MODE_WIDTH + col) * 2 + 1]; // color
		}
	}

	// clear bottom lines
	for (uint row = VGA_TEXT_MODE_HEIGHT - amt; row < VGA_TEXT_MODE_HEIGHT; row++)
	{
		for (uint col = 0; col < VGA_TEXT_MODE_WIDTH; col++)
		{
			console_vgaBuff[(col + row * VGA_TEXT_MODE_WIDTH) * 2] = ' ';
			console_vgaBuff[(col + row * VGA_TEXT_MODE_WIDTH) * 2 + 1] = console_color;
		}
	}
}

void console_handle_carriage_return()
{
	console_move_cursor(0, console_cursorY);
}

void console_handle_line_feed()
{
	if (console_cursorY < VGA_TEXT_MODE_HEIGHT - 1)
	{
		console_move_cursor(console_cursorX, console_cursorY + 1);

		return;
	}

	console_scroll_up(1);
	console_move_cursor(console_cursorX, VGA_TEXT_MODE_HEIGHT - 1);
}

void console_handle_newline()
{
	console_handle_carriage_return();
	console_handle_line_feed();
}

void console_set_color(u8 colorToSet)
{
	console_color = colorToSet;
}

void console_update_cursor()
{
	u16 pos = console_cursorY * VGA_TEXT_MODE_WIDTH + console_cursorX;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (u8)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

void console_advance_cursor(int amt)
{
	console_cursorX += amt;

	int scroll_up_amt = 0;

	while (console_cursorX >= VGA_TEXT_MODE_WIDTH)
	{
		console_cursorX -= VGA_TEXT_MODE_WIDTH;
		console_cursorY++;

		if (console_cursorY >= VGA_TEXT_MODE_HEIGHT)
		{
			scroll_up_amt++;
			console_cursorY = VGA_TEXT_MODE_HEIGHT - 1;
		}
	}

	if (scroll_up_amt > 0) {
		console_scroll_up(scroll_up_amt);
	}

	console_update_cursor();
}

void console_hide_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void console_show_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, console_shape_start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, console_shape_end);
}

void console_set_cursor_shape(u8 start, u8 end) {
	console_shape_start = start;
	console_shape_end = end;

	console_show_cursor();
}

void putchar(char c)
{
	switch (c)
	{
	case '\r':
		console_handle_carriage_return();
		break;
	case '\n':
		console_handle_newline();
		break;
	case '\t':
		for (int i = 0; i < 4; i++)
			putchar(' ');
		break;
	default:
		u16 pos = ((console_cursorY * VGA_TEXT_MODE_WIDTH) + console_cursorX) * 2;
		console_vgaBuff[pos] = c;
		console_vgaBuff[pos + 1] = console_color;

		console_advance_cursor(1);
	}
}

void print(char *str)
{
	for (char *cPtr = str; *cPtr; cPtr++)
	{
		char c = *cPtr;

		putchar(c);
	}
}

void console_init()
{
	console_color = INITIAL_COLOR;
	console_clear_screen();

	console_shape_start = 14;
	console_shape_end = 15;

	vga_disable_blink();
}

void console_clear_screen()
{
	for (int i = 0; i < VGA_TEXT_MODE_WIDTH * VGA_TEXT_MODE_HEIGHT * 2; i += 2)
	{
		console_vgaBuff[i] = ' ';
		console_vgaBuff[i + 1] = console_color;
	}

	console_move_cursor(0, 0);
}

void put_char_at(uint x, uint y, char ch)
{
	x = (x > VGA_TEXT_MODE_WIDTH - 1 ? VGA_TEXT_MODE_WIDTH - 1 : x);
	y = (y > VGA_TEXT_MODE_HEIGHT - 1 ? VGA_TEXT_MODE_HEIGHT - 1 : y);

	u16 pos = (y * VGA_TEXT_MODE_WIDTH + x) * 2;
	console_vgaBuff[pos] = ch;
}

void put_color_at(uint x, uint y, char color)
{
	x = (x > VGA_TEXT_MODE_WIDTH - 1 ? VGA_TEXT_MODE_WIDTH - 1 : x);
	y = (y > VGA_TEXT_MODE_HEIGHT - 1 ? VGA_TEXT_MODE_HEIGHT - 1 : y);

	u16 pos = (y * VGA_TEXT_MODE_WIDTH + x) * 2;
	console_vgaBuff[pos + 1] = color;
}

char get_char_at(uint x, uint y) {
	x = (x > VGA_TEXT_MODE_WIDTH - 1 ? VGA_TEXT_MODE_WIDTH - 1 : x);
	y = (y > VGA_TEXT_MODE_HEIGHT - 1 ? VGA_TEXT_MODE_HEIGHT - 1 : y);

	u16 pos = (y * VGA_TEXT_MODE_WIDTH + x) * 2;
	return console_vgaBuff[pos];
}

char get_color_at(uint x, uint y) {
	x = (x > VGA_TEXT_MODE_WIDTH - 1 ? VGA_TEXT_MODE_WIDTH - 1 : x);
	y = (y > VGA_TEXT_MODE_HEIGHT - 1 ? VGA_TEXT_MODE_HEIGHT - 1 : y);

	u16 pos = (y * VGA_TEXT_MODE_WIDTH + x) * 2;
	return console_vgaBuff[pos + 1];
}

#if LOADING_SCREEN
#define klog_print(x) serial_print(x);
#else
#define klog_print(x) if (klog_to_serial_only) { serial_print(x); } else { print(x); serial_print(x); }
#endif

void klog_prefix(char *section)
{
	console_set_color(LIGHTBLUE);
	serial_print("\x1b[94m");

	if (!time_can_use_for_klog)
	{
		klog_print("[KRNL] ");
	}
	else
	{
		uint min = time_ms / (1000 * 60);
		uint sec = (time_ms / 1000) % 60;
		uint ms = time_ms % 1000;
		
		char time_buffer[32];
		sprintf_(time_buffer, "[%02d:%02d.%03d] ", min, sec, ms);
		
		klog_print(time_buffer);
	}

	console_set_color(LIGHTGREEN);
	serial_print("\x1b[92m");
	klog_print(section);
	klog_print(": ");

	console_set_color(WHITE);
	serial_print("\x1b[97m");
}

void klog(char *section, char *str)
{
	u8 oldColor = console_color;

	klog_prefix(section);

	klog_print(str);
	klog_print("\n");

	console_set_color(oldColor);
	serial_print("\x1b[0m");
}

void klogf(char *section, char *fmt, ...)
{
	u8 oldColor = console_color;
	klog_prefix(section);

	char buffer[2048];
	va_list args;
	va_start(args, fmt);
	vsnprintf_(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	
	klog_print(buffer);
	klog_print("\n");

	console_set_color(oldColor);
	serial_print("\x1b[0m");
}
