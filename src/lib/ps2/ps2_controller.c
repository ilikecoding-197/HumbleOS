/*
    ps2_controller.c - ps2 controller code

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

#include <ps2/ps2_controller.h>
#include <ints.h>
#include <port.h>
#include <console.h>
#include <panic.h>

// ports
#define CONTROLLER_DATA_PORT 0x60
#define CONTROLLER_STATUS_COMMAND_PORT 0x64

// controller commands
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

u8 controller_read_status() {
    return inb(CONTROLLER_STATUS_COMMAND_PORT);
}

static void _controller_wait_input_empty() {
    while (controller_read_status() & 0x2); // bit 1 = input buffer full
}

static void _controller_wait_output_full() {
    while (!(controller_read_status() & 0x1)); // bit 0 = output buffer full
}

void controller_send_command(u8 command) {
    _controller_wait_input_empty();
    outb(CONTROLLER_STATUS_COMMAND_PORT, command);
}

void controller_send_command_with_argument(u8 command, u8 argument) {
    _controller_wait_input_empty();
    outb(CONTROLLER_STATUS_COMMAND_PORT, command);
    
    _controller_wait_input_empty();
    outb(CONTROLLER_DATA_PORT, argument);
}

u8 controller_send_command_with_output(u8 command) {
    _controller_wait_input_empty();
    outb(CONTROLLER_STATUS_COMMAND_PORT, command);
    
    _controller_wait_output_full();
    return inb(CONTROLLER_DATA_PORT);
}

u8 controller_send_command_with_output_and_argument(u8 command, u8 argument) {
    _controller_wait_input_empty();
    outb(CONTROLLER_STATUS_COMMAND_PORT, command);
    
    _controller_wait_input_empty();
    outb(CONTROLLER_DATA_PORT, argument);
    
    _controller_wait_output_full();
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
    klog("PS2_CTRL", "initializing...");

    // 1) disable devices
    controller_send_command(CONTROLLER_DISABLE_PS2_SECOND);
    controller_send_command(CONTROLLER_DISABLE_PS2_FIRST);

    // 2) flush buffer
    u8 flush = controller_send_command_with_output(CONTROLLER_READ_OUTPUT_PORT);

    // 3) update configuration byte
    u8 configuration = controller_read_configuration_byte();

    configuration &= ~(1 << 0); // clear IRQ1
    configuration &= ~(1 << 1); // clear IRQ12
    configuration &= ~(1 << 4); // disable port 1 clock
    configuration &= ~(1 << 5); // disable port 2 clock
    configuration &= ~(1 << 6); // disable translation
    controller_write_configuration_byte(configuration);

    // 4) perform self-test
    u8 result = controller_send_command_with_output(CONTROLLER_SELF_TEST);
    if (result != 0x55) {
        klog("PS2_CTRL", "FAIL: self-test failed");
        return 0;
    }

    // 5) test for second channel
    controller_send_command(CONTROLLER_ENABLE_PS2_SECOND);
    configuration = controller_read_configuration_byte();
    second_ps2 = (configuration & (1 << 5)) == 0;
    if (second_ps2) klog("PS2_CTRL", "DUAL CHANNEL DETECTED");
    else klog("PS2_CTRL", "SINGLE CHANNEL CONTROLLER");

    // disable second channel again
    controller_send_command(CONTROLLER_DISABLE_PS2_SECOND);
    configuration &= ~(1 << 1); // clear IRQ12
    configuration &= ~(1 << 5); // disable port 2 clock
    controller_write_configuration_byte(configuration);

    // 6) test first port
    result = controller_send_command_with_output(CONTROLLER_TEST_PS2_FIRST);
    first_ps2 = (result == 0x00);
    if (first_ps2) klog("PS2_CTRL", "1st port: PASS");
    else klog("PS2_CTRL", "1st port: FAIL");

    // test second port if exists
    if (second_ps2) {
        result = controller_send_command_with_output(CONTROLLER_TEST_PS2_SECOND);
        second_ps2 = (result == 0x00);
        if (second_ps2) klog("PS2_CTRL", "2nd port: PASS");
        else klog("PS2_CTRL", "2nd port: FAIL");
    }

    if (!first_ps2 && !second_ps2) {
        klog("PS2_CTRL", "NO PORTS ARE WORKING! FAIL!");
        return 0;
    }

    // 7) enable devices
    if (first_ps2) controller_send_command(CONTROLLER_ENABLE_PS2_FIRST);
    if (second_ps2) controller_send_command(CONTROLLER_ENABLE_PS2_SECOND);

    // 8) enable IRQs and clocks
    configuration = controller_read_configuration_byte();
    configuration |= (1 << 0); // enable IRQ1
    configuration |= (1 << 1); // enable IRQ12
    configuration &= ~(1 << 4); // enable port 1 clock
    configuration &= ~(1 << 5); // enable port 2 clock
    controller_write_configuration_byte(configuration);

    klog("PS2_CTRL", "PS2 controller should be working now");
    return 1;
}

void ps2_restart_system() {
    klog("PS2_CTRL", "restarting system...");
    controller_send_command(0xFE);
    
    // failed, just halt
	PANIC("couldn't restart system. here, have a panic and halt!");
}
