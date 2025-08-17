// HumbleOS file: serial.h
// Purpose: Serial header

#ifndef SERIAL_H
#define SERIAL_H

#include <stdarg.h>

#define SERIAL_PORT 0x3F8 // COM1

void serial_init();
void serial_print_char(char c);
void serial_print(char *str);
void serial_printf(char *format, ...);
void serial_vprintf(char *format, va_list args);

#endif
