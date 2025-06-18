// HumbleOS file: port.c
// Purpose: Port code.

#include <ints.h>
#include <port.h>

void outb(u16 port, u8 value) {
    // Write
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(value),
                        "d"(port));
}

u8 inb(u16 port) {
    // Read

    u8 result; // Result
    __asm__ volatile ("inb %1, %0"
                      : "=a"(result)
                      : "d"(port));
    return result;
}

void outw(u16 port, u16 value) {
    // Write
    __asm__ volatile ("outw %0, %1"
                      :
                      : "a"(value),
                        "d"(port));
}

u16 inw(u16 port) {
    // Read

    u16 result; // Result
    __asm__ volatile ("inw %1, %0"
                      : "=a"(result)
                      : "d"(port));
    return result;
}

void io_wait()
{
    outb(0x80, 0); // Write to 0x80
}
