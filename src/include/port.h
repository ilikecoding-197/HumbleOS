// HumbleOS file: port.h
// Purpose: Header file for port commands

#include <stdint.h>

#ifndef PORT_H
#define PORT_H

/// @brief Output a byte to a port.
/// @param port The port to send the byte to.
/// @param value What to send.
void outb(uint16_t port, uint8_t value);

/// @brief Input a byte from a port.
/// @param port The port to get the byte from.
/// @return The byte gotten.
uint8_t inb(uint16_t port);

/// @brief Output a word to a port.
/// @param port The port to send the word to.
/// @param value What to send.
void outw(uint16_t port, uint16_t value);

/// @brief Input a word from a port.
/// @param port The port to get the word from.
/// @return The word gotten.
uint16_t inw(uint16_t port);

/// @brief Wait for I/O. (Write from 0x80)
void io_wait();

#endif
