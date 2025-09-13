/*
    events.c - code for system-wide events

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



#include <events.h>
#include <stddef.h>
#include <panic.h>

#define HANDLER_AMT 512
static EventHandler handlers[HANDLER_AMT] = { NULL };
static void *handlersData[HANDLER_AMT] = { NULL };

uint add_event(EventHandler handler, void *data) {
    for (uint index = 0; index < HANDLER_AMT; index++) {
        if (handlers[index] == NULL) {
            handlers[index] = handler;
            handlersData[index] = data;
            return index;
        }
    }

    PANIC("EVENT HANDLERS EXHAUSTED!!!");
}

void remove_event(uint id) {
    if (id < HANDLER_AMT) {
        handlers[id] = NULL;
        handlersData[id] = NULL;
    }
}

void trigger_event(Event *e) {
    for (uint index = 0; index < HANDLER_AMT; index++) {
        if (handlers[index] != NULL) {
            e->data = handlersData[index];
            handlers[index](e);
        }
    }
}
