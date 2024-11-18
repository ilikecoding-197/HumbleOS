#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

#define KEYBOARD_INIT_SUCCESS 0
#define KEYBOARD_INIT_SELFTEST_FAIL 1
#define KEYBOARD_INIT_NO_PORTS_LEFT 2
int keyboard_init();

extern bool keyboard_dual_channel;
extern bool keyboard_first_ps2_works;

#endif
