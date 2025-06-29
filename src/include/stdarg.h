// HumbleOS file: stdarg.h
// Purpose: Recreation of stdarg.h, from info found in MY system headers

#ifndef STDARG_H
#define STDARG_H

typedef __builtin_va_list va_list;
#define va_start(v, ...) __builtin_va_start(v, 0)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_copy(d, s) __builtin_va_copy(d, s)

#endif // STDARG_H