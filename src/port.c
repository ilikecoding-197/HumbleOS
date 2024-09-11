#include <stdint.h>
#include <port.h>

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1"
                      :                  // No output
                      : "a"(value),      // Input: value in AL (lower 8 bits of EAX)
                        "d"(port));      // Input: port address in DX
}

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(result)    // Output: result stored in AL (lower 8 bits of EAX)
                      : "d"(port));     // Input: port address in DX
    return result;
}
