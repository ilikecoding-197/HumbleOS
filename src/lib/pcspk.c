// HumbleOS file: pcspk.c
// Purpose: PC Speaker code

#include <pcspk.h>
#include <time.h>

void pcspk_play_sound(u32 hz)
{
    // Set the PI to the desired frequency
    u32 div = 1193180 / hz;
    outb(0x43, 0xb6);
    outb(0x42, (u8)(div));
    outb(0x42, (u8)(div >> 8));

    // Play the sound
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