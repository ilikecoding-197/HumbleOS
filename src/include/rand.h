/*
    rand.h - header file for randomness

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
#ifndef RAND_H
#define RAND_H

// initalize randomness
// this will use the RDSEED instruction if available, if not RDRAND. if neither
// of those exist, it will use a simple linear congruential generator (LCG) to 
// generate random numbers. the LCG will simply be seeded by the current time in
// milliseconds from time.h
void rand_init();

// get a random number
// this is a pointer to a function, set in rand_init, to allow using multiple
// different types of random generators
extern int (*rand)();

// get a random number in a range
int rand_in_range(int min, int max);

#endif /* RAND_H */ // RAND_H