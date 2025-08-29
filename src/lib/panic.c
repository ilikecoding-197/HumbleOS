// HumbleOS file: panic.c
// Purpose: C file for panic

#include <panic.h>
#include "../config.h"
#include <console.h>
#include <stdarg.h>
#include <pcspk.h>
#include <serial.h>

// Get a nibble from a value.
#define GET_NIBBLE(val, nibble) (val & (0xF << (nibble * 4))) >> (nibble * 4)

extern int panic_reg_eax; // Saved EAX
extern int panic_reg_ebx; // Saved EBX
extern int panic_reg_ecx; // Saved ECX
extern int panic_reg_edx; // Saved EDX
extern int panic_reg_esi; // Saved ESI
extern int panic_reg_edi; // Saved EDI
extern int panic_reg_esp; // Saved ESP
extern int panic_reg_ebp; // Saved EBP

/// @brief Print a hex value.
/// @param val Value to print.
void _panic_print_hex(int val) {
	char hexChars[] = "0123456789ABCDEF"; // Hex character
	
	char buffer[11]; // Buffer
	buffer[0] = '0';
	buffer[1] = 'x';
	buffer[2] = hexChars[GET_NIBBLE(val, 7)]; // 0xX0000000
	buffer[3] = hexChars[GET_NIBBLE(val, 6)]; // 0x0X000000
	buffer[4] = hexChars[GET_NIBBLE(val, 5)]; // 0x00X00000
	buffer[5] = hexChars[GET_NIBBLE(val, 4)]; // 0x000X0000
	buffer[6] = hexChars[GET_NIBBLE(val, 3)]; // 0x0000X000
	buffer[7] = hexChars[GET_NIBBLE(val, 2)]; // 0x00000X00
	buffer[8] = hexChars[GET_NIBBLE(val, 1)]; // 0x000000X0
	buffer[9] = hexChars[GET_NIBBLE(val, 0)]; // 0x0000000X
	buffer[10] = 0;

	print(buffer); // Print it
}

struct stack_frame {
    struct stack_frame *prev;
    void *ret_addr;
};

#ifdef DEBUG
struct symbol_entry {
    u32 addr;
    const char* name;
};

// Only include if file exists, otherwise use empty array
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
	// Header
	console_clear_screen();
	console_set_color(RED);
	print("!!! KERNEL PANIC !!!\n");
	serial_print("!!! KERNEL PANIC !!!\n");
	console_set_color(LIGHTGRAY);
	print("---REGS---\n");
	serial_print("---REGS---\n");
	
	// Regs
	char buffer[256];
	sprintf_(buffer, "EAX: 0x%08x EBX: 0x%08x ECX: 0x%08x EDX: 0x%08x\n", panic_reg_eax, panic_reg_ebx, panic_reg_ecx, panic_reg_edx);
	print(buffer);
	serial_print(buffer);
	sprintf_(buffer, "ESI: 0x%08x EDI: 0x%08x ESP: 0x%08x EBP: 0x%08x\n", panic_reg_esi, panic_reg_edi, panic_reg_esp, panic_reg_ebp);
	print(buffer);
	serial_print(buffer);
	console_handle_newline();
	serial_print("\n");

	// Footer
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

	__asm__ __volatile__ ("cli; hlt"); // Halt

	while (1) {} // Infinte loop to stop GCC from complaining (it doesnt notice the __asm__ line 
	             // halts the computer.)
}
