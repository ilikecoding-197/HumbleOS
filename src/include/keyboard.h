#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_INIT_SUCCESS 0
#define KEYBOARD_INIT_SELFTEST_FAIL 1
#define KEYBOARD_INIT_NO_PORTS_LEFT 2
int keyboard_init();

extern int ps2_dual_channel;
extern int first_ps2_works;

extern char keyboard_buffer[256];
extern int keyboard_buffer_read_ptr;
extern int keyboard_buffer_write_ptr;

#endif
