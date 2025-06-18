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
#include <multiboot_info.h>

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

	// List multiboot tags
	klog("main", "multiboot tags:");
	struct multiboot_tag *tag = multiboot_info->tags;
    while (tag->type != 0) {
		int type = tag->type;
		int size = tag->size;
		char buf[100];
		int i = 0;
		i += strcpy(buf, "type: ");
		i += num_to_str(type, buf + i, 10, 0) - 1;
		i += strcpy(buf + i, ", size: ");
		i += num_to_str(multiboot_info_get_tag_size(type), buf + i, 10, 0) - 1;

		klog("main", buf);
        tag = (struct multiboot_tag *)((uint8_t *)tag + ((size + 7) & ~7));
    }
}
