/*
    vga.h - header for vga

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

// create a text mode attribute from a background and foreground
#define VGA_ATTR(fg, bg) ((u8)((fg) | ((bg) << 4)))

// a VGA RGB value for the DAC. 0-64, not 0-255
typedef struct {
    u8 r, g, b;
} vga_rgb;

// disable VGA text mode blinking
void vga_disable_blink();

// DAC registers
#define PALETTE_MASK 0x3C6
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

// set a VGA color pallete
void vga_set_color_palette(vga_rgb *palette, u8 amtColors);

// set a VGA color pallette, with 16 colors
void vga_set_color_palette16(vga_rgb *palette);

// the color palette Ubuntu uses for the VGA colors. (16, text-mode)
extern vga_rgb vga_ubuntu_palette[16];

#endif /* VGA_H */