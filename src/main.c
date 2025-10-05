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
#include <vga.h>
#include <humbleoslogo.h>
#include <string.h>
#include <events.h>
#include <build_info.h>
#include <block/ramdisk.h>

// Global constructors for C++
extern void (*__init_array_start)(void);
extern void (*__init_array_end)(void);

void call_global_constructors()
{
	for (void (**ctor)(void) = &__init_array_start; ctor < &__init_array_end; ++ctor)
	{
		klogf("main", "calling global constructor at %p", *ctor);
		(*ctor)(); // Call each constructor
	}
}

void user_main();

ps2_device ps2_devices[2];

#if LOADING_SCREEN
static void draw_logo(uint x, uint y)
{
	for (int i = 0; i < 5; i++)
	{
		char *line = HUMBLE_OS_BANNER[i];

		console_move_cursor(x, y + i);
		print(line);
	}
}

static void draw_time(uint right, uint y)
{
	char timeStr[25];

	sprintf(timeStr, "%dms", time_ms);

	uint len = strlen(timeStr);
	uint x = right - len;

	console_move_cursor(x, y);
	print(timeStr);
}

static void draw_loading_screen(uint x, uint y)
{
	console_set_color(VGA_ATTR(WHITE, LIGHTBLUE));
	for (int ly = 0; ly < 9; ly++)
	{
		for (int lx = 0; lx < 50; lx++)
		{
			put_color_at(lx + x - 1, ly + y - 1, VGA_ATTR(WHITE, LIGHTBLUE));
		}
	}

	draw_logo(x, y);

	console_move_cursor(x, y + 6);
	print("Loading...");
	draw_time(x + 48, y + 6);
}

#define LOADING_SCREEN_X 15
#define LOADING_SCREEN_Y 8

static void loading_screen_time_event(Event *e)
{
	if (e->type != EVENT_TIMER)
		return;

	console_set_color(VGA_ATTR(WHITE, LIGHTBLUE));
	draw_time(LOADING_SCREEN_X + 48, LOADING_SCREEN_Y + 6);
}
#endif // LOADING_SCREEN

void kernel_main(multiboot_info_t *mbd, uint magic)
{
	console_init();
	serial_init(); // do this as early as possible

	vga_set_color_palette16(vga_ubuntu_palette);

#if LOADING_SCREEN
	console_hide_cursor();
	console_set_color(VGA_ATTR(WHITE, LIGHTBLUE));
	draw_loading_screen(LOADING_SCREEN_X, LOADING_SCREEN_Y);
	klog_to_serial_only = 1;
#endif // LOADING_SCREEN

#if BUILD_NAME_IS_EMPTY
	klog("main", NAME " v" VERSION ", " RELEASE_TYPE " build");
#else
	klog("main", NAME " (" BUILD_NAME ") v" VERSION ", " RELEASE_TYPE " build");
#endif

	klog("main", "compiled with: " COMPILER_HUMAN_READABLE);

	// Check for correct magic
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		PANIC("Invalid magic number! Expecting 0x%x, got 0x%x.", MULTIBOOT_BOOTLOADER_MAGIC, magic);
	}

	// Initalize stuff
	pic_init();
	idt_init();
	exception_handlers_init();
	time_init();

#if LOADING_SCREEN
	// register time update
	uint loading_screen_time_event_id = add_event(loading_screen_time_event, NULL);
#endif // LOADING_SCREEN

	ps2_controller_init();
	ps2_detect_devices(ps2_devices, 2);
	ps2_keyboard_init();
	sys_info_gather(mbd);
	heap_init();
	rand_init();

	klog("main", "calling global constructors for C++ support");
	call_global_constructors();

	klog("main", "initialization complete");

	sys_info.boot_time = time_ms;

#if LOADING_SCREEN
	remove_event(loading_screen_time_event_id);
	console_show_cursor();
	console_set_color(WHITE);
	console_clear_screen();
#endif

#if BEEP_ON_INIT_DONE
	beep();
#endif

#if USE_USER_MAIN
	klog_to_serial_only = 1;
	user_main();
#endif
}