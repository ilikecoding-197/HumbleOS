#include <stdint.h>

#ifndef PORT_H
#define PORT_H
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait(void);
#endif
