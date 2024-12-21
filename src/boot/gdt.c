// HumbleOS file: gdt.c
// Purpose: C code for GDT. Called from boot.asm.
// Mainly tooken from https://wiki.osdev.org/Global_Descriptor_Table; see that
// for GDT info.

#include <stdint.h>      // Needed for custom siZe ints
#define GDT_ENTRIES 0xFF // Amount of GDT entries

// GDT structure
struct GDT {
	uint32_t base;       // Base
	uint32_t limit;      // Limit
	uint8_t access_byte; // Access byte
	uint8_t flags;       // flag
};

// GDTR structure
struct GDTR {
	uint16_t limit; // Limit
	uint32_t base;  // base
}__attribute((packed));

struct GDTR gdt_gdtr; // GDTR to GDT, used in boot.asm

uint8_t gdt_gdt[GDT_ENTRIES * 8]; // GDT bytes

// Encode a GDT entry.
void gdt_encodeGdtEntry(
	uint8_t *target, // Target (where to encode to)
	struct GDT source // Source (where to encode from)
) {
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;
    
    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
    
    // Encode the access byte
    target[5] = source.access_byte;
    
    // Encode the flags
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

	// Encode entries
	for (int i = 0; i < GDT_ENTRIES; i++) {
		gdt_encodeGdtEntry((uint8_t*)&gdt_gdt[i*8], entries[i]); // Encode the segment.
	}

	// Set GDTR
	gdt_gdtr.base = (uint32_t)((void *)&gdt_gdt);
	gdt_gdtr.limit = GDT_ENTRIES * 8 - 1;
}
