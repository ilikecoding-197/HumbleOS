// HumbleOS file: console.c
// Purpose: Console functions

#include <console.h>
#include <port.h>
#include <ints.h>
#include "../include/time.h"
#include <numconvert.h>
#include <stdarg.h>
#include <serial.h>

// Constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define INITIAL_COLOR LIGHTGRAY

/// @brief The VGA buffer.
char *console_vgaBuff = (char *)0xB8000;

uint console_cursorX;
uint console_cursorY;

/// @brief Console color.
u8 console_color;

int klog_to_serial_only = 0;

void console_move_cursor(uint x, uint y)
{
	x = (x > VGA_WIDTH - 1 ? VGA_WIDTH - 1 : x);   // Limit X
	y = (y > VGA_HEIGHT - 1 ? VGA_HEIGHT - 1 : y); // Limit Y

	console_cursorX = x; // Set X
	console_cursorY = y; // Set Y

	console_update_cursor(); // update cursor
}

void console_scroll_up(uint amt)
{
	if (amt == 0 || amt >= VGA_HEIGHT)
		return; // If amount is out of bounds, it does nothing.

	// Scroll everything up
	for (uint row = amt; row < VGA_HEIGHT; row++)
	{
		// For each column
		for (uint col = 0; col < VGA_WIDTH; col++)
		{
			console_vgaBuff[((row - amt) * VGA_WIDTH + col) * 2] = console_vgaBuff[(row * VGA_WIDTH + col) * 2];		 // Character
			console_vgaBuff[((row - amt) * VGA_WIDTH + col) * 2 + 1] = console_vgaBuff[(row * VGA_WIDTH + col) * 2 + 1]; // Color
		}
	}

	// Clear bottom lines
	for (uint row = VGA_HEIGHT - amt; row < VGA_HEIGHT; row++)
	{
		// For each column
		for (uint col = 0; col < VGA_WIDTH; col++)
		{
			console_vgaBuff[(col + row * VGA_WIDTH) * 2] = ' ';				  // Space (blank character)
			console_vgaBuff[(col + row * VGA_WIDTH) * 2 + 1] = console_color; // Color
		}
	}
}

void console_handle_carriage_return()
{
	console_move_cursor(0, console_cursorY); // Move cursor to start
}

void console_handle_line_feed()
{
	if (console_cursorY < VGA_HEIGHT - 1)
	{															   // Before end of screen
		console_move_cursor(console_cursorX, console_cursorY + 1); // Move down

		return;
	}

	console_scroll_up(1);								  // Scroll up screen
	console_move_cursor(console_cursorX, VGA_HEIGHT - 1); // Move to start of line
}

void console_handle_newline()
{
	console_handle_carriage_return(); // Carriage return
	console_handle_line_feed();		  // ...and line feed
}

void console_set_color(u8 colorToSet)
{
	console_color = colorToSet; // Set the color
}

void console_update_cursor()
{
	u16 pos = console_cursorY * VGA_WIDTH + console_cursorX; // Get position of cursor

	outb(0x3D4, 0x0F);					  // Low byte command
	outb(0x3D5, (u8)(pos & 0xFF));		  // Low byte
	outb(0x3D4, 0x0E);					  // High byte command
	outb(0x3D5, (u8)((pos >> 8) & 0xFF)); // High byte
}

void console_advance_cursor(int amt)
{
	console_cursorX += amt; // Increase X

	int scroll_up_amt = 0; // Amount to scroll up by at the end - initialize to 0

	while (console_cursorX >= VGA_WIDTH)
	{								  // While it is overflowing
		console_cursorX -= VGA_WIDTH; // Decreaase
		console_cursorY++;			  // And change Y

		if (console_cursorY >= VGA_HEIGHT)
		{									  // Before end of screen
			scroll_up_amt++;				  // Add a scroll up
			console_cursorY = VGA_HEIGHT - 1; // And fix cursor Y
		}
	}

	if (scroll_up_amt > 0) {
		console_scroll_up(scroll_up_amt); // Scroll screen up
	}

	console_update_cursor(); // Update the cursor
}

void putchar(char c)
{
	switch (c)
	{
	case '\r':							  // Carriage return
		console_handle_carriage_return(); // Handle it
		break;
	case '\n':					  // New line
		console_handle_newline(); // Handle it
		break;
	case '\t': // Tab
		for (int i = 0; i < 4; i++)
			putchar(' '); // 4 spaces for tab
		break;
	default:
		u16 pos = ((console_cursorY * VGA_WIDTH) + console_cursorX) * 2; // Calcuate Position
		console_vgaBuff[pos] = c;										 // Set character
		console_vgaBuff[pos + 1] = console_color;						 // Set color

		console_advance_cursor(1); // Advance cursor
	}
}

void print(char *str)
{
	for (char *cPtr = str; *cPtr; cPtr++)
	{					// Loop through characters
		char c = *cPtr; // Get the character

		putchar(c); // Print it
	}
}

void console_init()
{
	console_color = INITIAL_COLOR; // Set color
	console_clear_screen();		   // Clear screen
}

void console_clear_screen()
{
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2)
	{											// Loop through buffer
		console_vgaBuff[i] = ' ';				// Set character
		console_vgaBuff[i + 1] = console_color; // Set color
	}

	console_move_cursor(0, 0); // Update cursor
}

void put_char_at(uint x, uint y, char ch)
{
	x = (x > VGA_WIDTH - 1 ? VGA_WIDTH - 1 : x);   // Limit X
	y = (y > VGA_HEIGHT - 1 ? VGA_HEIGHT - 1 : y); // Limit Y

	u16 pos = (y * VGA_WIDTH + x) * 2; // Calcuate pos
	console_vgaBuff[pos] = ch;		   // Set character
}

void put_color_at(uint x, uint y, char color)
{
	x = (x > VGA_WIDTH - 1 ? VGA_WIDTH - 1 : x);   // Limit X
	y = (y > VGA_HEIGHT - 1 ? VGA_HEIGHT - 1 : y); // Limit Y

	u16 pos = (y * VGA_WIDTH + x) * 2; // Calcuate pos
	console_vgaBuff[pos + 1] = color;  // Set color
}

char getch()
{
	// We dont have the keyboard driver yet, so just return A
	return 'A';
}

#define klog_print(x) if (klog_to_serial_only) { serial_print(x); } else { print(x); serial_print(x); }


void klog_prefix(char *section)
{
	if (!time_can_use_for_klog)
	{
		klog_print("[KRNL] ");
	}
	else
	{
		uint min = time_ms / (1000 * 60);
		uint sec = (time_ms / 1000) % 60;
		uint ms = time_ms % 1000;
		
		// Format once, use for both
		char time_buffer[32];
		sprintf_(time_buffer, "[%02d:%02d.%03d] ", min, sec, ms);
		
		klog_print(time_buffer);
	}

	klog_print(section);
	klog_print(": ");
}

void klog(char *section, char *str)
{
	klog_prefix(section);
	klog_print(str);
	klog_print("\n");
}

void klogf(char *section, char *fmt, ...)
{
	klog_prefix(section);

	// Format once into a buffer, then output to both
	char buffer[2048];
	va_list args;
	va_start(args, fmt);
	vsnprintf_(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	
	klog_print(buffer);
	console_handle_newline();
	serial_print("\r\n");
}
