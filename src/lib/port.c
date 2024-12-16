#include <stdint.h>
#include <port.h>

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(value),
                        "d"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(result)
                      : "d"(port));
    return result;
}

void outw(uint16_t port, uint16_t value) {
    __asm__ volatile ("outw %0, %1"
                      :
                      : "a"(value),
                        "d"(port));
}

uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ volatile ("inw %1, %0"
                      : "=a"(result)
                      : "d"(port));
    return result;
}

void io_wait(void)
{
    outb(0x80, 0);
}
