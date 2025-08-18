// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <ps2.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <tui/tui.hpp>

using namespace tui;

extern "C"
{
#include <time.h>
#include <rand.h>
#include <serial.h>
#include <events.h>
}

void timer_event(Event *e)
{
    if (e->type != EVENT_TIMER)
        return;

    console_move_cursor(0, 24);
    printf("                                     ");
    console_move_cursor(0, 24);
    printf("%d", e->event.timer_ms);
}

extern "C" void user_main()
{
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

    uint timeEvent = add_event(timer_event, NULL);
    while (true)
    {
        MenuContext context;

        open_menu(&context, &menu, Point(2, 1), Size(76, 23));

        while (!context.isDone);

        uint id = get_menu(&context);
        close_menu(&context);

        console_move_cursor(0, 0);
        std::clear_screen();
        printf("You selected: %d. Press any key to go again.\n", id);
        getch();
    }
}
