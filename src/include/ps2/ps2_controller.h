/*
    ps2_controller.h - header file for the ps2 controller

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
#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

#include <ints.h>

// initalize ps2 controller
int ps2_controller_init();

// read the controllers
u8 controller_read_status();

// send a command to the controller
void controller_send_command(u8 command);

// send a command to the controller with a argument
void controller_send_command_with_argument(u8 command, u8 argument);

// send a command to the controller and return the output
u8 controller_send_command_with_output(u8 command);

// send a command to the controller with a argument and return the output
u8 controller_send_command_with_output_and_argument(u8 command, u8 argument);

// read the controller configuration byte
u8 controller_read_configuration_byte();

// write to the controller configuration byte
void controller_write_configuration_byte(u8 configuration);

// if the first ps2 port is present
extern int first_ps2;

// if the second ps2 port is present
extern int second_ps2;

// restart the system with ps2
void ps2_restart_system();

#endif /* PS2_CONTROLLER_H */
