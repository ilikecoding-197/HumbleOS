// HumbleOS file: main.c
// Purpose: The kernel code!

#include <console.h>
#include <idt.h>
#include <pic.h>
#include <exception_handlers.h>
#include <keyboard.h>
#include <stdbool.h>
#include <panic.h>
#include "settings.h"
#include <heap.h>
#include "apps/hello.h"
#include "apps/reboot.h"
#include "apps/info.h"
#include "apps/shutdown.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct component_t {
	char *name;
	bool (*run)(void);
	void (*after)(void);
} component_t;

void component_t_install(component_t component) {
	print("Installing ");
	console_set_color(GREEN);
	print(component.name);
	console_set_color(LIGHTGRAY);
	
	bool result = component.run();
	print(" [ ");
	console_set_color(result ? GREEN : RED);
	print(result ? "OK" : "FAIL");
	console_set_color(LIGHTGRAY);
	print(" ]\n");

	component.after();
}

void draw_box(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
	put_char_at(x1, y1, '\xda');
	put_char_at(x2, y1, '\xbf');
	put_char_at(x1, y2, '\xc0');
	put_char_at(x2, y2, '\xd9');

	for (unsigned int x = x1+1; x < x2; x++) {
		put_char_at(x, y1, '\xc4');
		put_char_at(x, y2, '\xc4');
	}

	for (unsigned int y = y1+1; y < y2; y++) {
		put_char_at(x1, y, '\xb3');
		put_char_at(x2, y, '\xb3');
	}
}

typedef struct {
	char *name;
	char *desc;
	void (*main)(void);
} app_t;

void main_menu() {
	app_t apps[] = {
		{ "Hello", "\"Hello, world!\" app", hello_main },
		{ "Reboot...", "Reboots the system.", reboot_main },
		{ "Info", "Info about HumbleOS.", info_main },
		{ "Shutdown...", "Shutdowns the system.", shutdown_main }
	};

	unsigned int select = 0;

	while (true) {
		console_clear_screen();

		draw_box(1, 1, VGA_WIDTH-2, VGA_HEIGHT-2);

		console_move_cursor(30, 0);
		print("HumbleOS Main Menu");

		console_move_cursor(1,VGA_HEIGHT-1);
		print("\x18\x19 to move, Enter to select. \xb3 ");
		print(apps[select].desc);

		for (unsigned int i = 0; i < sizeof(apps)/sizeof(app_t); i++) {
			console_move_cursor(3, 3+i);

			if (select == i) {
				console_set_color(0x70);
			} else {
				console_set_color(0x07);
			}

			print(apps[i].name);
		}

		console_set_color(0x07);

		while (1) {
			char ch = getch();

			if (ch == 0) {
				switch (getch()) {
					case 'W':
						if (select != 0) select--;
						
						break;
					case 'S':
						if (select != (sizeof(apps)/sizeof(app_t))-1) select++;
						
						break;
				}

				break;
			} else if (ch == '\n') {
				console_clear_screen();

				apps[select].main();
				break;
			}
		}

		//return;
	}
}

bool pic_install() {
	pic_init();
	return true;
}

bool idt_install() {
	idt_init();
	return true;
}

bool exception_handlers_install() {
	exception_handlers_init();
	return true;
}

bool heap_install() {
	heap_init();
	return true;
}

int keyboard_install_error;

bool keyboard_install() {
	keyboard_install_error = keyboard_init();
	
	return keyboard_install_error == KEYBOARD_INIT_SUCCESS;
}

void component_after_stub() {}

void print_yes_no(bool val) {
	if (val) {
		console_set_color(GREEN);
		print("Yes\n");
		console_set_color(LIGHTGRAY);
	} else {
		console_set_color(RED);
		print("No\n");
		console_set_color(LIGHTGRAY);
	}
}

void keyboard_after() {
	switch (keyboard_install_error) {
		case KEYBOARD_INIT_SELFTEST_FAIL:
			console_set_color(RED);
			print("Error: Selftest failed\n");
			console_set_color(LIGHTGRAY);
			return;
		case KEYBOARD_INIT_NO_PORTS_LEFT:
			console_set_color(RED);
			print("Error: No ports left\n");
			console_set_color(LIGHTGRAY);
			return;
	}

	print("First PS/2 port: ");
	print_yes_no(first_ps2_works);

	print("Second PS/2 port: ");
	print_yes_no(ps2_dual_channel);
}

#define GET_NIBBLE(val, nibble) (val & (0xF << (nibble * 4))) >> (nibble * 4)
void print_hex(int val) {
	char hexChars[] = "0123456789ABCDEF";
	
	char buffer[11];
	buffer[0] = '0';
	buffer[1] = 'x';
	buffer[2] = hexChars[GET_NIBBLE(val, 7)];
	buffer[3] = hexChars[GET_NIBBLE(val, 6)];
	buffer[4] = hexChars[GET_NIBBLE(val, 5)];
	buffer[5] = hexChars[GET_NIBBLE(val, 4)];
	buffer[6] = hexChars[GET_NIBBLE(val, 3)];
	buffer[7] = hexChars[GET_NIBBLE(val, 2)];
	buffer[8] = hexChars[GET_NIBBLE(val, 1)];
	buffer[9] = hexChars[GET_NIBBLE(val, 0)];
	buffer[10] = 0;

	print(buffer);
}

void kernel_main() {
	component_t components[] = {
		{ "PIC", pic_install, component_after_stub },
		{ "IDT", idt_install, component_after_stub },
		{ "Exception handlers", exception_handlers_install, component_after_stub },
		{ "Heap", heap_install, component_after_stub },
		{ "Keyboard", keyboard_install, keyboard_after }
	};
	
	console_init();

	console_set_color(GREEN);
	print(NAME " v" VERSION "\n");
	console_set_color(LIGHTGRAY);

	for (unsigned int i = 0; i < sizeof(components)/sizeof(component_t); i++) {
		component_t_install(components[i]);
	}

	// Test keyboard driver
	while (1) {
		putchar(getch());
	}
}
