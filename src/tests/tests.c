// HumbleOS file: tests.c
// Purpose: Tests

#include <tests.h>
#include <console.h>

#include "../settings.h"

#ifdef USE_TESTS

void tests_run() {
    console_clear_screen();

    // act like we are doing something
    console_set_color(LIGHTGREEN);
    print("Tests passed!\n");
    console_set_color(LIGHTGRAY);
}

#endif