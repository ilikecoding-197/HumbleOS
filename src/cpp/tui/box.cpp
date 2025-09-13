/*
    box.cpp - box drawing code for tui

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

#include <tui/box.hpp>
#include <tui/helpers.hpp>
#include <ints.h>

extern "C" {
    #include <console.h>
}

// exposed tui namesoace
namespace tui {
    void box_chars(Point at, Size size) {
        int right = at.x + size.width - 1;
        int bottom = at.y + size.height - 1;

        // corners
        put_char_at(at.x, at.y, BOX_TL);
        put_char_at(right, at.y, BOX_TR);
        put_char_at(at.x, bottom, BOX_BL);
        put_char_at(right, bottom, BOX_BR);

        // horizontal lines
        for (int x = at.x+1; x < right; x++) {
            put_char_at(x, at.y, BOX_HR);
            put_char_at(x, bottom, BOX_HR);
        }

        // vertical lines
        for (int y = at.y+1; y < bottom; y++) {
            put_char_at(at.x, y, BOX_VR);
            put_char_at(right, y, BOX_VR);
        }
    }

    void box(Point at, Size size, u8 foreground, u8 background, char fill) {
        tui::fill(at, size, fill, (background << 4) | foreground);
        box_chars(at, size);
    }

    void dbox_chars(Point at, Size size) {
        int right = at.x + size.width - 1;
        int bottom = at.y + size.height - 1;

        // corners
        put_char_at(at.x, at.y, DBOX_TL);
        put_char_at(right, at.y, DBOX_TR);
        put_char_at(at.x, bottom, DBOX_BL);
        put_char_at(right, bottom, DBOX_BR);

        // horizontal lines
        for (int x = at.x+1; x < right; x++) {
            put_char_at(x, at.y, DBOX_HR);
            put_char_at(x, bottom, DBOX_HR);
        }

        // vertical lines
        for (int y = at.y+1; y < bottom; y++) {
            put_char_at(at.x, y, DBOX_VR);
            put_char_at(right, y, DBOX_VR);
        }
    }

    void dbox(Point at, Size size, u8 foreground, u8 background, char fill) {
        tui::fill(at, size, fill, (background << 4) | foreground);
        dbox_chars(at, size);
    }
}
