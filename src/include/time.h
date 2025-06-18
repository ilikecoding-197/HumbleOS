// HumbleOS file: time.h
// Purpose: Header file for time

#ifndef TIME_H
#define TIME_H

#include <ints.h>

// For now all this part of HumbleOS will do is just keep track of the millseconds since time_init was called,
// and also have a little sleep function.

/// @brief Time (in milliseconds) since time_init was called
extern uint time_ms;

/// @brief If time_can_use_for_klog is 1, then the time module can be used for kernel logging
extern uint time_can_use_for_klog;

/// @brief Initialize the time module
void time_init();

/// @brief Sleep for a given number of milliseconds
void sleep(uint ms);

#endif // TIME_H