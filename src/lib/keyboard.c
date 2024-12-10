#include <keyboard.h>
#include <console.h>
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
#define SYS_RESET 0xFE

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
const char KEYBOARD_SHIFTED_KEY[]   = "1234567890/.,\\';][=-`";
const char KEYBOARD_SHIFTED_VALUE[] = "!@#$%^&*()?><|\":}{+_}~";

int keyboard_shift;
int keyboard_capslock;
int keyboard_key_break;
int keyboard_ext;

char keyboard_buffer[256];
int keyboard_buffer_read_ptr;
int keyboard_buffer_write_ptr;

/*
 * Translate a keyboard scancode to ACSII.
 */
char  keyboard_scancode_to_acsii(char scancode) {
	char acsii =  KEYBOARD_SCANCODE_TO_ACSII[(int)scancode];

	if (keyboard_capslock) {
		if (acsii >= 65 && acsii <= 90) acsii += 32;
	}

	if (keyboard_shift) {
		if (acsii >= 97 && acsii <= 122) {
			acsii -= 32;
		} else if (acsii >= 65 && acsii <= 90) {
			acsii += 32;
		} else {
			for (unsigned int i = 0; i < sizeof(KEYBOARD_SHIFTED_KEY)-1; i++) {
				if (KEYBOARD_SHIFTED_KEY[i] == acsii) {
					acsii = KEYBOARD_SHIFTED_VALUE[i];

					break;
				}
			}
		}
	}

	return acsii;
}

void keyboard_buf_push(char data) {
	keyboard_buffer[keyboard_buffer_write_ptr++] = data;
}

/*
 * Handler for keyboard interrupt.
 */
void keyboard_handler() {
	uint8_t data = inb(DATA_PORT);

	if (keyboard_ext) {
		if (data == 0xF0) {
			keyboard_key_break = 1;
			return;
		}

		if (keyboard_key_break) {
			keyboard_key_break = 0;
			keyboard_ext = 0;
			
			return;
		}

		keyboard_buf_push(0);

		switch (data) {
			case 0x6B:
				keyboard_buf_push('A');

				break;
			case 0x72:
				keyboard_buf_push('S');

				break;
			case 0x74:
				keyboard_buf_push('D');

				break;
			case 0x75:
				keyboard_buf_push('W');

				break;
		}

		keyboard_ext = 0;
		return;
	}

	if (data == 0xF0) {
		keyboard_key_break = 1;

		return;
	}

	if (keyboard_key_break) {
		if (data == 0x12) {
			keyboard_shift = 0;
		}

		keyboard_key_break = 0;

		return;
	}

	if (data == 0x12) {
		keyboard_shift = 1;

		return;
	}

	if (data == 0x58) {
		keyboard_capslock = !keyboard_capslock;

		return;
	}

	if (data == 0xE0 && !keyboard_key_break) {
		keyboard_ext = 1;
		return;
	}

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

	keyboard_key_break = keyboard_shift = keyboard_ext = 0;

	return KEYBOARD_INIT_SUCCESS;
}
