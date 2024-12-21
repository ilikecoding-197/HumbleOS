// HumbleOS file: keyboard.c
// Purpose: Keyboard driver.

#include <keyboard.h>
#include <console.h>
#include <stdint.h>
#include <port.h>
#include <idt.h>

// Commands
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
#define SYS_RESET 0xFE

// Controller functions

/// @brief Get keyboard status.
#define keyboard_controller_get_status() inb(STATUS_OR_COMMAND_REGISTER)

int first_ps2_works;
int ps2_dual_channel;

/// @brief Sends data to the keyboard controller.
/// @param data Data to send.
/// @param port Port to send to
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

/// @brief Receives data from the keyboard controller.
/// @return Data received.
uint8_t keyboard_controller_receive_data() {
	while (1) { // Wait until we can receive
		if (!(keyboard_controller_get_status() & 0b00000010))
			break;
	}
	
	return inb(DATA_PORT); // Receive
}

/// @brief Send a command to the controller.
/// @param command The command to send.
/// @param hasArg If the command has a argument
/// @param arg If hasArg is true, argument to send.
/// @param hasOutput If the command outputs something.
/// @param port Port to send to.
/// @return If hasOutput is true, output. Else 0.
int keyboard_controller_send_command(uint8_t command, int hasArg, uint8_t arg, int hasOutput, int port) {
	outb(STATUS_OR_COMMAND_REGISTER, command); // Send command

	if (hasArg) {
		keyboard_controller_send_data(arg, port); // Send arg
	}

	if (hasOutput) {
		return keyboard_controller_receive_data(); // Return output
	}

	return 0; // Else, return 0.
}

// ACSII for each scancode
const char KEYBOARD_SCANCODE_TO_ACSII[] = 
	"\0\0\0\0\0\0\0\0\0\0\0\0\0\t`\0\0\0\0\0\0q1\0\0\0"
	"zsaw2\0\0cxde43\0\0 vftr5\0\0nbhgy6\0\0\0mju78\0\0,"
	"kio09\0\0./l;p-\0\0\0'\0[=\0\0\0\0\n]\0\\\0\0\0\0"
	"\0\0\0\0\b\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0";
const char KEYBOARD_SHIFTED_KEY[]   = "1234567890/.,\\';][=-]`"; // Shifted key
const char KEYBOARD_SHIFTED_VALUE[] = "!@#$%^&*()?><|\":}{+_}~"; // Shifted value

int keyboard_shift;     // If shift is held
int keyboard_capslock;  // If capslock is on
int keyboard_key_break; // If the last key was a break scancode
int keyboard_ext;       // If the last key was a EXT scancode

char keyboard_buffer[256];
char keyboard_buffer_read_ptr;
char keyboard_buffer_write_ptr;

/*
 * Translate a keyboard scancode to ACSII.
 */
char  keyboard_scancode_to_acsii(char scancode) {
	char acsii = KEYBOARD_SCANCODE_TO_ACSII[(int)scancode]; // ACSII of scancode

	if (keyboard_capslock) {
		// If in lowercase range, turn to uppercase
		if (acsii >= 65 && acsii <= 90) 
			acsii += 32;               
	}

	if (keyboard_shift) {
		// If in uppercase range, turn to lowercase
		if (acsii >= 97 && acsii <= 122)
			acsii -= 32;
		// If in lowercase range, turn to uppercase
		else if (acsii >= 65 && acsii <= 90)
			acsii += 32;
		// Shifted keys
		else {
			// For each shifted keys
			for (unsigned int i = 0; i < sizeof(KEYBOARD_SHIFTED_KEY)-1; i++) {
				if (KEYBOARD_SHIFTED_KEY[i] == acsii) { // If this is the same
				                                        // as out ACSII value
					acsii = KEYBOARD_SHIFTED_VALUE[i];  // Change it

					break;
				}
			}
		}
	}

	return acsii; // Return our transformed value
}

/// @brief Push data to the keyboard buffer.
/// @param data The data.
void keyboard_buf_push(char data) {
	keyboard_buffer[(int)keyboard_buffer_write_ptr++] = data; // Push
}

