// HumbleOS file: vga.h
// Purpose: VGA header file

#ifndef VGA_H
#define VGA_H

#include <ints.h>

// text mode info
#define VGA_TEXT_MODE_MEM 0xB8000
#define VGA_TEXT_MODE_WIDTH 80
#define VGA_TEXT_MODE_HEIGHT 25

// text mode colors
enum {
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE          = 1,
    VGA_COLOR_GREEN         = 2,
    VGA_COLOR_CYAN          = 3,
    VGA_COLOR_RED           = 4,
    VGA_COLOR_MAGENTA       = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GRAY    = 7,
    VGA_COLOR_DARK_GRAY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW        = 14,
    VGA_COLOR_WHITE         = 15
};

// Create a text mode attribute from a background and foreground
#define VGA_ATTR(fg, bg) ((u8)((fg) | ((bg) << 4)))

// A VGA RGB value for the DAC. 0-64, not 0-255
typedef struct {
    u8 r, g, b;
} vga_rgb;

// Disable VGA text mode blinking
void vga_disable_blink();

// DAC registers
#define PALETTE_MASK 0x3C6
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

// Set a VGA color pallete
void vga_set_color_palette(vga_rgb *palette, u8 amtColors);

// Set a VGA color pallette, with 16 colors
void vga_set_color_palette16(vga_rgb *palette);

// The color palette Ubuntu uses for the VGA colors. (16, text-mode)
extern vga_rgb vga_ubuntu_palette[16];

#endif // VGA_H