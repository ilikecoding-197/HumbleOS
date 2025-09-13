/*
	pic.c - pic code

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

// not going to explain this code, as I just copied (most) of it from OSDev. sorry :(

#include <port.h>
#include <ints.h>
#include <console.h>

#define PIC1			0x20
#define PIC2			0xA0
#define PIC1_COMMAND	PIC1
#define PIC1_DATA		(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		(PIC2+1)
#define PIC_EOI		0x20
#define PIC_OFFSET1     0x20
#define PIC_OFFSET2     0x28

#define ICW1_ICW4	0x01
#define ICW1_SINGLE	0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL	0x08
#define ICW1_INIT	0x10

#define ICW4_8086	0x01
#define ICW4_AUTO	0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	0x10

#define PIC_READ_IRR                0x0a
#define PIC_READ_ISR                0x0b

void PIC_sendEOI(u8 irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
	
	outb(PIC1_COMMAND,PIC_EOI);
}

void PIC_remap(int offset1, int offset2)
{
	u8 a1, a2;
	
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);
	
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
	
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void pic_init() {
	klog("PIC", "initalizing...");
	PIC_remap(PIC_OFFSET1, PIC_OFFSET2);
	klog("PIC", "done");
}

void IRQ_set_mask(u8 IRQline) {
    u16 port;
    u8 value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}

void IRQ_clear_mask(u8 IRQline) {
    u16 port;
    u8 value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}