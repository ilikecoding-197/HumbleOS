#include <keyboard.h>
#include <console.h>
#include <stdint.h>
#include <port.h>
#include <idt.h>
#include "controller_constants.h"

// Controller functions
#define keyboard_controller_get_status() inb(STATUS_OR_COMMAND_REGISTER)

int first_ps2_works;
int ps2_dual_channel;

/*
 * Sends data to the keyboard controller.
 *
 * Arguments:
 *   * data (uint8_t) - Data to send.
 *   * port (int)     - Port to send it to.
 */
void keyboard_controller_send_data(uint8_t data, int port) {
	if (port == 0) {
		while (1) { // Wait until the input buffer is empty
			if (!(keyboard_controller_get_status() & 0b00000010))
				break;
		}

		outb(DATA_PORT, data); // Write data
	} else {
		outb(STATUS_OR_COMMAND_REGISTER, WRITE_SECOND_PS2_REAL); // Tell the controller that we want to talk to the second port

		keyboard_controller_send_data(data, 0); // Send the data
	}
}

/*
 * Receives data from the keyboard controller.
 *
 */
uint8_t keyboard_controller_receive_data() {
	while (1) { // Wait until we can receive
		if (!(keyboard_controller_get_status() & 0b00000010))
			break;
	}
	
	return inb(DATA_PORT); // Receive
}

/*
 * Sends a command to the controller.
 *
 * Arguments:
 *   * command (uint8_t) - Command to send.
 *   * hasArg (int)      - If the command has a argument.
 *   * arg (uint8_t)     - The argument to send (if hasArg is 1.)
 *   * hasOutput (int)   - If the command has a output.
 *   * port (int)        - The port to send the command to.
 */
int keyboard_controller_send_command(uint8_t command, int hasArg, uint8_t arg, int hasOutput, int port) {
	outb(STATUS_OR_COMMAND_REGISTER, command); // Send command

	if (hasArg) {
		keyboard_controller_send_data(arg, port); // Send arg
	}

	if (hasOutput) {
		return keyboard_controller_receive_data();
	}

	return 0;
}

/*
 * Handler for keyboard interrupt.
 */
void keyboard_handler() {
	print("KBD INTERRUPT");
}

/*
 * Initalize keyboard.
 */
int keyboard_init() {
	// Step 1: Disable devices
	keyboard_controller_send_command(DISABLE_FIRST_PS2, 0, 0, 0, 0);
	keyboard_controller_send_command(DISABLE_SECOND_PS2, 0, 0, 0, 0);

	// Step 2: Flush output buffer
	inb(DATA_PORT);

	// Step 3: Set config byte
	uint8_t config_byte = keyboard_controller_send_command(READ_BYTE_0, 0, 0, 1, 0);
	config_byte &= 0b10101110;
	keyboard_controller_send_command(WRITE_BYTE_0, 1, config_byte, 1, 0);

	// Step 4: Perform self test
	if (keyboard_controller_send_command(SELF_TEST, 0, 0, 0, 0) == TEST_PASSED) {
		return KEYBOARD_INIT_SELFTEST_FAIL;
	}

	// Step 5: Are there 2 channels?
	keyboard_controller_send_command(ENABLE_SECOND_PS2, 0, 0, 0, 0);
	config_byte = keyboard_controller_send_command(READ_BYTE_0, 0, 0, 1, 0);
	if (config_byte & 0b00100000) {
		ps2_dual_channel = 0;

		keyboard_controller_send_command(DISABLE_SECOND_PS2, 0, 0, 0, 0);
		config_byte &= 0b11011101;
		keyboard_controller_send_command(WRITE_BYTE_0, 1, config_byte, 1, 0);
	}

	// Step 6: Test ports
	first_ps2_works = keyboard_controller_send_command(TEST_FIRST_PS2, 0, 0, 0, 0) == 0;
	ps2_dual_channel = ps2_dual_channel && keyboard_controller_send_command(TEST_SECOND_PS2, 0, 0, 0, 0) == 0;

	if (!first_ps2_works && !ps2_dual_channel) {
		return KEYBOARD_INIT_NO_PORTS_LEFT;
	}

	// Step 7: Enable ports
	if (first_ps2_works) keyboard_controller_send_command(ENABLE_FIRST_PS2, 0, 0, 0, 0);
	if (ps2_dual_channel) keyboard_controller_send_command(ENABLE_SECOND_PS2, 0, 0, 0, 0);

	config_byte = keyboard_controller_send_command(READ_BYTE_0, 0, 0, 1, 0);
	if (first_ps2_works) config_byte |= 0x00000001;
	if (ps2_dual_channel) config_byte |= 0x00000010;
	keyboard_controller_send_command(WRITE_BYTE_0, 1, config_byte, 1, 0);

	// Step 8: Reset Devices
	// I need the keyboard command sending and recieving for this...

	// Step 9: Attach interrupt
	attach_interrupt(0x21, keyboard_handler);

	return KEYBOARD_INIT_SUCCESS;
}
