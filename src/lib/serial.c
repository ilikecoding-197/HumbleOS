/*
    serial.c - serial code.

    Part of HumbleOS

    Copyright 2025 Thomas Shrader

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/



#include <serial.h>
#include <port.h>
#include <console.h>
#include <printf.h>

static int serial_is_init = 0;

void serial_init()
{
    klog("SERIAL", "initalizing serial...");
    outb(SERIAL_PORT + 1, 0x00); // disable all interrupts
    outb(SERIAL_PORT + 3, 0x80); // enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT + 0, 0x03); // set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT + 1, 0x00); //                  (hi byte)
    outb(SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7); // enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(SERIAL_PORT + 4, 0x1E); // set in loopback mode, test the serial chip
    outb(SERIAL_PORT + 0, 0xAE); // test serial chip (send byte 0xAE and check if serial returns same byte)

    // check if serial is faulty (i.e: not same byte as sent)
    if (inb(SERIAL_PORT + 0) != 0xAE)
    {
        klog("SERIAL", "faulty serial chip; just wont send anything to it");
        return;
    }

    // if serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(SERIAL_PORT + 4, 0x0F);
    serial_is_init = 1;
    klog("SERIAL", "serial is initalized!");

    // clear the serial console with ANSI
    serial_print("\033[H\033[2J");
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

    // convert \n to \r\n for proper terminal display
    if (c == '\n') {
        while (is_transmit_empty() == 0);
        outb(SERIAL_PORT, '\r');
    }
    
    while (is_transmit_empty() == 0);
    outb(SERIAL_PORT, c);
}

void serial_flush() {
    // wait for all data to be transmitted
    while ((inb(SERIAL_PORT + 5) & 0x40) == 0);
}

void serial_print(char* str) {
    for (int i = 0; str[i]; i++) {
        serial_print_char(str[i]);
    }
    serial_flush(); // Ensure it's sent before returning
}

void _serial_out_char(char character, void *arg) {
    serial_print_char(character);
}