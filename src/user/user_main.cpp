// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <ps2.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <tui/tui.hpp>

using namespace tui;

extern "C" {
    #include <time.h>
    #include <rand.h>
}

extern "C" void user_main() {
    // tui testing
    std::clear_screen();

    Menu menu;
    menu.amtItems = 3;
    menu.items = new MenuItem[3];
    menu.items[0].text = "Hello, world! (1)";
    menu.items[0].description = nullptr;
    menu.items[0].id = 1;
    menu.items[1].text = "Hello, world! (2)";
    menu.items[1].description = nullptr;
    menu.items[1].id = 2;
    menu.items[2].text = "Hello, world! (3)";
    menu.items[2].description = nullptr;
    menu.items[2].id = 3;
    
    uint id = menu_block(&menu, Point(2, 1), Size(76, 23));
    printf("You selected: %d\n", id);
}
