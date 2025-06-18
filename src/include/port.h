// HumbleOS file: port.h
// Purpose: Header file for port commands

#include <ints.h>

#ifndef PORT_H
#define PORT_H

/// @brief Output a byte to a port.
/// @param port The port to send the byte to.
/// @param value What to send.
void outb(u16 port, u8 value);

/// @brief Input a byte from a port.
/// @param port The port to get the byte from.
/// @return The byte gotten.
u8 inb(u16 port);

/// @brief Output a word to a port.
/// @param port The port to send the word to.
/// @param value What to send.
void outw(u16 port, u16 value);

/// @brief Input a word from a port.
/// @param port The port to get the word from.
/// @return The word gotten.
u16 inw(u16 port);

/// @brief Wait for I/O. (Write from 0x80)
void io_wait();

#endif
