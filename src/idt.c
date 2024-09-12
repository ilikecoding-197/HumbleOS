#include <stdint.h>
#include <stdbool.h>
#include <console.h>

typedef struct {
	uint16_t    isr_low;
	uint16_t    kernel_cs;
	uint8_t     reserved;
	uint8_t     attributes;
	uint16_t    isr_high;
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;
extern uint8_t irq;

//__attribute__((noreturn))
void exception_handler() {
	const char *errors[] = {
		"Divison Error",
		"NOT ERR",
		"NOT ERR",
		"NOT ERR",
		"Overflow",
		"Bound Range Exceeded",
		"Invaild Opcode",
		"Device Not Available",
		"Double Fault",
		"NOT ERR",
		"Invaild TSS",
		"Segment Not Present",
		"Stack-segment Fault",
		"General Protection Fault",
		"Page Fault",
		"NOT ERR",
		"x87 Floating-Point Exception",
		"Alignment Check",
		"Machine Check",
		"SIMD Floating-Point Exception",
		"Virtualiztion Exception",
		"Control Protection Exception",
		"NOT ERR",
		"NOT ERR",
		"NOT ERR",
		"NOT ERR",
		"NOT ERR",
		"NOT ERR",
		"Hypervisor Injection Exception",
		"VMM Communication Exception",
		"Security Exeption",
		"NOT ERR",
		"NOT ERR",
		"NOT ERR"
	};
	clear_screen();
	print("ERR!\nError: ");
	print(errors[irq]);
	
    __asm__ volatile ("cli; hlt");
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

#define IDT_MAX_DESCRIPTORS 256

static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti"); 
}
