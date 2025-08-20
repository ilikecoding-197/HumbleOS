// HumbleOS file: apps.hpp
// Purpose: Defines a function to create a menu containing apps and a Exit item

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
