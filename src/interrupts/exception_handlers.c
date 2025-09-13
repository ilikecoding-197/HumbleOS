/*
	exception_handlers.c - exception handlers code

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

#include <panic.h>
#include <idt.h>
#include <stddef.h>
#include <stdbool.h>
#include <ints.h>

extern u8 irq; // IRQ from IDT assembly
extern u32 _error_code;

char *exceptions[] = {
	"Division Error",
	"Debug",
	NULL,
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invaild Opcode",
	"Device Not Available",
	"Double Fault",
	NULL,
	"Invaild TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	NULL,
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Pointer Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	NULL};

u8 exceptions_with_error_code[5] = {
	10,
	11,
	12,
	13,
	14,
};

static const char *hexDigits = "0123456789ABCDEF";

void exception()
{
	bool doErrorCode = false;
	for (int i = 0; i < 5; i++)
	{
		if (exceptions_with_error_code[i] == irq)
		{
			doErrorCode = true;
			break;
		}
	}

	if (doErrorCode)
	{
		PANIC("exception: %s (error code: %d)", exceptions[irq], _error_code);
	} else {
		PANIC("exception: %s", exceptions[irq]);
	}
}

void exception_handlers_init()
{
	klog("EXCEPT", "initializing...");
	for (int i = 0; i < 32; i++)
	{
		if (exceptions[i] == NULL)
			continue;

		attach_interrupt(i, exception);
	}
	klog("EXCEPT", "done");
}
