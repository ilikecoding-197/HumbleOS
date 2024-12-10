#include <console.h>

void hello_main() {
    print("Hello, world!\n");
    print("Press any key to continue...");

    (void)getch();
}