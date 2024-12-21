#include <console.h>
#include "../settings.h"

char INFO_INFO_STR[] =
"Welcome to HumbleOS! HumbleOS is a basic OSDev project made by ilikecoding-197\n"
"that is not supposed to be a real OS, instead just a collection of fun apps.\n"
"So, what are you waiting for! Go and have some fun! :)\n";

void info_main() {
    console_move_cursor(0, 0);
    console_set_color(LIGHTGRAY);
    print(INFO_INFO_STR);

    print("\n\n");
    console_set_color(GREEN);
    print("Version ");
    console_set_color(LIGHTGRAY);
    print(VERSION);

    console_move_cursor(0, 24);
    console_set_color(GREEN);
    print("Press any key to continue... ");
    console_set_color(LIGHTGRAY);

    getch();
}