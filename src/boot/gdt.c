/*
	gdt.c - C code for GDT. called from boot.asm

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
#define GDT_ENTRIES 0xFF // amount of GDT entries

// GDT structure
struct GDT {
	u32 base;
	u32 limit;
	u8 access_byte;
	u8 flags;
};

// GDTR structure
struct GDTR {
	u16 limit;
	u32 base;
}__attribute((packed));

struct GDTR gdt_gdtr; // GDTR to GDT, used in boot.asm

u8 gdt_gdt[GDT_ENTRIES * 8]; // GDT bytes

// encode a GDT entry.
void gdt_encodeGdtEntry(
	u8 *target, // target (where to encode to)
	struct GDT source // source (where to encode from)
) {
    // encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;
    
    // encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
    
    // encode the access byte
    target[5] = source.access_byte;
    
    // encode the flags
    target[6] |= (source.flags << 4);
}


void gdt_gdt_c() {
	struct GDT entries[GDT_ENTRIES];

	// Null Descriptor
	entries[0].base = 0;
	entries[0].limit = 0;
	entries[0].access_byte = 0;
	entries[0].flags = 0;

	// Code Segment
	entries[1].base = 0;
	entries[1].limit = 0xFFFFF;
	entries[1].access_byte = 0x9A;
	entries[1].flags = 0xC;

	// Data Segment
	entries[2].base = 0;
	entries[2].limit = 0xFFFFF;
	entries[2].access_byte = 0x92;
	entries[2].flags = 0xC;

	// encode entries
	for (u8 i = 0; i < GDT_ENTRIES; i++) {
		gdt_encodeGdtEntry((u8*)&gdt_gdt[i*8], entries[i]);
	}

	// set GDTR
	gdt_gdtr.base = (u32)((void *)&gdt_gdt);
	gdt_gdtr.limit = GDT_ENTRIES * 8 - 1;
}
