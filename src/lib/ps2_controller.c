// HumbleOS file: ps2_controller.c
// Purpose: PS2 controller

#include <ps2_controller.h>
#include <ints.h>
#include <port.h>
#include <console.h>

// Ports
#define CONTROLLER_DATA_PORT 0x60
#define CONTROLLER_STATUS_COMMAND_PORT 0x64

// Controller commands
#define CONTROLLER_READ_BYTE_0 0x20
#define CONTROLLER_WRITE_BYTE_0 0x60
#define CONTROLLER_DISABLE_PS2_SECOND 0xA7
#define CONTROLLER_ENABLE_PS2_SECOND 0xA8
#define CONTROLLER_TEST_PS2_SECOND 0xA9
#define CONTROLLER_SELF_TEST 0xAA
#define CONTROLLER_TEST_PS2_FIRST 0xAB
#define CONTROLLER_DISABLE_PS2_FIRST 0xAD
#define CONTROLLER_ENABLE_PS2_FIRST 0xAE
#define CONTROLLER_READ_OUTPUT_PORT 0xD0
#define CONTROLLER_WRITE_OUTPUT_PORT 0xD1

// Functions from the header file
u8 controller_read_status() {
    return inb(CONTROLLER_STATUS_COMMAND_PORT);
}

void controller_send_command(u8 command) {
    outb(CONTROLLER_STATUS_COMMAND_PORT, command);
}

void controller_send_command_with_argument(u8 command, u8 argument) {
    outb(CONTROLLER_STATUS_COMMAND_PORT, command);

	// Wait for the controller to be ready
	while ((controller_read_status() & 0x2) != 0);
	outb(CONTROLLER_DATA_PORT, argument);
}

u8 controller_send_command_with_output(u8 command) {
	outb(CONTROLLER_STATUS_COMMAND_PORT, command);

	// Wait until it arrived
	while ((controller_read_status() & 0x1) == 0);
	return inb(CONTROLLER_DATA_PORT);
}

u8 controller_send_command_with_output_and_argument(u8 command, u8 argument) {
	outb(CONTROLLER_STATUS_COMMAND_PORT, command);

	// Wait for the controller to be ready
	while ((controller_read_status() & 0x2) != 0);
	outb(CONTROLLER_DATA_PORT, argument);

	// Wait until it arrived
	while ((controller_read_status() & 0x1) == 0);
	return inb(CONTROLLER_DATA_PORT);
}

u8 controller_read_configuration_byte() {
	return controller_send_command_with_output(CONTROLLER_READ_BYTE_0);
}

void controller_write_configuration_byte(u8 configuration) {
	controller_send_command_with_argument(CONTROLLER_WRITE_BYTE_0, configuration);
}

int first_ps2 = 1;
int second_ps2 = 1;

int ps2_controller_init() {
	klog("PS2", "initalizing...");

	// 1) Disable devices
	klog("PS2", "disabling devices...");
	controller_send_command(CONTROLLER_DISABLE_PS2_SECOND);
	controller_send_command(CONTROLLER_DISABLE_PS2_FIRST);

	// 2) Flush buffer
	klog("PS2", "flushing buffer...");
	controller_send_command_with_output(CONTROLLER_READ_OUTPUT_PORT);
	
	// 3) Update configuration byte
	// 3.1) Read the configuration byte
	// 3.2) Clear bits 0, 6, and 4
	// 3.3) Write the configuration byte
	klog("PS2", "updating configuration byte...");
	u8 configuration = controller_read_configuration_byte();
	configuration &= 0x10111110; // Clear bits 0 and 6 (IRQ and translation from port 1)
	configuration &= 0x11101111; // Clear bit 4 (enable clock signal)
	controller_write_configuration_byte(configuration);

	// 4) Perform self-test
	klog("PS2", "performing self-test...");
	u8 result = controller_send_command_with_output(CONTROLLER_SELF_TEST);
	if (result != 0x55) {
		klog("PS2", "FAIL");
		return 0;
	}

	// 5) Are there two channels?
	// 5.1) Try to enable the second channel
	// 5.2) Read the configuration byte
	// 5.3) Bit 5 should be cleared if we have two channels - as we just enabled the second channel.
	// 5.4) Set the second_ps2 variable
	// 5.5) If we have two channels, disable the second channel again; and disable IRQs
	//      for it and enable clock (clearing bits 1 and 5 of the configuration byte
	klog("PS2", "testing for two channels...");
	controller_send_command(CONTROLLER_ENABLE_PS2_SECOND);
	configuration = controller_read_configuration_byte();
	second_ps2 = (configuration & 0x20) == 0;
	if (second_ps2) {
		klog("PS2", "DUAL CHANNEL!");
		controller_send_command(CONTROLLER_DISABLE_PS2_SECOND);
		configuration &= 0x11011101; // Clear bits 1 and 5
		controller_write_configuration_byte(configuration);
	}

	// 6) Perform tests
	// 6.1) Test the first channel and set the first_ps2 variable
	// 6.2) If we are dual-channel, test the second channel and set the second_ps2 variable
	// 6.3) If we dont have either of the ports, return 0
	klog("PS2", "performing tests...");
	result = controller_send_command_with_output(CONTROLLER_TEST_PS2_FIRST);
	first_ps2 = (result == 0x00);

	if (first_ps2) klog("PS2", "1st port: PASS");
	else klog("PS2", "1st port: FAIL");

	if (second_ps2) {
		result = controller_send_command_with_output(CONTROLLER_TEST_PS2_SECOND);
		second_ps2 = (result == 0x00);
		if (second_ps2) klog("PS2", "2nd port: PASS");
		else klog("PS2", "2nd port: FAIL");
	}

	if (!first_ps2 && !second_ps2) {
		klog("PS2", "NO PORTS ARE WORKING! FAIL!");
		return 0;
	}

	// 7) Enable devices
	klog("PS2", "enabling devices...");
	if (first_ps2)  controller_send_command(CONTROLLER_ENABLE_PS2_FIRST);
	if (second_ps2) controller_send_command(CONTROLLER_ENABLE_PS2_SECOND);

	klog("PS2", "CONTROLLER IS WORKING!");
	return 1; // Just say it worked for now
}