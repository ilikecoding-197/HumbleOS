/*
    pcspk.c - c code for speaker

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

#include <pcspk.h>
#include <time.h>
#include <port.h>

void pcspk_play_sound(u32 hz)
{
    // set the PIC to the desired frequency
    u32 div = 1193180 / hz;
    outb(0x43, 0xb6);
    outb(0x42, (u8)(div));
    outb(0x42, (u8)(div >> 8));

    // play the sound
    u8 tmp;
    tmp = inb(0x61);
    if (tmp != (tmp | 3))
    {
        outb(0x61, tmp | 3);
    }
}

void pcspk_nosound()
{
    outb(0x61, inb(0x61) & 0xFC);
}

void beep()
{
    pcspk_play_sound(1000);
    sleep(10);
    pcspk_nosound();
}

void beep_at(u32 hz)
{
    pcspk_play_sound(hz);
    sleep(10);
    pcspk_nosound();
}

void beep_for(uint sec)
{
    pcspk_play_sound(1000);
    sleep(sec);
    pcspk_nosound();
}

void beep_at_for(u32 hz, uint sec)
{
    pcspk_play_sound(hz);
    sleep(sec);
    pcspk_nosound();
}