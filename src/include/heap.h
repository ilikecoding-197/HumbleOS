// HumbleOS file: heap.h
// Purpose: Header file for heap. 

#include <stdint.h>

#ifndef HEAP_H
#define HEAP_H

/// @brief Dynamically allocate some memory.
/// @param size Amount to allocate.
/// @return On fail, returns NULL, else the pointer to the memory.
void *heap_malloc(uint32_t size);

/// @brief Free a heap pointer.
/// @param ptr Pointer to free. If NULL, return. If not a heap pointer, who knows what will happen.
void heap_free(void *ptr);

/// @brief Initalize Heap.
void heap_init();

#endif // HEAP_H