/*
    time.c - time functions

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

#include <time.h>
#include <console.h>
#include <idt.h>
#include <ints.h>
#include <events.h>
#include <cpu.h>

uint time_ms;
uint time_can_use_for_klog = 0;
uint time_sleep_ms_start;

void time_interrupt() {
    time_ms++;

    Event timeEvent;
    timeEvent.type = EVENT_TIMER;
    timeEvent.event.timer_ms = time_ms;
    trigger_event(&timeEvent);
}

void time_init() {
    klog("time", "initalizing time");
    time_ms = 0;

    // we will use the PIT to get the time, at a rate of 1000Hz (every millisecond the 
    // time_interrupt function will be called to increate time_ms)

    klog("time", "initalizing PIT at 1000Hz");
    __asm__ ("call init_PIT"); // call the init_PIT function in assembly
    klog("time", "done!");

    // set up the interrupt handler for the PIT
    klog("time", "setting up time interrupt");
    attach_interrupt(32, time_interrupt);
    klog("time", "done! kernel log messages can now use the time");
    time_can_use_for_klog = 1;
}

void sleep(uint ms) {
    time_sleep_ms_start = time_ms;
    while (time_ms - time_sleep_ms_start < ms) {
        // instead of doing nothing, halt. when a interrupt comes in,
        // the halting will stop, which will make us check.
        cpu_hlt(); 
    }
}