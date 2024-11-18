#include <console.h>
#include <idt.h>
#include <pic.h>
#include <exception_handlers.h>
#include <keyboard.h>
#include <stdbool.h>
#include <panic.h>
#include "settings.h"

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
		console_clear_screen();

		console_move_cursor(1, 1);
		print("HumbleOS Main Menu");
		console_move_cursor(1, 2);
		print("------------------------------------------------------------------------------");
		console_move_cursor(1, VGA_HEIGHT-3);
		print("------------------------------------------------------------------------------");

		char status_bar[VGA_WIDTH-1];

		for (int i = 0; i < VGA_WIDTH-2; i++) status_bar[i] = ' ';

		strcpy(status_bar, " HumbleOS | Use arrows keys to move, enter to select.");
		
		console_move_cursor(1, VGA_HEIGHT-2);
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
	if (keyboard_first_ps2_works) {
		console_set_color(GREEN);
		print("Yes\n");
		console_set_color(LIGHTGRAY);
	} else {
		console_set_color(RED);
		print("No\n");
		console_set_color(LIGHTGRAY);
	}

	print("Second PS/2 port: ");
	if (keyboard_dual_channel) {
		console_set_color(GREEN);
		print("Yes\n");
		console_set_color(LIGHTGRAY);
	} else {
		console_set_color(RED);
		print("No\n");
		console_set_color(LIGHTGRAY);
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

	console_set_color(GREEN);
	print(NAME " " VERSION ", " BUILD " build\n");
	console_set_color(LIGHTGRAY);

	for (int i = 0; i < COMPONENT_AMT; i++) {
		component_t_install(components[i]);
	}
}
