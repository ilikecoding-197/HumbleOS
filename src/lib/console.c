// HumbleOS file: console.c
// Purpose: Console functions

#include <console.h>
#include <port.h>
#include <stdint.h>
#include <time.h>

// Constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define INITIAL_COLOR LIGHTGRAY

/// @brief The VGA buffer.
char *console_vgaBuff = (char *)0xB8000;

unsigned int console_cursorX;
unsigned int console_cursorY;

/// @brief Console color.
uint8_t console_color;

void console_move_cursor(unsigned int x, unsigned int y) {
	x = (x > VGA_WIDTH-1 ? VGA_WIDTH-1 : x);   // Limit X
	y = (y > VGA_HEIGHT-1 ? VGA_HEIGHT-1 : y); // Limit Y
	
	console_cursorX = x;                       // Set X
	console_cursorY = y;                       // Set Y

	console_update_cursor();    // update cursor
}

void console_scroll_up(unsigned int amt) {
	if (amt == 0 || amt >= VGA_HEIGHT) return; // If amount is out of bounds, it does nothing.
	
	// Scroll everything up
	for (unsigned int row = amt; row < VGA_HEIGHT; row++) {
		// For each column
		for (unsigned int col = 0; col < VGA_WIDTH; col++) {
			console_vgaBuff[((row - amt) * VGA_WIDTH + col) * 2] = console_vgaBuff[(row * VGA_WIDTH + col) * 2]; // Character
			console_vgaBuff[((row - amt) * VGA_WIDTH + col) * 2 + 1] = console_vgaBuff[(row * VGA_WIDTH + col) * 2 + 1]; // Color
		}
	}

	// Clear bottom lines
	for (unsigned int row = VGA_HEIGHT-amt; row < VGA_HEIGHT; row++) {
		// For each column
		for (unsigned int col = 0; col < VGA_WIDTH; col++) {
			console_vgaBuff[(col+row*VGA_WIDTH)*2] = ' ';             // Space (blank character)
			console_vgaBuff[(col+row*VGA_WIDTH)*2+1] = console_color; // Color
		}
	}
}

void console_handle_carriage_return() {
	console_move_cursor(0, console_cursorY); // Move cursor to start
}

void console_handle_line_feed() {
	if (console_cursorY < VGA_HEIGHT-1) { // Before end of screen
		console_move_cursor(console_cursorX, console_cursorY+1); // Move down
	
		return;
	}

	console_scroll_up(1); // Scroll up screen
	console_move_cursor(console_cursorX, VGA_HEIGHT-1); // Move to start of line
}

void console_handle_newline() {
	console_handle_carriage_return(); // Carriage return
	console_handle_line_feed();       // ...and line feed
}

void console_set_color(uint8_t colorToSet) {
	console_color = colorToSet; // Set the color
}

void console_update_cursor()
{
	uint16_t pos = console_cursorY * VGA_WIDTH + console_cursorX; // Get position of cursor

	outb(0x3D4, 0x0F);                          // Low byte command
	outb(0x3D5, (uint8_t) (pos & 0xFF));        // Low byte
	outb(0x3D4, 0x0E);                          // High byte command
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF)); // High byte
}

void console_advance_cursor(int amt) {
	console_cursorX += amt; // Increase X

	int scroll_up_amt; // Amount to scroll up by at the end

    while (console_cursorX >= VGA_WIDTH) { // While it is overflowing
        console_cursorX -= VGA_WIDTH; // Decreaase
        console_cursorY++; // And change Y

        if (console_cursorY >= VGA_HEIGHT) { // Before end of screen
            scroll_up_amt++; // Add a scroll up
            console_cursorY = VGA_HEIGHT - 1; // And fix cursor Y
        }
    }

	console_scroll_up(scroll_up_amt); // Scroll screen up

	console_update_cursor(); // Update the cursor
}

void putchar(char c) {
	switch (c) {
	case '\r': // Carriage return
		console_handle_carriage_return(); // Handle it
		break;
	case '\n': // New line
		console_handle_newline(); // Handle it
		break;
	case '\t': // Tab
		for (int i = 0; i < 4; i++) putchar(' '); // 4 spaces for tab
		break;
	default:
		uint16_t pos = ((console_cursorY * VGA_WIDTH) + console_cursorX) * 2; // Calcuate Position
		console_vgaBuff[pos] = c; // Set character
		console_vgaBuff[pos+1] = console_color; // Set color
					
		console_advance_cursor(1); // Advance cursor
	}
}

void print(char *str) {
	for (char *cPtr = str; *cPtr; cPtr++) { // Loop through characters
		char c = *cPtr; // Get the character

		putchar(c); // Print it
	}
}

void console_init() {
	console_color = INITIAL_COLOR; // Set color
	console_clear_screen();        // Clear screen
}

void console_clear_screen() {
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) { // Loop through buffer
		console_vgaBuff[i] = ' ';             // Set character
		console_vgaBuff[i+1] = console_color; // Set color
	}

	console_move_cursor(0, 0); // Update cursor
}

void put_char_at(unsigned int x, unsigned int y, char ch) {
	x = (x > VGA_WIDTH-1 ? VGA_WIDTH-1 : x);   // Limit X
	y = (y > VGA_HEIGHT-1 ? VGA_HEIGHT-1 : y); // Limit Y

	uint16_t pos = (y * VGA_WIDTH + x) * 2; // Calcuate pos
	console_vgaBuff[pos] = ch; // Set character
}

void put_color_at(unsigned int x, unsigned int y, char color) {
	x = (x > VGA_WIDTH-1 ? VGA_WIDTH-1 : x);   // Limit X
	y = (y > VGA_HEIGHT-1 ? VGA_HEIGHT-1 : y); // Limit Y

	uint16_t pos = (y * VGA_WIDTH + x) * 2; // Calcuate pos
	console_vgaBuff[pos+1] = color; // Set color
}

char getch() {
	// We dont have the keyboard driver yet, so just return A
	return 'A';
}

void klog(char *section, char *str) {
	if (!time_can_use_for_klog) {
		print("[KRNL] "); // Print prefix
	} else {
		// We need to convert time_ms to a string. Right now, we dont have number
		// to string functions. So I've decided to just do that conversion here for now
		char buf[11]; // Just enough space for a 10 digit number (the biggest size of int)
		int temp = time_ms;
		int i = 0;
		if (temp == 0) {
			buf[i] = '0'; // Add a 0
			i++; // Increment i
		}
		while (temp > 0) {
			buf[i] = (temp % 10) + '0'; // Add the last digit
			temp /= 10; // Remove the last digit
			i++; // Increment i
		}
		buf[i] = '\0'; // Add null terminator

		// Reverse the string
		for (int i = 0; i < strlen(buf)/2; i++) {
			char temp = buf[i];
			buf[i] = buf[strlen(buf)-i-1];
			buf[strlen(buf)-i-1] = temp;
		}

		// Print it
		print("[");
		print(buf);
		print("ms] ");
	}
	print(section);   // Print section
	print(": ");      // Print colon
	print(str);       // Print string
	print("\n");      // Print newline
}