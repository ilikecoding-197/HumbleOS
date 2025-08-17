// HumbleOS file: ps2.h
// Purpose: Functions for handling PS2 devices, on top of the controller

#ifndef PS2_H
#define PS2_H


#include <ints.h>
#include "ps2/ps2_controller.h"

/// @brief ps/2 device types
typedef enum {
    PS2_DEVICE_NONE,
    PS2_DEVICE_KEYBOARD,
    PS2_DEVICE_MOUSE,
    PS2_DEVICE_UNKNOWN
} ps2_device_type;

/// @brief ps/2 device descriptor
typedef struct {
    ps2_device_type type;  /* [type of device] */
    int controller_index;  /* [0 for first, 1 for second] */
    const char *name;      /* [optional friendly name] */
    int present;           /* [if device is actually present] */
} ps2_device;

/// @brief initialize the ps/2 subsystem, including controller(s)
/// @return 0 on success, non-zero on failure
int ps2_init();

/// @brief detect all connected devices
/// @param devices pointer to array to fill
/// @param max_devices maximum number of devices to fill
/// @return number of devices detected
int ps2_detect_devices(ps2_device *devices, int max_devices);

/// @brief send a command to a specific device
/// @param device the device descriptor
/// @param command the command byte
void ps2_device_send_command(ps2_device *device, u8 command);

/// @brief send a command with one argument to a specific device
/// @param device the device descriptor
/// @param command the command byte
/// @param argument the argument byte
void ps2_device_send_command_with_argument(ps2_device *device, u8 command, u8 argument);

/// @brief send a command and get a response from a specific device
/// @param device the device descriptor
/// @param command the command byte
/// @return the response byte
u8 ps2_device_send_command_with_output(ps2_device *device, u8 command);

/// @brief send a command with argument and get response
/// @param device the device descriptor
/// @param command the command byte
/// @param argument the argument byte
/// @return the response byte
u8 ps2_device_send_command_with_output_and_argument(ps2_device *device, u8 command, u8 argument);

/// @brief generic read for a device (keyboard/mouse/etc.)
/// @param device the device descriptor
/// @return the data byte, or 0xff if nothing available
u8 ps2_device_read(ps2_device *device);

#endif // PS2_H