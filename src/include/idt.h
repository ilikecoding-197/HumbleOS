/// HumbleOS file: idt.h
/// Purpose: Header file for idt

#include <ints.h>

#ifndef IDT_H
#define IDT_H

/// @brief Initalize IDT.
void idt_init();

/// @brief Typedef for a interrupt handler.
typedef void (*interrupt_function)();

/// @brief Attach an interrupt.
/// @param irq The IRQ to attach to,
/// @param function Function to attach.
void attach_interrupt(u8 irq, interrupt_function function);

#endif
