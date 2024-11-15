#include <console.h>
#include <idt.h>
#include <pic.h>
#include <exception_handlers.h>
#include <keyboard.h>
#include <stdbool.h>
#include "settings.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct component_t {
	char *name;
	bool (*run)(void);
	void (*after)(void);
} component_t;

void test(void) {
	print(" Works!");
}

void component_t_install(component_t component) {
	print("Installing ");
	set_color(GREEN);
	print(component.name);
	set_color(LIGHTGRAY);
	
	bool result = component.run();
	print(" [ ");
	set_color(result ? GREEN : RED);
	print(result ? "OK" : "FAIL");
	set_color(LIGHTGRAY);
	print(" ]\n");

	component.after();
}

void strcpy(char *dest, char *src) {
	int i = 0;

	while (true) {
		if (src[i] == 0 || dest[i] == 0) break;

		dest[i] = src[i];

		i++;
	}
}

void main_menu() {
	while (true) {
		clear_screen();

		move_cursor(1, 1);
		print("HumbleOS Main Menu");
		move_cursor(1, 2);
		print("------------------------------------------------------------------------------");
		move_cursor(1, VGA_HEIGHT-3);
		print("------------------------------------------------------------------------------");

		char status_bar[VGA_WIDTH-1];

		for (int i = 0; i < VGA_WIDTH-2; i++) status_bar[i] = ' ';

		strcpy(status_bar, " HumbleOS | Use arrows keys to move, enter to select.");
		
		move_cursor(1, VGA_HEIGHT-2);
		print(status_bar);

		return;
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

int keyboard_install_error;

bool keyboard_install() {
	keyboard_install_error = keyboard_init();
	
	return keyboard_install_error == KEYBOARD_INIT_SUCCESS;
}

void component_after_stub() {}

void keyboard_after() {
	switch (keyboard_install_error) {
		case KEYBOARD_INIT_SELFTEST_FAIL:
			set_color(RED);
			print("Error: Selftest failed\n");
			set_color(LIGHTGRAY);
			break;
		case KEYBOARD_INIT_NO_PORTS_LEFT:
			set_color(RED);
			print("Error: No ports left\n");
			set_color(LIGHTGRAY);
			break;
	}
}

#define COMPONENT_AMT 4
void kernel_main(){
	component_t components[COMPONENT_AMT] = {
		{ "PIC", pic_install, component_after_stub },
		{ "IDT", idt_install, component_after_stub },
		{ "Exception handlers", exception_handlers_install, component_after_stub },
		{ "Keyboard", keyboard_install, keyboard_after }
	};
	
	console_init();

	set_color(GREEN);
	print(NAME " " VERSION ", " BUILD " build\n");
	set_color(LIGHTGRAY);

	for (int i = 0; i < COMPONENT_AMT; i++) {
		component_t_install(components[i]);
	}

	print("\nThe system has now booted!");

	while (1);
}

