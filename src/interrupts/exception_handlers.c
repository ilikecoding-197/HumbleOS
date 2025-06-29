#include <console.h>
#include <idt.h>
#include <stddef.h>
#include <ints.h>

extern u8 irq; // IRQ from IDT assembly

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

u8 exceptions_with_error_code[5] = {
	10,
	11,
	12,
	13,
	14,
};

const char *hexDigits = "0123456789ABCDEF";

void exception() {
	print("EXCEPTION!\nError: ");
	print(exceptions[irq]);

	for (int i = 0; i < 5; i++) {
		if (exceptions_with_error_code[i] == irq) {
			// Get error code
			u32 code;
				
			__asm__ __volatile__ ("pop %0"
				                      : "=a" (code) ); // Error code is from stack
			
			char toPrint[9];
			u32 and_value = 0xF0000000;
			int shift_value = 4*7;
			for (int i = 0; i < 8; i++) {
				toPrint[i] = hexDigits[(code & and_value) >> shift_value];
			}
			toPrint[8] = 0;
			
			print("\nError code: ");
			print(toPrint);
			
			break;
		}
	}
	
	__asm__ __volatile__ ("cli; hlt");
}

void exception_handlers_init() {
	klog("EXCEPT", "initializing...");
	for (int i = 0; i < 32; i++) {
		if (exceptions[i] == NULL) continue;

		attach_interrupt(i, exception);
	}
	klog("EXCEPT", "done");
}
