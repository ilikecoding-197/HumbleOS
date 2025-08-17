// HumbleOS file: box.hpp
// Purpose: Defines functions for box drawing

#pragma once

#include <tui/point.hpp>
#include <tui/size.hpp>
#include <ints.h>

// Box characters
#define BOX_TL 0xDA // top-left corner
#define BOX_TR 0xBF // top-right corner
#define BOX_BL 0xC0 // bottom-left corner
#define BOX_BR 0xD9 // bottom-right corner
#define BOX_HR 0xC4 // horizonal line
#define BOX_VR 0xB3 // vertical line
#define DBOX_TL 0xC9 // top-left corner
#define DBOX_TR 0xBB // top-right corner
#define DBOX_BL 0xC8 // bottom-left corner
#define DBOX_BR 0xBC // bottom-right corner
#define DBOX_HR 0xCD // horizonal line
#define DBOX_VR 0xBA // vertical line

namespace tui {
    /// @brief Draw the box characters at a specfic area on the screen.
    void box_chars(Point at, Size size);
    
    /// @brief Draw a box at a specfic area on the screen, with the specified colors,
    ///        and optionally a fill character (for non-box-parts.)
    void box(Point at, Size size, u8 foreground, u8 background, char fill = ' ');

    /// @brief Draw the double box characters at a specfic area on the screen.
    void dbox_chars(Point at, Size size);
    
    /// @brief Draw a double box at a specfic area on the screen, with the specified colors,
    ///        and optionally a fill character (for non-box-parts.)
    void dbox(Point at, Size size, u8 foreground, u8 background, char fill = ' ');
}