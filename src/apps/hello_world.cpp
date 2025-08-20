// App: hello_world
// Description: Prints "Hello, world!" to the screen.
// Author: Thomas Shrader

#include "app.hpp"
#include <console.hpp>

static void app_main(void) {
    std::print("Hello, world!");
}

App hello_world_app = {
    "Hello, world!", "Prints \"Hello, world!\" to the screen.", "Thomas Shrader", app_main
};