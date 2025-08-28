// HumbleOS file: main.c
// Purpose: The kernel code!

#include <console.h>
#include <idt.h>
#include <pic.h>
#include <exception_handlers.h>
#include <ps2/ps2_controller.h>
#include <stdbool.h>
#include <panic.h>
#include "config.h"
#include "info.h"
#include <heap.h>
#include <time.h>
#include <multiboot.h>
#include <sys_info.h>
#include <ints.h>
#include <rand.h>
#include <serial.h>
#include <ps2/ps2.h>
#include <ps2/ps2_keyboard.h>
#include <pcspk.h>

// Global constructors for C++
extern void (*__init_array_start)(void);
extern void (*__init_array_end)(void);

void call_global_constructors() {
	for (void (**ctor)(void) = &__init_array_start; ctor < &__init_array_end; ++ctor) {
		klogf("main", "calling global constructor at %p", *ctor);
		(*ctor)(); // Call each constructor
	}
}

void user_main();

ps2_device ps2_devices[2];

void kernel_main(multiboot_info_t* mbd, uint magic) {
	console_init();
	serial_init(); // do this as early as possible

#if BUILD_NAME_IS_EMPTY
	klog("main", NAME " v" VERSION ", " RELEASE_TYPE " build");
#else
	klog("main", NAME " (" BUILD_NAME ") v" VERSION ", " RELEASE_TYPE " build");
#endif

	// Check for correct magic
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("Invalid magic number! Expecting 0x%x, got 0x%x.", MULTIBOOT_BOOTLOADER_MAGIC, magic);
	}

	// Initalize stuff
	pic_init();
	idt_init();
	exception_handlers_init();
	time_init();
	ps2_controller_init();
	ps2_detect_devices(ps2_devices, 2);
	ps2_keyboard_init();
	sys_info_gather(mbd);
	heap_init();
	rand_init();

	klog("main", "calling global constructors for C++ support");
	call_global_constructors();

	klog("main", "initialization complete");

	#if BEEP_ON_INIT_DONE
	beep();
	#endif

	#if USE_USER_MAIN
	klog_to_serial_only = 1;
	user_main();
	#endif
}
