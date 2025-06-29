// HumbleOS file: stdbool.h
// Purpose: Replacement for stdbool.h as we dont have the C headers

#ifndef STDBOOL_H
#define STDBOOL_H

#include <ints.h>

typedef u8 bool; // Boolean type
#define true 1
#define false 0

#endif // STDBOOL_H