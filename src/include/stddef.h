// HumbleOS file: stddef.h
// Purpose: Replacement for stddef.h as we dont have C headers

#ifndef STDDEF_H
#define STDDEF_H

#define NULL 0
typedef unsigned int size_t;
typedef typeof((int*)NULL - (int*)NULL) ptrdiff_t;

#endif // STDDEF_H