/*
    box.hpp - tui box header

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

#pragma once

#include <tui/point.hpp>
#include <tui/size.hpp>
#include <ints.h>

// box characters
#define BOX_TL 0xDA // top-left corner
#define BOX_TR 0xBF // top-right corner
#define BOX_BL 0xC0 // bottom-left corner
#define BOX_BR 0xD9 // bottom-right corner
#define BOX_HR 0xC4 // horizonal line
#define BOX_VR 0xB3 // vertical line

// double
#define DBOX_TL 0xC9 // top-left corner
#define DBOX_TR 0xBB // top-right corner
#define DBOX_BL 0xC8 // bottom-left corner
#define DBOX_BR 0xBC // bottom-right corner
#define DBOX_HR 0xCD // horizonal line
#define DBOX_VR 0xBA // vertical line

namespace tui {
    // draw the box characters at a specfic area on the screen.
    void box_chars(Point at, Size size);
    
    // draw a box at a specfic area on the screen, with the specified colors,
    // and optionally a fill character (for non-box-parts.)
    void box(Point at, Size size, u8 foreground, u8 background, char fill = ' ');

    // draw the double box characters at a specfic area on the screen.
    void dbox_chars(Point at, Size size);
    
    // draw a double box at a specfic area on the screen, with the specified colors,
    // and optionally a fill character (for non-box-parts.)
    void dbox(Point at, Size size, u8 foreground, u8 background, char fill = ' ');
}