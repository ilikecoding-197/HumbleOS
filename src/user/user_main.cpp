// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <ps2.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <tui/tui.hpp>
#include "../apps/apps.hpp"

using namespace tui;

extern "C"
{
#include <time.h>
#include <rand.h>
#include <serial.h>
#include <events.h>
#include <ps2/ps2_keyboard.h>
#include <cpu.h>
}

extern "C" void user_main()
{
    // tui testing
    std::clear_screen();

    Menu menu;
    app_menu(&menu);

    while (1) {
        MenuContext context;

        open_menu(&context, &menu, Point(0, 0), Size(80, 25));

        while (!context.isDone) {
            cpu_hlt();
        }

        uint id = get_menu(&context);
        close_menu(&context);

        std::clear_screen();
        apps[id]->main();
        ps2_keyboard_flush();
        print("Press any key to continue...");
        getch();
        std::clear_screen();
    }
}
