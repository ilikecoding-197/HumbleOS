// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <ps2.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <tui/tui.hpp>

extern "C" void user_main() {
    std::clear_screen();

    std::string hi("Hello World!");
    std::print(hi);

    klog("USER", "Hello World! This should only appear on the serial.");

    // tui testing

    tui::fill(tui::Point(0, 0), tui::Size(10, 10), 'A', 0x70);
}
