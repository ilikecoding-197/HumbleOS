#include <console.h>
#include <idt.h>
#include <stdint.h>

#define NULL 0

extern uint8_t irq;

char *exceptions[] = {
	"Division Error",
	"Debug",
	NULL,
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invaild Opcode",
	"Device Not Available",
	"Double Fault",
	NULL,
	"Invaild TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	NULL,
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Pointer Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	NULL
};

uint8_t exceptions_with_error_code[5] = {
	10,
	11,
	12,
	13,
	14,
};

const char *hexDigits = "0123456789ABCDEF";

void exception() {
	clear_screen();
	print("EXCEPTION!\nError: ");
	print(exceptions[irq]);

	for (int i = 0; i < 5; i++) {
		if (exceptions_with_error_code[i] == irq) {
			// Get error code
			uint32_t code;
				
			__asm__ __volatile__ ("pop %0"
				                      : "=a" (code) );
			
			char toPrint[5];
			toPrint[0] = hexDigits[(code & 0xF000) >> 24];
			toPrint[1] = hexDigits[(code & 0x0F00) >> 16];
			toPrint[2] = hexDigits[(code & 0x00F0) >> 8];
			toPrint[3] = hexDigits[code & 0x000F];
			toPrint[4] = 0;
			
			print("\nError code: ");
			print(toPrint);
			
			break;
		}
	}
	
	__asm__ __volatile__ ("cli; hlt");
}

void exception_handlers_init() {
	for (int i = 0; i < 32; i++) {
		if (exceptions[i] == NULL) continue;

		attach_interupt(i, exception);
	}
}
