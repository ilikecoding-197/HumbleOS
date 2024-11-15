#include <console.h>
#include <port.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define INITIAL_COLOR LIGHTGRAY

char *vgaBuff = (char *)0xB8000;

unsigned int cursorX;
unsigned int cursorY;

uint8_t color;

unsigned int amountOfLines;

void _move_cursor(unsigned int x, unsigned int y) {
	x = (x > VGA_WIDTH-1 ? VGA_WIDTH-1 : x); // Limit X
	y = (y > VGA_HEIGHT-1 ? VGA_HEIGHT-1 : y); // Limit Y
	
	cursorX = x;
	cursorY = y;
}

void move_cursor(unsigned int x, unsigned int y) {
	_move_cursor(x, y);

	update_cursor();
}

void scroll_up(unsigned int amt) {
	if (amt == 0) return; // Nothing to scroll by
	
	if (amt >= VGA_HEIGHT) { // Scroll that is off the screen
		unsigned int tempCursorX = cursorX;
		unsigned int tempCursorY = cursorY;
		
		clear_screen();

		move_cursor(tempCursorX, tempCursorY);

		return;
	}

	// Scrool rows up
	for (unsigned int row = amt; row < VGA_HEIGHT; row++) {
		for (unsigned int col = 0; col < VGA_WIDTH; col++) {
			vgaBuff[((row - amt) * VGA_WIDTH + col) * 2] = vgaBuff[(row * VGA_WIDTH + col) * 2]; // Character
			vgaBuff[((row - amt) * VGA_WIDTH + col) * 2 + 1] = vgaBuff[(row * VGA_WIDTH + col) * 2 + 1]; // Color
		}
	}

	// Clear empty rows
	for (unsigned int row = VGA_HEIGHT-amt; row < VGA_HEIGHT; row++) {
		for (unsigned int col = 0; col < VGA_WIDTH; col++) {
			vgaBuff[(col+row*VGA_WIDTH)*2] = ' '; // Space character
			vgaBuff[(col+row*VGA_WIDTH)*2+1] = color; // Color
		}
	}
}

void handle_carriage_return() {
	move_cursor(0, cursorY); // Send cursor to start of line
}

void handle_line_feed() {
	if (amountOfLines < VGA_HEIGHT) {
		move_cursor(cursorX, amountOfLines+1); // Move cursor down
		amountOfLines++;
	
		return;
	}

	scroll_up(1); // Scroll up to leave space for next line
	move_cursor(cursorX, VGA_HEIGHT-1); // Move cursor down
	amountOfLines++;
}

void handle_newline() {
	handle_carriage_return();
	handle_line_feed();
}

void set_color(uint8_t colorToSet) {
	color = colorToSet;
}

void update_cursor()
{
	uint16_t pos = cursorY * VGA_WIDTH + cursorX; // Get position of cursor

	// Send to VGA
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void _advance_cursor(int amt) {
    cursorX += amt;

	// Remove the excess cursorX
    while (cursorX >= VGA_WIDTH) {
        cursorX -= VGA_WIDTH;
        cursorY++;

        if (cursorY >= VGA_HEIGHT) {
            scroll_up(1);
            cursorY = VGA_HEIGHT - 1;
        }

        amountOfLines++;
    }
}

void advance_cursor(int amt) {
	_advance_cursor(amt);

	update_cursor();
}

void putchar(char c) {
	switch (c) {
	case '\r':
		handle_carriage_return();
		break;
	case '\n':
		handle_newline();
		break;
	default:
		uint16_t pos = (cursorY * VGA_WIDTH + cursorX) * 2;
		vgaBuff[pos] = c;
		vgaBuff[pos+1] = color;
					
		advance_cursor(1);
	}
}

void print(char *str) {
	for (char *cPtr = str; *cPtr; cPtr++){
		char c = *cPtr;

		putchar(c);
	}
}

void console_init() {
	move_cursor(0, 0);
	color = INITIAL_COLOR;
	clear_screen();
}

void clear_screen() {
	amountOfLines = 0;
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
		vgaBuff[i] = ' ';
		vgaBuff[i+1] = color;
	}
	move_cursor(0, 0);
}
