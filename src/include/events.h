// HumbleOS file: events.h
// Purpose: System-wide events

#ifndef EVENTS_H
#define EVENTS_H

#include <ints.h>

typedef enum {
    EVENT_TIMER,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP
} EventType;

typedef struct {
    EventType type;
    void *data;
    union {
        uint timer_ms;
        u16 keycode;
    } event;
} Event;

typedef void (*EventHandler)(Event*);

uint add_event(EventHandler handler, void *data);
void remove_event(uint id);
void trigger_event(Event *e);

#endif