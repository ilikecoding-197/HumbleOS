// HumbleOS file: events.c
// Purpose: System-wide events

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
