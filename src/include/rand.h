// HumbleOS file: rand.h
// Purpose: Header file for randomness

#ifndef RAND_H
#define RAND_H

/// @brief Initalize randomness.
/// @note This will use the RDSEED instruction if available, if not RDRAND. If neither
///       of those exist, it will use a simple linear congruential generator (LCG) to 
///       generate random numbers. The LCG will simply be seeded by the current time in
///       milliseconds from time.h.
void rand_init();

/// @brief Get a random number.
/// @return A random number.
/// @note This is a function pointer - so we can easily just set this to the RDSEED, RDRAND, or LCG.
extern int (*rand)();

/// @brief Get a random number in a range.
/// @param min The minimum value of the range.
/// @param max The maximum value of the range.
/// @return A random number in the range.
/// @note If the range is invalid (min >= max), the function will return 0.
int rand_in_range(int min, int max);

#endif // RAND_H