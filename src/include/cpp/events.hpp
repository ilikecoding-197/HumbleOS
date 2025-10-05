/*
    events.hpp - events wrapper for C++

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

#pragma once

extern "C" {
#include "events.h"
}

namespace events
{
    using type = EventType;
    using event = Event;
    using handler = EventHandler;

    inline constexpr type timer   = EVENT_TIMER;
    inline constexpr type key_down = EVENT_KEY_DOWN;
    inline constexpr type key_up   = EVENT_KEY_UP;

    inline uint add(handler h, void* data)
    {
        return add_event(h, data);
    }

    inline void remove(uint id)
    {
        remove_event(id);
    }

    inline void trigger(event* e)
    {
        trigger_event(e);
    }
}