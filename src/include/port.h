// HumbleOS file: port.h
// Purpose: Header file for port commands

#include <ints.h>

#ifndef PORT_H
#define PORT_H

// output a byte to a port
static inline void outb(u16 port, u8 value)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(value),
                       "d"(port));
}

// input a byte from a port
static inline u8 inb(u16 port)
{
    u8 result;
    __asm__ volatile("inb %1, %0"
                     : "=a"(result)
                     : "d"(port));
    return result;
}

// output a word to a port
static inline void outw(u16 port, u16 value)
{
    __asm__ volatile("outw %0, %1"
                     :
                     : "a"(value),
                       "d"(port));
}

// input a word from a port
static inline u16 inw(u16 port)
{
    u16 result;
    __asm__ volatile("inw %1, %0"
                     : "=a"(result)
                     : "d"(port));
    return result;
}

// wait for I/O (write from 0x80)
static inline void io_wait()
{
    outb(0x80, 0);
}

#endif
