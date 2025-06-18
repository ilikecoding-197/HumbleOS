// HumbleOS file: panic.h
// Purpose: Header file for panic.

#ifndef PANIC_H
#define PANIC_H

#define _STR(x) #x
#define STR(x) _STR(x)

/// @brief Panic. Use this instead of panic_panic.
/// @param msg Message to print.
#define PANIC(msg) do { __asm__ __volatile__ ("call panic_save_regs"); panic_panic(msg, __FILE__, STR(__LINE__)); } while (0);

/// @brief Panic. Regs must be saved from panic_save_regs.
/// @param msg Message to print.
/// @param file File that called panic. (filled in by PANIC)
/// @param line Line that called panic. (filled in by PANIC)
/// @note Do not use this! This is only used from the PANIC macro, which also
///       saves the registers. Use that macro instead.
void panic_panic(char *msg, char *file, char *line) __attribute__((noreturn));

#endif // PANIC_H
