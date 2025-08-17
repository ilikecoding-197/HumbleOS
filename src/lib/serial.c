// HumbleOS file: serial.c
// Purpose: Serial code

#include <serial.h>
#include <port.h>
#include <console.h>
#include <printf.h>

static int serial_is_init = 0;

void serial_init()
{
    klog("SERIAL", "initalizing serial...");
    outb(SERIAL_PORT + 1, 0x00); // Disable all interrupts
    outb(SERIAL_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT + 1, 0x00); //                  (hi byte)
    outb(SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(SERIAL_PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(SERIAL_PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(SERIAL_PORT + 0) != 0xAE)
    {
        klog("SERIAL", "faulty serial chip; just wont send anything to it");
        return;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(SERIAL_PORT + 4, 0x0F);
    serial_is_init = 1;
    klog("SERIAL", "serial is initalized!");
}

static int is_transmit_empty()
{
    return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_print_char(char c)
{
    if (!serial_is_init)
    {
        return;
    }

    // Convert \n to \r\n for proper terminal display
    if (c == '\n') {
        while (is_transmit_empty() == 0);
        outb(SERIAL_PORT, '\r');
    }
    
    while (is_transmit_empty() == 0);
    outb(SERIAL_PORT, c);
}

void serial_flush() {
    // Wait for all data to be transmitted
    while ((inb(SERIAL_PORT + 5) & 0x40) == 0);
}

void serial_print(char* str) {
    for (int i = 0; str[i]; i++) {
        serial_print_char(str[i]);
    }
    serial_flush(); // Ensure it's sent before returning
}

void serial_printf(char *format, ...)
{
    if (!serial_is_init)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    serial_vprintf(format, args);
    va_end(args);
}

static void serial_out_char(char character, void *arg) {
    serial_print_char(character);
}

void serial_vprintf(char *format, va_list args)
{
    if (!serial_is_init)
    {
        return;
    }

    fctprintf(serial_out_char, NULL, format, args);
}
