#include <console.h>

void info_main() {
    console_move_cursor(0, 24);
    console_set_color(GREEN);
    print("Press any key to continue... ");

    console_move_cursor(0, 0);
    console_set_color(LIGHTGRAY);
    print("HumbleOS is a basic hobby OS made by ilikecoding-197. Its not"
          "meant to be a whole functioning OS, all it is some fun apps"
          "made in plain C for fun!\n"
          "\n"
          "So what are you waiting for! Have fun! :)");

    getch();
}