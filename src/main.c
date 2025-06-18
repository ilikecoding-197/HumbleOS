// HumbleOS file: main.c
// Purpose: The kernel code!

#include <console.h>
#include <idt.h>
#include <pic.h>
#include <exception_handlers.h>
#include <ps2_controller.h>
#include <stdbool.h>
#include <panic.h>
#include "settings.h"
#include <heap.h>
#include <time.h>

void kernel_main() {
	console_init();
	klog("main", NAME " v" VERSION);

	// Initalize stuff
	klog("main", "initalizing everything");
	pic_init();
	idt_init();
	exception_handlers_init();
	time_init();
	heap_init();
	ps2_controller_init();
	klog("main", "done!");
}
