#include <stdint.h>

#ifndef IDT_H
#define IDT_H
void idt_init();
typedef void (*interupt_function)();
void attach_interupt(uint8_t irq, interupt_function function);
#endif
