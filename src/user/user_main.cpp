// HumbleOS file: user_main.cpp
// Purpose: The main interface of HumbleOS, using C++ of course.

#include <console.hpp>
#include <ps2.hpp>
#include <string.hpp>

extern "C" void user_main() {
    console::clear_screen();

    string hi("Hello World!");
    console::print(hi);
}
