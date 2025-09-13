/*
    menu.hpp - tui menu header

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

#include <string.hpp>
#include <tui/point.hpp>
#include <tui/size.hpp>

namespace tui {
    // a menu item
    struct MenuItem {
        std::string text; // text of the menu item
        std::string description; // description - not used at the moment
        uint id; // id of the menu item - what is returned

        inline MenuItem() {};
    };

    // a menu
    struct Menu {
        uint amtItems; // amount of items
        MenuItem *items; // items

        inline Menu() {};
    };

    struct MenuContext {
        Menu *menu; // menu this context is for
        uint currentIndex; // the currently selected index
        uint lastIndex; // the last selected index
        tui::Point at; // where the menu is at
        tui::Size size; // the size of the menu
        bool isDone; // if the menu is done
        u8 prevContent[80*25*2]; // the previous content of the menu rectangle - put back on
                                 // screen at the end of thee menu
        u8 background; // background color
        u8 foreground; // foreground color
        uint keyEvent; // key event id

        inline MenuContext() {};
    };

    // open a menu to a context.
    void open_menu(MenuContext *context, Menu *menu, tui::Point at, tui::Size size, uint startingIndex = 0);

    // draw the menu from a context.
    void draw_menu(MenuContext *context);

    // get the ID of the selected item
    uint get_menu(MenuContext *context);

    // close the menu from a context (reloads the previous content).
    void close_menu(MenuContext *context);

    // open a menu and handle the drawing and updating and closing for you. returns the ID of the selected item.
    uint menu_block(Menu *menu, tui::Point at, tui::Size size, uint startingIndex = 0);
}