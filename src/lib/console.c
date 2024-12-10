#include <console.h>
#include <keyboard.h>
#include <port.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define INITIAL_COLOR LIGHTGRAY

char *console_vgaBuff = (char *)0xB8000;

unsigned int console_cursorX;
unsigned int console_cursorY;

uint8_t console_color;

unsigned int console_amountOfLines;

void console__move_cursor(unsigned int x, unsigned int y) {
	x = (x > VGA_WIDTH-1 ? VGA_WIDTH-1 : x);
	y = (y > VGA_HEIGHT-1 ? VGA_HEIGHT-1 : y);
	
	console_cursorX = x;
	console_cursorY = y;
}

void console_move_cursor(unsigned int x, unsigned int y) {
	console__move_cursor(x, y);

	console_update_cursor();
}

void console_scroll_up(unsigned int amt) {
	if (amt == 0) return;
	
	if (amt >= VGA_HEIGHT) {
		unsigned int tempCursorX = console_cursorX;
		unsigned int tempCursorY = console_cursorY;
		
		console_clear_screen();

		console_move_cursor(tempCursorX, tempCursorY);

		return;
	}
	
	for (unsigned int row = amt; row < VGA_HEIGHT; row++) {
		for (unsigned int col = 0; col < VGA_WIDTH; col++) {
			console_vgaBuff[((row - amt) * VGA_WIDTH + col) * 2] = console_vgaBuff[(row * VGA_WIDTH + col) * 2]; // Character
			console_vgaBuff[((row - amt) * VGA_WIDTH + col) * 2 + 1] = console_vgaBuff[(row * VGA_WIDTH + col) * 2 + 1]; // Color
		}
	}

	for (unsigned int row = VGA_HEIGHT-amt; row < VGA_HEIGHT; row++) {
		for (unsigned int col = 0; col < VGA_WIDTH; col++) {
			console_vgaBuff[(col+row*VGA_WIDTH)*2] = ' ';
			console_vgaBuff[(col+row*VGA_WIDTH)*2+1] = console_color;
		}
	}
}

void console_handle_carriage_return() {
	console_move_cursor(0, console_cursorY);
}

void console_handle_line_feed() {
	if (console_amountOfLines < VGA_HEIGHT) {
		console_move_cursor(console_cursorX, console_amountOfLines+1);
		console_amountOfLines++;
	
		return;
	}

	console_scroll_up(1);
	console_move_cursor(console_cursorX, VGA_HEIGHT-1);
	console_amountOfLines++;
}

void console_handle_newline() {
	console_handle_carriage_return();
	console_handle_line_feed();
}

void console_set_color(uint8_t colorToSet) {
	console_color = colorToSet;
}

void console_update_cursor()
{
	uint16_t pos = console_cursorY * VGA_WIDTH + console_cursorX; // Get position of cursor

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void console__advance_cursor(int amt) {
    console_cursorX += amt;

    while (console_cursorX >= VGA_WIDTH) {
        console_cursorX -= VGA_WIDTH;
        console_cursorY++;

        if (console_cursorY >= VGA_HEIGHT) {
            console_scroll_up(1);
            console_cursorY = VGA_HEIGHT - 1;
        }

        console_amountOfLines++;
    }
}

void console_advance_cursor(int amt) {
	console__advance_cursor(amt);

	console_update_cursor();
}

void putchar(char c) {
	switch (c) {
	case '\r':
		console_handle_carriage_return();
		break;
	case '\n':
		console_handle_newline();
		break;
	case '\t':
		for (int i = 0; i < 4; i++) putchar(' ');
		break;
	default:
		uint16_t pos = (console_cursorY * VGA_WIDTH + console_cursorX) * 2;
		console_vgaBuff[pos] = c;
		console_vgaBuff[pos+1] = console_color;
					
		console_advance_cursor(1);
	}
}

void print(char *str) {
	for (char *cPtr = str; *cPtr; cPtr++){
		char c = *cPtr;

		putchar(c);
	}
}

void console_init() {
	console_move_cursor(0, 0);
	console_color = INITIAL_COLOR;
	console_clear_screen();
}

void console_clear_screen() {
	console_amountOfLines = 0;
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
		console_vgaBuff[i] = ' ';
		console_vgaBuff[i+1] = console_color;
	}
	console_move_cursor(0, 0);
}

void put_char_at(unsigned int x, unsigned int y, char ch) {
	x = (x > VGA_WIDTH-1 ? VGA_WIDTH-1 : x);
	y = (y > VGA_HEIGHT-1 ? VGA_HEIGHT-1 : y);

	uint16_t pos = (x * VGA_WIDTH + y) * 2;
	console_vgaBuff[pos] = ch;
}

void put_color_at(unsigned int x, unsigned int y, char color) {
	x = (x > VGA_WIDTH-1 ? VGA_WIDTH-1 : x);
	y = (y > VGA_HEIGHT-1 ? VGA_HEIGHT-1 : y);

	uint16_t pos = (x * VGA_WIDTH + y) * 2;
	console_vgaBuff[pos+1] = color;
}

char getch() {
	do {} while (keyboard_buffer_read_ptr == keyboard_buffer_write_ptr);

	return keyboard_buffer[keyboard_buffer_read_ptr++];
}