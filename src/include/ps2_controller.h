// HumbleOS file: ps2_controller.h
// Purpose: Header file for the PS2 controller.

#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

#include <ints.h>

/// @brief Initalize PS2 controller.
/// @return If it worked.
int ps2_controller_init();

/// @brief Read the status from the controler.
/// @return The status.
u8 controller_read_status();

/// @brief Send a command to the controller, with no output (or arguments)
/// @param command The command to send.
void controller_send_command(u8 command);

/// @brief Send a command to the controller, with no output and one argument
/// @param command The command to send.
/// @param argument The argument to send.
void controller_send_command_with_argument(u8 command, u8 argument);

/// @brief Send a command to the controller, with output and no arguments
/// @param command The command to send.
/// @return The output of the command.
u8 controller_send_command_with_output(u8 command);

/// @brief Send a command to the controller, with output and one argument
/// @param command The command to send.
/// @param argument The argument to send.
/// @return The output of the command.
u8 controller_send_command_with_output_and_argument(u8 command, u8 argument);

/// @brief Read the controller configuration byte
/// @return The configuration byte.
u8 controller_read_configuration_byte();

/// @brief Write to the controller configuration byte
/// @param configuration The configuration byte to write.
void controller_write_configuration_byte(u8 configuration);

/// @brief If the first PS2 controller is present.
extern int first_ps2;

/// @brief If the second PS2 controller is present.
extern int second_ps2;

#endif
