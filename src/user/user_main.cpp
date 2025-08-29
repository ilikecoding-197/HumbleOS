// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <ps2.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <tui/tui.hpp>
#include "../apps/apps.hpp"
#include "../info.h"

using namespace tui;

extern "C"
{
#include <rand.h>
#include <serial.h>
#include <events.h>
#include <ps2/ps2_keyboard.h>
#include <cpu.h>
}

// acsii art
char *HUMBLE_OS_BANNER[] = {
    " _   _                 _     _       ___  ____  ",
    "| | | |_   _ _ __ ___ | |__ | | ___ / _ \\/ ___| ",
    "| |_| | | | | '_ ` _ \\| '_ \\| |/ _ \\ | | \\___ \\ ",
    "|  _  | |_| | | | | | | |_) | |  __/ |_| |___) |",
    "|_| |_|\\__,_|_| |_| |_|_.__/|_|\\___|\\___/|____/ ",
};

void banner()
{
    // banner (bunch of random numbers time!)
    std::clear_screen();
    tui::fill_color(Point(0, 0), Size(80, 7), 0x1F);
    std::set_color(0x16); // white on blue
    for (int i = 0; i < 5; i++)
    {
        char *line = HUMBLE_OS_BANNER[i];

        ::console_move_cursor(15, 1 + i);
        std::print(line);
    }
    ::console_move_cursor(64, 1);
    ::print("v" VERSION);
    ::console_move_cursor(64, 2);
    ::print(" " RELEASE_TYPE);
    std::set_color(0x07); // normal
}

extern "C" void user_main()
{
    // banner();

    // Color pallette test
    for (int bg = 0; bg < 16; bg++)
    {
        for (int fg = 0; fg < 16; fg++)
        {
            u8 attr = (bg << 4) | fg;
            std::set_color(attr);
            std::print("A");
        }
        std::set_color(0);
        std::print("\n");
    }
    
    std::set_color(VGA_ATTR(LIGHTGRAY, BLACK));
}
