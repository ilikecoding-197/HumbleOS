#include <keyboard.h>
#include <stdint.h>
#include <port.h>
#include <idt.h>

#define DATA_PORT 0x60
#define STATUS_OR_COMMAND_REGISTER 0x64

#define READ_BYTE_0 0x20
#define WRITE_BYTE_0 0x60

#define DISABLE_SECOND_PS2 0xA7
#define ENABLE_SECOND_PS2 0xA8
#define TEST_SECOND_PS2 0xA9
#define WRITE_SECOND_PS2_FAKE 0xD3
#define WRITE_SECOND_PS2_REAL 0xD4

#define TEST_PS2_PASSED 0x00
#define TEST_PS2_CLOCK_LINE_STUCK_LOW 0x01
#define TEST_PS2_CLOCK_LINE_STUCK_HIGH 0x02
#define TEST_PS2_DATA_LINE_STUCK_LOW 0x03
#define TEST_PS2_DATA_LINE_STUCK_HIGH 0x04 

#define DISABLE_FIRST_PS2 0xAD
#define ENABLE_FIRST_PS2 0xAE
#define TEST_FIRST_PS2 0xAB
#define WRITE_FIRST_PS2 0xD2

#define READ_CONTROLLER_INPUT_PORT 0xC0
#define COPY_0_TO_3_OF_INPUT_PORT_TO_STATUS_4_TO_7 0xC1
#define COPY_4_TO_7_OF_INPUT_PORT_TO_STATUS_4_TO_7 0xC2

#define READ_CONTROLLER_OUTPUT_PORT 0xD0
#define WRITE_CONTROLLER_OUTPUT_PORT 0xD1

#define SELF_TEST 0xAA
#define TEST_PASSED 0x55

#define RESET 0xFF

#include <console.h>
#include <stdbool.h>

bool keyboard_dual_channel;
bool keyboard_first_ps2_works;

uint8_t keyboard_get_status() {
	return inb(STATUS_OR_COMMAND_REGISTER);
}

void keyboard_send_data(uint8_t data, uint8_t port) {
	if (port == 0) {
		while (1) {
			uint8_t status = keyboard_get_status(); // Get status
						
			if (((status & 0b00000010) == 0) && status != 0) { // If we can send data
				break;
			}  
		}
				
		outb(DATA_PORT, data); // Send the command
	} else {
		if (keyboard_dual_channel) {
			keyboard_send_data(WRITE_SECOND_PS2_REAL, 0); // Tell the keyboard controller the send the next command to the 2nd port
			keyboard_send_data(data, 0); // Send the command
		}
	}
}

uint8_t keyboard_recieve_data() {
	while (1) {
		uint8_t status = keyboard_get_status(); // Get status
				
		if ((status & 0b00000001) == 1) { // If we can recieve data
			break;
		}  
	}
			
	return inb(DATA_PORT);
}

uint8_t keyboard_send_command(uint8_t cmd, int output, int hasArg, uint8_t arg, uint8_t port) {
	outb(STATUS_OR_COMMAND_REGISTER, cmd); // Send command

	if (hasArg) {
		keyboard_send_data(arg, port); // Send argument
	}

	if (output) {
		return keyboard_recieve_data(); // Recieve output
	}

	return 0;
}

void keyboard_print_hex(uint8_t value) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[5];

    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[2] = hex_chars[value >> 4];
    buffer[3] = hex_chars[value & 0x0F];
    buffer[4] = '\0';

    print(buffer);
}

void keyboard_interrupt() {
	uint8_t data = inb(DATA_PORT); // Get data

	keyboard_print_hex(data); // Print it in hex
}

int keyboard_init() {
	// Disable PS2
	keyboard_send_command(DISABLE_SECOND_PS2, 0, 0, 0, 0);
	keyboard_send_command(DISABLE_FIRST_PS2, 0, 0, 0, 0);

	inb(DATA_PORT);

	// Set config byte
	uint8_t config_byte = keyboard_send_command(READ_BYTE_0, 1, 0, 0, 0);
	config_byte &= 0b10101110;
	keyboard_send_command(WRITE_BYTE_0, 0, 1, config_byte, 0);

	// Test
	uint8_t test = keyboard_send_command(SELF_TEST, 1, 0, 0, 0);
	if (test != TEST_PASSED) return KEYBOARD_INIT_SELFTEST_FAIL;

	// Check for dual channel
	keyboard_send_command(ENABLE_SECOND_PS2, 0, 0, 0, 0);
	
	config_byte = keyboard_send_command(READ_BYTE_0, 1, 0, 0, 0);
	if (config_byte & 0x00100000) {
		keyboard_dual_channel = 0;
	} else {
		keyboard_send_command(DISABLE_SECOND_PS2, 0, 0, 0, 0);	
		config_byte &= 0b11011101;
		keyboard_dual_channel = 1;
	}

	// Test first PS2
	test = keyboard_send_command(TEST_FIRST_PS2, 1, 0, 0, 0);
	if (test != 0 ) {
		if (!keyboard_dual_channel) return KEYBOARD_INIT_NO_PORTS_LEFT;
	} else {
		keyboard_first_ps2_works = 1;
	}

	// Test second PS2
	test = keyboard_send_command(TEST_SECOND_PS2, 1, 0, 0, 0);
	if (test != 0 && keyboard_first_ps2_works == 0) return KEYBOARD_INIT_NO_PORTS_LEFT;

	if (keyboard_first_ps2_works) {
		keyboard_send_command(ENABLE_FIRST_PS2, 0, 0, 0, 0);
	}

	if (keyboard_dual_channel) {
		keyboard_send_command(ENABLE_SECOND_PS2, 0, 0, 0, 0);
		keyboard_send_command(RESET, 0, 0, 0, 1);
	}

	// Reset
	keyboard_send_command(RESET, 0, 0, 0, 0);

	attach_interrupt(0x21, keyboard_interrupt);
	return KEYBOARD_INIT_SUCCESS;
}
