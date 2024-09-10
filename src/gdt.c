#include <stdint.h>
#define GDT_ENTRIES 256

// GDT structure
struct GDT {
	uint32_t base;
	uint32_t limit;
	uint8_t access_byte;
	uint8_t flags;
};

// GDTR structure
struct GDTR {
	uint16_t limit;
	uint32_t base;
}__attribute((packed));

struct GDTR gdtr;

uint8_t gdt[GDT_ENTRIES * 8];

void encodeGdtEntry(uint8_t *target, struct GDT source)
{
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


void gdt_c() {
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

	// Encode entries=
	for (int i = 0; i < GDT_ENTRIES; i++) {
		encodeGdtEntry((uint8_t*)((&gdt) + (i*8)), entries[i]);
	}

	// Set GDTR
	gdtr.base = (uint32_t)((void *)&gdt);
	gdtr.limit = GDT_ENTRIES * 8;
}
