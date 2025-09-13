/*
    helpers.cpp - tui helpers

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

#include <tui/helpers.hpp>

extern "C" {
    #include <console.h>
}

namespace tui {
    void copy(u8 *data, Point at, Size size) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                char character = get_char_at(at.x + x, at.y + y);
                char color = get_color_at(at.x + x, at.y + y);

                u16 pos = (y * size.width + x) * 2;

                data[pos] = character;
                data[pos + 1] = color;
            }
        }
    }

    void paste(u8 *data, Point at, Size size) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                u16 pos = (y * size.width + x) * 2;

                char character = data[pos];
                char color = data[pos + 1];

                put_char_at(at.x + x, at.y + y, character);
                put_color_at(at.x + x, at.y + y, color);
            }
        }
    }

    void fill_character(Point at, Size size, char character) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                put_char_at(at.x + x, at.y + y, character);
            }
        }
    }

    void fill_color(Point at, Size size, char color) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                put_color_at(at.x + x, at.y + y, color);
            }
        }
    }

    void fill(Point at, Size size, char character, char color) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                put_char_at(at.x + x, at.y + y, character);
                put_color_at(at.x + x, at.y + y, color);
            }
        }
    }
}
