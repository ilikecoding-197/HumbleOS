// HumbleOS file: idt.c
// Purpose: C code for the IDT

#include <ints.h>
#include <stdbool.h>
#include <console.h>
#include <port.h>

// An IDT entry
typedef struct {
	u16    isr_low;    // Low word of ISR
	u16    kernel_cs;  // Code segment for kernel
	u8     reserved;   // Reserved byte
	u8     attributes;
	u16    isr_high;   // High byte of ISR
} __attribute__((packed)) idt_entry_t;

// IDT entries
__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];

// IDT pointer
typedef struct {
	u16	limit;
	u32	base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;
extern u8 irq;       // IRQ from assembly

void interrupt_stub() {}   // Empty interrupt handler
void (*interrupts[256])(); // Interrupt handlers

void interrupt_handler() {
	interrupts[irq]();     // Run the handler for that interrupt
}

void idt_set_descriptor(u8 vector, void* isr, u8 flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (u32)isr & 0xFFFF; // Low byte of ISR
    descriptor->kernel_cs      = 0x08;                   // Kernel code segment
    descriptor->attributes     = flags;                  
    descriptor->isr_high       = (u32)isr >> 16;    // High byte of ISR
    descriptor->reserved       = 0;
}

#define IDT_MAX_DESCRIPTORS 256 
extern void* interrupt_stub_table[];

void attach_interrupt(u8 irqNum, void (*function)()) {
	interrupts[irqNum] = function;
}

void idt_init() {
    klog("IDT", "initializing...");

    klog("IDT", "setting stubs...");
	for (int i = 0; i < 256; i++) {
		interrupts[i] = interrupt_stub; // Fill the interupt list with stubs
	}

	// Set up IDTR
    klog("IDT", "setting IDTR...");
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (u16)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (u8 vector = 0; vector < 48; vector++) {
        idt_set_descriptor(vector, interrupt_stub_table[vector], 0x8E); // Set the vector
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // Load IDT
    __asm__ volatile ("sti");                   // Enable interupts

    klog("IDT", "done");
}
