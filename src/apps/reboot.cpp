// App: reboot
// Description: Reboots the system.
// Author: Thomas Shrader

#include "app.hpp"
#include <ps2.hpp>

static void app_main(void) {
    ps2::restart();
}

App reboot_app = {
    "Reboot", "Reboots the system.", "Thomas Shrader", app_main
};