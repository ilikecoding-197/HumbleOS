// HumbleOS file: port.c
// Purpose: Port code.

#include <stdint.h>
#include <port.h>

void outb(uint16_t port, uint8_t value) {
    // Write
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(value),
                        "d"(port));
}

uint8_t inb(uint16_t port) {
    // Read

    uint8_t result; // Result
    __asm__ volatile ("inb %1, %0"
                      : "=a"(result)
                      : "d"(port));
    return result;
}

void outw(uint16_t port, uint16_t value) {
    // Write
    __asm__ volatile ("outw %0, %1"
                      :
                      : "a"(value),
                        "d"(port));
}

uint16_t inw(uint16_t port) {
    // Read

    uint16_t result; // Result
    __asm__ volatile ("inw %1, %0"
                      : "=a"(result)
                      : "d"(port));
    return result;
}

void io_wait()
{
    outb(0x80, 0); // Write to 0x80
}
