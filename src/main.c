#include <console.h>
#include <stdbool.h>
#include "settings.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct Component {
	char *name;
	void (*run)(void);
} Component;

void test(void) {
	print(" Works!");
}

void Component_install(Component component) {
	print("Installing ");
	set_color(GREEN);
	print(component.name);
	set_color(LIGHTGRAY);
	component.run();
	print(" [ ");
	set_color(GREEN);
	print("OK");
	set_color(LIGHTGRAY);
	print(" ]\n");
}

void kernel_main(){
	Component components[1] = {
		{ "Test", test }
	};
	
	console_init();

	set_color(GREEN);
	print(NAME " " VERSION ", " BUILD " build\n");
	set_color(LIGHTGRAY);

	for (int i = 0; i < 1; i++) {
		Component_install(components[i]);
	}

	main_menu();
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
