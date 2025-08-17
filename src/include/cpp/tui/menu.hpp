// HumbleOS file: menu.hpp
// Purpose: Menus for TUI

#pragma once

#include <string.hpp>
#include <tui/point.hpp>
#include <tui/size.hpp>

namespace tui {
    struct MenuItem {
        std::string text;
        std::string description;
        uint id;

        inline MenuItem() {};
    };

    struct Menu {
        uint amtItems;
        MenuItem *items;

        inline Menu() {};
    };

    struct MenuContext {
        Menu *menu;
        uint currentIndex;
        uint lastIndex;
        tui::Point at;
        tui::Size size;
        bool isDone;
        u8 prevContent[80*25*2];
        u8 background, foreground;

        inline MenuContext() {};
    };

    // Open a menu to a context.
    void open_menu(MenuContext *context, Menu *menu, tui::Point at, tui::Size size, uint startingIndex = 0);

    // Draw the menu from a context.
    void draw_menu(MenuContext *context);

    // Update the menu from a context (handle input). Returns true if the menu is done.
    bool update_menu(MenuContext *context);

    // Get the ID of the selected item
    uint get_menu(MenuContext *context);

    // Close the menu from a context (reloads the previous content).
    void close_menu(MenuContext *context);

    // Open a menu and handle the drawing and updating and closing for you. Returns the ID of the selected item.
    uint menu_block(Menu *menu, tui::Point at, tui::Size size, uint startingIndex = 0);
}