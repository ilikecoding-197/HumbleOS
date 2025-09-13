// HumbleOS file: helpers.hpp
// Purpose: Header file for TUI helpers

#pragma once

#include <ints.h>
#include <tui/point.hpp>
#include <tui/size.hpp>

namespace tui {
    // copy a certain portion of the screen.
    void copy(u8 *data, Point at, Size size);

    // paste a certain portion of the screen.
    void paste(u8 *data, Point at, Size size);

    // fill a certain portion of the screen with a character.
    void fill_character(Point at, Size size, char character);

    // fill a certain portion of the screen with a color.
    void fill_color(Point at, Size size, char color);

    // fill a certain portion of the screen with a character and color.
    void fill(Point at, Size size, char character, char color);
}