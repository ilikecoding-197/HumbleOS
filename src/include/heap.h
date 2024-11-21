#include <stdint.h>

#ifndef HEAP_H
#define HEAP_H

void *heap_malloc(uint32_t size);
void heap_free(void *ptr);
void heap_init();

#endif // HEAP_H
