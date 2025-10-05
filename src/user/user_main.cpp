// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <tui/tui.hpp>
#include "../info.h"
#include <humbleoslogo.h>
#include <user/shell.hpp>
#include <sys_info.hpp>

void banner()
{
    /* banner (bunch of random numbers time!) */
    std::clear_screen();
    tui::fill_color(tui::Point(0, 0), tui::Size(80, 9), VGA_ATTR(WHITE, LIGHTBLUE));
    std::set_color(VGA_ATTR(WHITE, LIGHTBLUE));
    for (int i = 0; i < 5; i++)
    {
        char *line = HUMBLE_OS_BANNER[i];

        std::move_cursor(1, 1 + i);
        std::print(line);
    }
    std::move_cursor(50, 1);
    std::print("v" VERSION);
    std::move_cursor(50, 2);
    std::print(RELEASE_TYPE);

    std::move_cursor(1, 7);
    std::printf("Booted in %dms (%ds) %dMB total memory", sys::boot_time, sys::boot_time / 1000,
        sys::mem_amt / (1024 * 1024));
    std::move_cursor(0, 10);

    /* palette test (8x2 blocks) */
    for (int i = 0; i < 16; i++)
    {
        int x = 50 + (i % 8) * 2;
        int y = 4 + (i / 8);

        std::set_color((i << 4) | 0x0F); /* background = i, foreground = white */
        std::move_cursor(x, y);
        std::print("  "); /* two spaces = a “block” */
    }

    std::set_color(0x0F); /* normal */
}


extern "C" void user_main()
{
    banner();
    std::move_cursor(0, 10);
    
    std::printf("Hello and welcome to HumbleOS!\n");

    //shell::run();
}
