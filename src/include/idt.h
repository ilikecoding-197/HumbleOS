#include <stdint.h>

#ifndef IDT_H
#define IDT_H
void idt_init();
typedef void (*interrupt_function)();
void attach_interrupt(uint8_t irq, interrupt_function function);
#endif