/// @brief Handler for keyboard interrupt.
void keyboard_handler() {
	uint8_t data = inb(DATA_PORT); // Get our data!

	if (keyboard_ext) { // Extended mode
		if (data == 0xF0) { // Break code
			// Set it and return
			keyboard_key_break = 1;
			return;
		}

		if (keyboard_key_break) { // We're on a break code! We ignore it
			// Set values to good values. Epic values
			keyboard_key_break = 0;
			keyboard_ext = 0;
			
			return;
		}

		keyboard_buf_push(0); // Push zero. We do DOS-style extended keys.

		switch (data) {
			// Arrows keys. We use WASD (im a hardcode gamer)
			case 0x6B: // Left
				keyboard_buf_push('A');

				break;
			case 0x72: // Down
				keyboard_buf_push('S');

				break;
			case 0x74: // Right
				keyboard_buf_push('D');

				break;
			case 0x75: // Up
				keyboard_buf_push('W');

				break;

			// Default case. We just add zero.
			default:
				keyboard_buf_push('\0');

				break;
		}

		keyboard_ext = 0; // Set extended to zero; we are done
		return;
	}

	if (data == 0xF0) { // Break scancode
		// Set it and exit.
		keyboard_key_break = 1;

		return;
	}

	if (keyboard_key_break) { // We're on a break.
		if (data == 0x12) { // Handle shift
			keyboard_shift = 0;
		}

		keyboard_key_break = 0; // Set back to 0

		return;
	}

	if (data == 0x12) { // Shift
		// Set it and leave
		keyboard_shift = 1;

		return;
	}

	if (data == 0x58) { // Caps lock
		// Set it and leave
		keyboard_capslock = !keyboard_capslock;

		return;
	}

	if (data == 0xE0 && !keyboard_key_break) { // Extended code
		keyboard_ext = 1;
		return;
	}

	// Else, just push our data
	keyboard_buf_push(keyboard_scancode_to_acsii(data));
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
	config_byte &= 0b10101110; // Clear a couple bits
	keyboard_controller_send_command(WRITE_BYTE_0, 1, config_byte, 1, 0); // Set it back

	// Step 4: Perform self test
	if (keyboard_controller_send_command(SELF_TEST, 0, 0, 0, 0) == TEST_PASSED) { // Send test
		return KEYBOARD_INIT_SELFTEST_FAIL; // Report fail
	}

	// Step 5: Are there 2 channels?
	keyboard_controller_send_command(ENABLE_SECOND_PS2, 0, 0, 0, 0); // Enable 
	config_byte = keyboard_controller_send_command(READ_BYTE_0, 0, 0, 1, 0); // Get config byte
	if (config_byte & 0b00100000) { // If this bit is zero (false), that mean the second port isnt
	                                // enabled. If so, its not dual channel, as it should be
									// enabled.
		ps2_dual_channel = 0;       // No dual channel!

		
	} else { // Dual channel!
		ps2_dual_channel = 1;

		keyboard_controller_send_command(DISABLE_SECOND_PS2, 0, 0, 0, 0); // Disable second PS2
		                                                                  // again
		config_byte &= 0b11011101; // Enable clock and disable IRQS
		keyboard_controller_send_command(WRITE_BYTE_0, 1, config_byte, 1, 0); // Write it back
	}

	// Step 6: Test ports
	first_ps2_works = keyboard_controller_send_command(TEST_FIRST_PS2, 0, 0, 0, 0) == 0; // Test it
	ps2_dual_channel = ps2_dual_channel && 
	                   keyboard_controller_send_command(TEST_SECOND_PS2, 0, 0, 0, 0) == 0; // Test it

	if (!first_ps2_works && !ps2_dual_channel) { // No ports left!
		return KEYBOARD_INIT_NO_PORTS_LEFT;
	}

	// Step 7: Enable ports
	if (first_ps2_works) keyboard_controller_send_command(ENABLE_FIRST_PS2, 0, 0, 0, 0); // First
	if (ps2_dual_channel) keyboard_controller_send_command(ENABLE_SECOND_PS2, 0, 0, 0, 0); // Second

	config_byte = keyboard_controller_send_command(READ_BYTE_0, 0, 0, 1, 0); // Read config byte
	if (first_ps2_works) config_byte |= 0x00000001;  // Enable first PS2 IRQs
	if (ps2_dual_channel) config_byte |= 0x00000010; // Enable second PS2 IRQs
	keyboard_controller_send_command(WRITE_BYTE_0, 1, config_byte, 1, 0); // Write it back

	// TODO: Reset devices

	// Step 9: Attach interrupt
	attach_interrupt(0x21, keyboard_handler);

	keyboard_key_break = keyboard_shift = keyboard_ext = 0; // Set control values

	return KEYBOARD_INIT_SUCCESS; // Success!
}
