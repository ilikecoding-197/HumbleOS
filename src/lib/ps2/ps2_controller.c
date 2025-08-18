// HumbleOS file: ps2_controller.c
// Purpose: PS2 controller

#include <ps2/ps2_controller.h>
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

// Wait until input buffer is empty
static void _controller_wait_input_empty() {
    while (controller_read_status() & 0x2); // bit 1 = input buffer full
}

// Wait until output buffer is full
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
    klog("PS2_CTRL", "initializing (DEBUG)...");

    // 1) Disable devices
    klog("PS2_CTRL", "disabling devices...");
    controller_send_command(CONTROLLER_DISABLE_PS2_SECOND);
    controller_send_command(CONTROLLER_DISABLE_PS2_FIRST);

    // 2) Flush buffer
    klog("PS2_CTRL", "flushing buffer...");
    u8 flush = controller_send_command_with_output(CONTROLLER_READ_OUTPUT_PORT);
    klogf("PS2_CTRL", "flushed byte: 0x%02X", flush);

    // 3) Update configuration byte
    klog("PS2_CTRL", "reading configuration byte...");
    u8 configuration = controller_read_configuration_byte();
    klogf("PS2_CTRL", "original config byte: 0x%02X", configuration);

    configuration &= ~(1 << 0); // clear IRQ1
    configuration &= ~(1 << 1); // clear IRQ12
    configuration &= ~(1 << 4); // disable port 1 clock
    configuration &= ~(1 << 5); // disable port 2 clock
    configuration &= ~(1 << 6); // disable translation
    klogf("PS2_CTRL", "writing config byte: 0x%02X", configuration);
    controller_write_configuration_byte(configuration);

    // 4) Perform self-test
    klog("PS2_CTRL", "performing self-test...");
    u8 result = controller_send_command_with_output(CONTROLLER_SELF_TEST);
    klogf("PS2_CTRL", "self-test result: 0x%02X", result);
    if (result != 0x55) {
        klog("PS2_CTRL", "FAIL: self-test failed");
        return 0;
    }

    // 5) Test for second channel
    klog("PS2_CTRL", "enabling second channel for detection...");
    controller_send_command(CONTROLLER_ENABLE_PS2_SECOND);
    configuration = controller_read_configuration_byte();
    klogf("PS2_CTRL", "config byte after enabling second: 0x%02X", configuration);
    second_ps2 = (configuration & (1 << 5)) == 0;
    if (second_ps2) klog("PS2_CTRL", "DUAL CHANNEL DETECTED");
    else klog("PS2_CTRL", "SINGLE CHANNEL CONTROLLER");

    // disable second channel again
    controller_send_command(CONTROLLER_DISABLE_PS2_SECOND);
    configuration &= ~(1 << 1); // clear IRQ12
    configuration &= ~(1 << 5); // disable port 2 clock
    controller_write_configuration_byte(configuration);

    // 6) Test first port
    klog("PS2_CTRL", "testing first port...");
    result = controller_send_command_with_output(CONTROLLER_TEST_PS2_FIRST);
    first_ps2 = (result == 0x00);
    klogf("PS2_CTRL", "first port test result: 0x%02X", result);
    if (first_ps2) klog("PS2_CTRL", "1st port: PASS");
    else klog("PS2_CTRL", "1st port: FAIL");

    // test second port if exists
    if (second_ps2) {
        klog("PS2_CTRL", "testing second port...");
        result = controller_send_command_with_output(CONTROLLER_TEST_PS2_SECOND);
        second_ps2 = (result == 0x00);
        klogf("PS2_CTRL", "second port test result: 0x%02X", result);
        if (second_ps2) klog("PS2_CTRL", "2nd port: PASS");
        else klog("PS2_CTRL", "2nd port: FAIL");
    }

    if (!first_ps2 && !second_ps2) {
        klog("PS2_CTRL", "NO PORTS ARE WORKING! FAIL!");
        return 0;
    }

    // 7) Enable devices
    klog("PS2_CTRL", "enabling devices...");
    if (first_ps2) controller_send_command(CONTROLLER_ENABLE_PS2_FIRST);
    if (second_ps2) controller_send_command(CONTROLLER_ENABLE_PS2_SECOND);

    // 8) Enable IRQs and clocks
    configuration = controller_read_configuration_byte();
    klogf("PS2_CTRL", "config byte before enabling IRQs: 0x%02X", configuration);
    configuration |= (1 << 0); // enable IRQ1
    configuration |= (1 << 1); // enable IRQ12
    configuration &= ~(1 << 4); // enable port 1 clock
    configuration &= ~(1 << 5); // enable port 2 clock
    controller_write_configuration_byte(configuration);
    klogf("PS2_CTRL", "final config byte: 0x%02X", configuration);

    klog("PS2_CTRL", "DEBUG INIT COMPLETE");
    return 1;
}

void ps2_restart_system() {
    klog("PS2_CTRL", "restarting system...");
    
    // Method 1: Try PS/2 controller reset
    controller_send_command(0xFE); // Reset command
    
    // Method 2: If that fails, try keyboard controller
    u8 temp;
    do {
        temp = controller_read_status();
        if (temp & 0x01) {
            inb(CONTROLLER_DATA_PORT); // Clear input buffer
        }
    } while (temp & 0x02); // Wait for input buffer to be empty
    
    outb(CONTROLLER_STATUS_COMMAND_PORT, 0xFE); // Reset
    
    // Method 3: Last resort - triple fault
	klog("PS2_CTRL", "COULD NOT RESTART SYSTEM! do it yourself.");
    __asm__ volatile("cli; hlt");
}
