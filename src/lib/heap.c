/*
	heap.c - heap code (not by me!)

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

#include <ints.h>
#include <console.h>
#include <panic.h>
#include <stddef.h>
#include <string.h>
#include <sys_info.h>

// CODE FROM https://wiki.osdev.org/User:Pancakes/BitmapHeapImplementation
typedef struct _KHEAPBLOCKBM {
	struct _KHEAPBLOCKBM *next;
	u32              size;
	u32              used;
	u32              bsize;
    u32              lfb;
} KHEAPBLOCKBM;

typedef struct _KHEAPBM {
	KHEAPBLOCKBM *fblock;
} KHEAPBM;

void k_heapBMInit(KHEAPBM *heap) {
	heap->fblock = 0;
}

int k_heapBMAddBlock(KHEAPBM *heap, uintptr_t addr, u32 size, u32 bsize) {
	KHEAPBLOCKBM		*b;
	u32				bcnt;
	u32				x;
	u8				*bm;
	
	b = (KHEAPBLOCKBM*)addr;
	b->size = size - sizeof(KHEAPBLOCKBM);
	b->bsize = bsize;
	
	b->next = heap->fblock;
	heap->fblock = b;

	bcnt = b->size / b->bsize;
	bm = (u8*)&b[1];
	
	/* clear bitmap */
	for (x = 0; x < bcnt; ++x) {
			bm[x] = 0;
	}

	/* reserve room for bitmap */
	bcnt = (bcnt / bsize) * bsize < bcnt ? bcnt / bsize + 1 : bcnt / bsize;
	for (x = 0; x < bcnt; ++x) {
			bm[x] = 5;
	}
	
	b->lfb = bcnt - 1;
	
	b->used = bcnt;
	
	return 1;
}

static u8 k_heapBMGetNID(u8 a, u8 b) {
	u8		c;	
	for (c = a + 1; c == b || c == 0; ++c);
	return c;
}

void *k_heapBMAlloc(KHEAPBM *heap, u32 size) {
	KHEAPBLOCKBM		*b;
	u8				*bm;
	u32				bcnt;
	u32				x, y, z;
	u32				bneed;
	u8				nid;

	/* iterate blocks */
	for (b = heap->fblock; b; b = b->next) {
		/* check if block has enough room */
		if (b->size - (b->used * b->bsize) >= size) {
			
			bcnt = b->size / b->bsize;		
			bneed = (size / b->bsize) * b->bsize < size ? size / b->bsize + 1 : size / b->bsize;
			bm = (u8*)&b[1];
			
			for (x = (b->lfb + 1 >= bcnt ? 0 : b->lfb + 1); x < bcnt; ++x) {
				if (bm[x] == 0) {	
					/* count free blocks */
					for (y = 0; bm[x + y] == 0 && y < bneed && (x + y) < bcnt; ++y);
					
					/* we have enough, now allocate them */
					if (y == bneed) {
						/* find ID that does not match left or right */
						nid = k_heapBMGetNID(x > 0 ? bm[x - 1] : 0, (x + y) < bcnt ? bm[x + y] : 0);
						
						/* allocate by setting id */
						for (z = 0; z < y; ++z) {
							bm[x + z] = nid;
						}
						
						/* optimization */
						b->lfb = x;
						
						/* count used blocks NOT bytes */
						b->used += y;
						
						return (void*)(x * b->bsize + (uintptr_t)&b[1]);
					}
					
					/* x will be incremented by one ONCE more in our FOR loop */
					x += (y - 1);
					continue;
				}
			}
		}
	}
	
	return 0;
}

void k_heapBMFree(KHEAPBM *heap, void *ptr) {
	KHEAPBLOCKBM		*b;	
	uintptr_t				ptroff;
	u32				bi, x;
	u8				*bm;
	u8				id;
	u32				max;
	
	for (b = heap->fblock; b; b = b->next) {
		if ((uintptr_t)ptr > (uintptr_t)b && (uintptr_t)ptr < (uintptr_t)b + sizeof(KHEAPBLOCKBM) + b->size) {
			/* found block */
			ptroff = (uintptr_t)ptr - (uintptr_t)&b[1];  /* get offset to get block */
			/* block offset in BM */
			bi = ptroff / b->bsize;
			/* .. */
			bm = (u8*)&b[1];
			/* clear allocation */
			id = bm[bi];
			/* oddly.. GCC did not optimize this */
			max = b->size / b->bsize;
			for (x = bi; bm[x] == id && x < max; ++x) {
				bm[x] = 0;
			}
			/* update free block count */
			b->used -= x - bi;
			return;
		}
	}



}

// END OF CODE FROM https://wiki.osdev.org/User:Pancakes/BitmapHeapImplementation

// Wrapper functions
KHEAPBM heap_heap;

void *heap_malloc(u32 size) {
	return k_heapBMAlloc(&heap_heap, size);
}

void *heap_calloc(u32 size) {
	void *p = heap_malloc(size);
	memset(p, 0, size);
	return p;
}

void heap_free(void *ptr) {
	k_heapBMFree(&heap_heap, ptr);
}

void heap_init() {
	klog("HEAP", "initializing...");
	k_heapBMInit(&heap_heap);
	
	// Use memory from the memory map instead
	extern struct _sys_info sys_info;
	uintptr_t heap_start = sys_info.kernel_end;
	u32 heap_size = 2 * 1024 * 1024; // 2MB heap
	
	k_heapBMAddBlock(&heap_heap, heap_start, heap_size, 16);
	klogf("HEAP", "using %dMB at 0x%08x", heap_size / 1024 / 1024, heap_start);
	klog("HEAP", "done");
}

void *_kmalloc(u32 size, char *file, char *line) {
	void *ptr = heap_malloc(size);
	if (ptr == NULL) {
		__asm__ __volatile__ ("call panic_save_regs");
		panic_panic("could not malloc in kernel - memory out", file, line);
	}
	return ptr;
}
