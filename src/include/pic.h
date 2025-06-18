// HumbleOS file: pic.h
// Purpose: Header file for PIC.

#include <ints.h>

#ifndef PIC_H
#define PIC_H

/// @brief Send a EOI to the PIC.
/// @param irq IRQ used to send to EOI.
void PIC_sendEOI(u8 irq);

/// @brief Remaps the PIC.
/// @param offset1 Vector offset for master PIC
/// @param offset2 Vector offset for slave PIC
void PIC_remap(int offset1, int offset2);

/// @brief Initalize the PIC.
void pic_init();

/// @brief Set a mask for a IRQ.
/// @param IRQline The IRQ to set.
void IRQ_set_mask(u8 IRQline);

/// @brief Clear a mask for a IRQ.
/// @param IRQline The IRQ to clear.
void IRQ_clear_mask(u8 IRQline);

#endif
