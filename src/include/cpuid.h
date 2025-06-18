// HumbleOS file: cpuid.h
// Purpose: Header file for cpuid.c

#ifndef CPUID_H
#define CPUID_H

/// @brief Initalize CPUID
/// @note  If CPUID is not supported, a PANIC will happen.
void cpuid_init();

#endif // CPUID_H