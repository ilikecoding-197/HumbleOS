/*
	panic.c - C code for panic

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
#include "../config.h"
#include <console.h>
#include <stdarg.h>
#include <pcspk.h>
#include <serial.h>

extern int panic_reg_eax;
extern int panic_reg_ebx;
extern int panic_reg_ecx;
extern int panic_reg_edx;
extern int panic_reg_esi;
extern int panic_reg_edi;
extern int panic_reg_esp;
extern int panic_reg_ebp;

struct stack_frame {
    struct stack_frame *prev;
    void *ret_addr;
};

#ifdef DEBUG
struct symbol_entry {
    u32 addr;
    const char* name;
};

// only include if file exists, otherwise use empty array
#if __has_include("../symbols_generated.h")
static struct symbol_entry symbols[] = {
#include "../symbols_generated.h"
};
#else
static struct symbol_entry symbols[] = {
    {0, ""}
};
#endif

const char* lookup_symbol(u32 addr) {
    const char* best_match = "unknown";
    u32 best_addr = 0;
    
    for (int i = 0; symbols[i].addr != 0; i++) {
        if (symbols[i].addr <= addr && symbols[i].addr > best_addr) {
            best_addr = symbols[i].addr;
            best_match = symbols[i].name;
        }
    }
    
    return best_match;
}
#endif

__attribute__((noreturn)) void panic_panic(char *msg, char *file, char *line, ...) {
	// header
	console_clear_screen();
	console_set_color(RED);
	print("!!! KERNEL PANIC !!!\n");
	serial_print("!!! KERNEL PANIC !!!\n");
	console_set_color(WHITE);
	print("---REGS---\n");
	serial_print("---REGS---\n");
	
	// regs
	char buffer[256];
	sprintf_(buffer, "EAX: 0x%08x EBX: 0x%08x ECX: 0x%08x EDX: 0x%08x\n", panic_reg_eax, panic_reg_ebx, panic_reg_ecx, panic_reg_edx);
	print(buffer);
	serial_print(buffer);
	sprintf_(buffer, "ESI: 0x%08x EDI: 0x%08x ESP: 0x%08x EBP: 0x%08x\n", panic_reg_esi, panic_reg_edi, panic_reg_esp, panic_reg_ebp);
	print(buffer);
	serial_print(buffer);
	console_handle_newline();
	serial_print("\n");

	// footer
	print("Reason: ");
	serial_print("Reason: ");

	va_list args;
	va_start(args, line);
	vprintf(msg, args);
	va_end(args);
	
	print("\nHappened in C file ");
	serial_print("Happened in C file ");
	print(file);
	serial_print(file);
	print(" at line ");
	serial_print(" at line ");
	print(line);
	serial_print(line);

	// stack trace
	print("\n\nStack trace: ");
	serial_print("\n\nStack trace: ");
	struct stack_frame *frame = (struct stack_frame *)panic_reg_ebp;

	while (frame) {
		sprintf_(buffer, "\n  0x%08x", frame->ret_addr);
		print(buffer);
		serial_print(buffer);
		#ifdef DEBUG
		sprintf_(buffer, " (%s)", lookup_symbol((u32)frame->ret_addr));
		print(buffer);
		serial_print(buffer);
		#endif
		frame = frame->prev;
	}

	print("\nSystem will now ");
	serial_print("\nSystem will now ");
	console_set_color(RED);
	print("halt.");
	serial_print("halt.");

	#if BEEP_ON_PANIC
	beep();
	#endif

	__asm__ __volatile__ ("cli; hlt");

	while (1) {} // infinte loop to stop GCC from complaining (it doesnt notice the __asm__ line 
	             // halts the computer.)
}
