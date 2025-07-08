// HumbleOS file: main.c
// Purpose: The kernel code!

#include <console.h>
#include <idt.h>
#include <pic.h>
#include <exception_handlers.h>
#include <ps2/ps2_controller.h>
#include <stdbool.h>
#include <panic.h>
#include "settings.h"
#include <heap.h>
#include <time.h>
#include <multiboot.h>
#include <sys_info.h>
#include <ints.h>
#include <rand.h>

void kernel_main(multiboot_info_t* mbd, uint magic) {
	console_init();
	klog("main", NAME " v" VERSION);

	// Check for correct magic
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("Invalid magic number! Expecting 0x%x, got 0x%x.", MULTIBOOT_BOOTLOADER_MAGIC, magic);
	}

	// Initalize stuff
	pic_init();
	idt_init();
	exception_handlers_init();
	time_init();
	heap_init();
	ps2_controller_init();
	sys_info_gather(mbd);
	rand_init();

	// randomness test
	klogf("main", "random number: %d", rand());
	klogf("main", "random number from -100 to 100: %d", rand_in_range(-100, 100));
}
