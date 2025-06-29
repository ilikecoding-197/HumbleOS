// HumbleOS file: panic.c
// Purpose: C file for panic

#include <panic.h>
#include <console.h>
#include <stdarg.h>

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

__attribute__((noreturn)) void panic_panic(char *msg, char *file, char *line, ...) {
	// Header
	console_clear_screen();
	console_set_color(RED);
	print("!!! KERNEL PANIC !!!\n");
	console_set_color(LIGHTGRAY);
	print("---REGS---\n");
	
	// Regs
	print("EAX: ");
	_panic_print_hex(panic_reg_eax);
	print(" EBX: ");
	_panic_print_hex(panic_reg_ebx);
	print(" ECX: ");
	_panic_print_hex(panic_reg_ecx);
	print(" EDX: ");
	_panic_print_hex(panic_reg_edx);
	console_handle_newline();
	
	print("ESI: ");
	_panic_print_hex(panic_reg_esi);
	print(" EDI: ");
	_panic_print_hex(panic_reg_edi);
	print(" ESP: ");
	_panic_print_hex(panic_reg_esp);
	print(" EBP: ");
	_panic_print_hex(panic_reg_ebp);
	console_handle_newline();

	// Footer
	print("Reason: ");

	va_list args;
	va_start(args, line);
	vprintf(msg, args);
	va_end(args);
	
	print("\nHappened in C file ");
	print(file);
	print(" at line ");
	print(line);
	print(".\nSystem will now ");
	console_set_color(RED);
	print("halt.");

	__asm__ __volatile__ ("cli; hlt"); // Halt

	while (1) {} // Infinte loop to stop GCC from complaining (it doesnt notice the __asm__ line 
	             // halts the computer.)
}
