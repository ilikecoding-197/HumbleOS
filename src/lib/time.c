// HumbleOS file: time.c
// Purpose: Time functions

#include <time.h>
#include <console.h>
#include <idt.h>

int time_ms;
int time_can_use_for_klog = 0;
int time_sleep_ms_start;

void time_interrupt() {
    time_ms++;
}

void time_init() {
    klog("time", "initalizing time");
    time_ms = 0;
    time_can_use_for_klog = 1;

    // We will use the PIT to get the time, at a rate of 1000Hz (every millisecond the 
    // time_interrupt function will be called to increate time_ms)

    klog("time", "initalizing PIT at 1000Hz");
    __asm__ ("call init_PIT"); // Call the init_PIT function in assembly
    klog("time", "done!");

    // Set up the interrupt handler for the PIT
    klog("time", "setting up time interrupt");
    attach_interrupt(32, time_interrupt);
    klog("time", "done!");
}

void sleep(int ms) {
    time_sleep_ms_start = time_ms;
    while (time_ms - time_sleep_ms_start < ms) {
        // Do nothing
    }
}