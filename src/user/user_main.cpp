// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <ps2.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <tui/tui.hpp>
#include "../apps/apps.hpp"
#include "../info.h"
#include <humbleoslogo.h>
#include <user/shell.hpp>

using namespace tui;

extern "C"
{
#include <rand.h>
#include <serial.h>
#include <events.h>
#include <ps2/ps2_keyboard.h>
#include <cpu.h>
#include <sys_info.h>
}

void banner()
{
    /* banner (bunch of random numbers time!) */
    std::clear_screen();
    tui::fill_color(Point(0, 0), Size(80, 9), VGA_ATTR(WHITE, LIGHTBLUE));
    std::set_color(VGA_ATTR(WHITE, LIGHTBLUE));
    for (int i = 0; i < 5; i++)
    {
        char *line = HUMBLE_OS_BANNER[i];

        ::console_move_cursor(1, 1 + i);
        std::print(line);
    }
    ::console_move_cursor(50, 1);
    ::print("v" VERSION);
    ::console_move_cursor(50, 2);
    ::print(RELEASE_TYPE);

    ::console_move_cursor(1, 7);
    std::printf("Booted in %dms (%ds) %dMB total memory", ::sys_info.boot_time, ::sys_info.boot_time / 1000,
        ::sys_info.mem_amt / (1024 * 1024));
    ::console_move_cursor(0, 10);

    /* palette test (8x2 blocks) */
    for (int i = 0; i < 16; i++)
    {
        int x = 50 + (i % 8) * 2;
        int y = 4 + (i / 8);

        std::set_color((i << 4) | 0x0F); /* background = i, foreground = white */
        ::console_move_cursor(x, y);
        std::print("  "); /* two spaces = a “block” */
    }

    std::set_color(0x0F); /* normal */
}


extern "C" void user_main()
{
    banner();
    ::console_move_cursor(0, 10);
    
    shell::run();
}
