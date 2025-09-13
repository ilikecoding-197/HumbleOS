/*
    events.h - events for HumbleOS

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

#ifndef EVENTS_H
#define EVENTS_H

#include <ints.h>

// event type
typedef enum {
    EVENT_TIMER,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP
} EventType;

// a event
typedef struct {
    EventType type; // type of the event
    void *data; // pointer to user-defined data when this event was registered
    union {
        uint timer_ms; // if type = EVENT_TIMER, the time ms the event was called at
        u16 keycode; // if type = EVENT_KEY_DOWN or EVENT_KEY_UP, the keycode
    } event; // event data
} Event;

// event handler
typedef void (*EventHandler)(Event*);

// register a event. returns the id of the event handler.
uint add_event(EventHandler handler, void *data);

// remove a event from its id
void remove_event(uint id);

// trigger a event - sends the event to every handlER
void trigger_event(Event *e);

#endif /* EVENTS_H */