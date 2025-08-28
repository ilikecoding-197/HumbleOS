// HumbleOS file: pcspk.h
// Purpose: PC Speaker header

#ifndef PCSPK_H
#define PCSPK_H

#include <ints.h>

// Play sound at a certain HZ level.
void pcspk_play_sound(u32 hz);

// Stop playing any sound from the speaker.
void pcspk_nosound();

// Make a tiny, 1000 HZ, beep.
void beep();

// Make a tiny beep at a frequency.
void beep_at(u32 hz);

// Make a beep (1000 HZ) for a certain amount of time.
void beep_for(uint sec);

// Make a beep at a certain frequency and amount of time.
void beep_at_for(u32 hz, uint sec);

#endif // PCSPK_H