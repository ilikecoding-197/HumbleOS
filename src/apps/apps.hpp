/*
    apps.hpp - defines app menu for user_main

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

#include "app.hpp"
#include <tui/menu.hpp>

#define APP_EXTERN(name) extern App name##_app;

APP_EXTERN(hello_world)
APP_EXTERN(reboot)

App* apps[] = { &hello_world_app, &reboot_app };
constexpr uint APP_COUNT = sizeof(apps) / sizeof(apps[0]);

void app_menu(tui::Menu* menu) {
    menu->amtItems = APP_COUNT;

    // static allocation for items
    static tui::MenuItem static_items[APP_COUNT];
    menu->items = static_items;

    for (uint i = 0; i < APP_COUNT; i++) {
        menu->items[i].text = apps[i]->name;
        menu->items[i].description = apps[i]->desc;
        menu->items[i].id = i;
    }
}
