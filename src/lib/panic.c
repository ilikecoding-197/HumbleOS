#include <panic.h>
#include <console.h>

#define GET_NIBBLE(val, nibble) (val & (0xF << (nibble * 4))) >> (nibble * 4)

extern int panic_reg_eax;
extern int panic_reg_ebx;
extern int panic_reg_ecx;
extern int panic_reg_edx;
extern int panic_reg_esi;
extern int panic_reg_edi;
extern int panic_reg_esp;
extern int panic_reg_ebp;

void _panic_print_hex(int val) {
	char hexChars[] = "0123456789ABCDEF";
	
	char buffer[11];
	buffer[0] = '0';
	buffer[1] = 'x';
	buffer[2] = hexChars[GET_NIBBLE(val, 7)];
	buffer[3] = hexChars[GET_NIBBLE(val, 6)];
	buffer[4] = hexChars[GET_NIBBLE(val, 5)];
	buffer[5] = hexChars[GET_NIBBLE(val, 4)];
	buffer[6] = hexChars[GET_NIBBLE(val, 3)];
	buffer[7] = hexChars[GET_NIBBLE(val, 2)];
	buffer[8] = hexChars[GET_NIBBLE(val, 1)];
	buffer[9] = hexChars[GET_NIBBLE(val, 0)];
	buffer[10] = 0;

	print(buffer);
}

void panic_panic(char *msg) {
	console_set_color(RED);
	print("!!! KERNEL PANIC !!!\n");
	console_set_color(LIGHTGRAY);
	print("---REGS---\n");
	
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

	print("Reason: ");
	print(msg);
	print("\nSystem will now ");
	console_set_color(RED);
	print("halt.");

	__asm__ __volatile__ ("cli; hlt");
}
