// HumbleOS file: ps2_controller.h
// Purpose: Header file for the PS2 controller.

#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

#include <stdint.h>

/// @brief Initalize PS2 controller.
/// @return If it worked.
int ps2_controller_init();

/// @brief Read the status from the controler.
/// @return The status.
uint8_t controller_read_status();

/// @brief Send a command to the controller, with no output (or arguments)
/// @param command The command to send.
void controller_send_command(uint8_t command);

/// @brief Send a command to the controller, with no output and one argument
/// @param command The command to send.
/// @param argument The argument to send.
void controller_send_command_with_argument(uint8_t command, uint8_t argument);

/// @brief Send a command to the controller, with output and no arguments
/// @param command The command to send.
/// @return The output of the command.
uint8_t controller_send_command_with_output(uint8_t command);

/// @brief Send a command to the controller, with output and one argument
/// @param command The command to send.
/// @param argument The argument to send.
/// @return The output of the command.
uint8_t controller_send_command_with_output_and_argument(uint8_t command, uint8_t argument);

/// @brief Read the controller configuration byte
/// @return The configuration byte.
uint8_t controller_read_configuration_byte();

/// @brief Write to the controller configuration byte
/// @param configuration The configuration byte to write.
void controller_write_configuration_byte(uint8_t configuration);

/// @brief If the first PS2 controller is present.
extern int first_ps2;

/// @brief If the second PS2 controller is present.
extern int second_ps2;

#endif
