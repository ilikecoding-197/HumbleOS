/*
    idt.c - C code for the IDT

    Part of HumbleOS

    Copyright 2025 Thomas Shrader

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <ints.h>
#include <stdbool.h>
#include <console.h>
#include <port.h>

// an IDT entry
typedef struct
{
    u16 isr_low;
    u16 kernel_cs;
    u8 reserved;
    u8 attributes;
    u16 isr_high;
} __attribute__((packed)) idt_entry_t;

// IDT entries
__attribute__((aligned(0x10))) static idt_entry_t idt[256];

// IDT pointer
typedef struct
{
    u16 limit;
    u32 base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;
extern u8 irq; // IRQ from assembly

void interrupt_stub() {}   // empty interrupt handler
void (*interrupts[256])(); // interrupt handlers

void interrupt_handler()
{
    interrupts[irq](); // run the handler for that interrupt
}

void idt_set_descriptor(u8 vector, void *isr, u8 flags)
{
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low = (u32)isr & 0xFFFF; // Low byte of ISR
    descriptor->kernel_cs = 0x08;            // Kernel code segment
    descriptor->attributes = flags;
    descriptor->isr_high = (u32)isr >> 16; // High byte of ISR
    descriptor->reserved = 0;
}

#define IDT_MAX_DESCRIPTORS 256
extern void *interrupt_stub_table[];

void attach_interrupt(u8 irqNum, void (*function)())
{
    interrupts[irqNum] = function;
}

void idt_init()
{
    klog("IDT", "initializing...");

    klog("IDT", "setting stubs...");
    for (int i = 0; i < 256; i++)
    {
        interrupts[i] = interrupt_stub;
    }

    klog("IDT", "setting IDTR...");
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (u16)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (u8 vector = 0; vector < 48; vector++)
    {
        idt_set_descriptor(vector, interrupt_stub_table[vector], 0x8E); // Set the vector
    }

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");

    klog("IDT", "done");
}
