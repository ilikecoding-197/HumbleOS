#pragma once

extern "C" {
    #include <ps2/ps2_controller.h>
}

namespace ps2 {
    inline void restart() {
        ps2_restart_system();
    }
}