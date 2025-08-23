// HumbleOS file: cpu.h
// Purpose: CPU-level functions

#ifndef CPU_H
#define CPU_H

// Halt the CPU until the next interrupt.
static inline void cpu_hlt()
{
    __asm__ __volatile__("hlt");
}

#endif // CPU_H