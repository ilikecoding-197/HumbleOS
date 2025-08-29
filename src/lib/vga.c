// HumbleOS file: vga.c
// Purpose: VGA code file

#include <vga.h>
#include <port.h>

void vga_disable_blink() {
    u8 mode;

    (void)inb(0x3da);
    outb(0x3c0, 0x10);

    mode = inb(0x3c1);
    mode &= ~(1 << 3);

    (void)inb(0x3da);

    outb(0x3c0, 0x10);
    outb(0x3c0, mode);

    outb(0x3c0, 0x20);
}

void vga_set_color_palette(vga_rgb *palette, u8 amtColors) {
    outb(PALETTE_MASK, 0xFF);
    outb(PALETTE_WRITE, 0);

    for (int i = 0; i < amtColors; i++) {
        vga_rgb *color = palette + (i * sizeof(vga_rgb));

        outb(PALETTE_DATA, color->r); // Red
        outb(PALETTE_DATA, color->g); // Green
        outb(PALETTE_DATA, color->b); // Blue
    }
}

void vga_set_color_palette16(vga_rgb *palette) {
    vga_set_color_palette(palette, 16);
}

// Palettes
vga_rgb vga_ubuntu_palette[16] = {
    { 11, 13, 13 }, // #2E3436 - Black
    { 13, 25, 41 }, // #3465A4 - Blue
    { 19, 38, 1 }, // #4E9A06 - Green
    { 1, 38, 38 }, // #06989A - Cyan
    { 50, 0, 0 }, // #CC0000 - Red
    { 29, 20, 30 }, // #75507B - Magenta
    { 48, 40, 0 }, // #C4A000 - Yellow
    { 52, 53, 51 }, // #D3D7CF - Light Gray
    { 21, 21, 21 }, // #555753 - Gray
    { 28, 39, 51 }, // #729FCF - Light Blue
    { 34, 56, 13 }, // #8AE234 - Light Green
    { 13, 56, 56 }, // #34E2E2 - Light Cyan
    { 59, 10, 10 }, // #EF2929 - Light Red
    { 43, 31, 42 }, // #AD7FA8 - Light Magenta
    { 62, 58, 20 }, // #FCE94F - Light Yellow
    { 59, 59, 58 }, // #EEEEEC - White
};