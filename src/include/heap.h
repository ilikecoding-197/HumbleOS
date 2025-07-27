// HumbleOS file: heap.h
// Purpose: Header file for heap. 

#include <ints.h>

#ifndef HEAP_H
#define HEAP_H

/// @brief Dynamically allocate some memory.
/// @param size Amount to allocate.
/// @return On fail, returns NULL, else the pointer to the memory.
void *heap_malloc(u32 size);

/// @brief Dynamically allocate some memory, cleared with 0.
/// @param size Amount to allocate.
/// @return On fail, returns NULL, else the pointer to the memory.
void *heap_calloc(u32 size);

/// @brief Free a heap pointer.
/// @param ptr Pointer to free. If NULL, return. If not a heap pointer, who knows what will happen.
void heap_free(void *ptr);

/// @brief Initalize Heap.
void heap_init();

#define _STR(x) #x
#define STR(x) _STR(x)

#define kmalloc(size) _kmalloc(size, __FILE__, STR(__LINE__))
#define kfree(ptr) heap_free(ptr)

#endif // HEAP_H