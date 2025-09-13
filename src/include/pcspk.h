/*
    pcspk.h - header for pc speaker support

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
#ifndef PCSPK_H
#define PCSPK_H

#include <ints.h>

// play sound at a certain HZ level
void pcspk_play_sound(u32 hz);

// stop playing any sound from the speaker
void pcspk_nosound();

// make a tiny, 1000 HZ, beep
void beep();

// make a tiny beep at a frequency
void beep_at(u32 hz);

// make a beep (1000 HZ) for a certain amount of time
void beep_for(uint sec);

// make a beep at a certain frequency and amount of time
void beep_at_for(u32 hz, uint sec);

#endif /* PCSPK_H */