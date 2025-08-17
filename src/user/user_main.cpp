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
    tui::dbox(Point(2, 1), Size(76, 23), LIGHTGRAY, BLUE);
}
