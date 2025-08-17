// HumbleOS file: ps2.c
// Purpose: PS2 drivers

#include <ps2/ps2.h>

int ps2_init() {
    klog("PS2", "initalization here...");

    return 0;
}

int ps2_detect_devices(ps2_device *devices, int max_devices) {
    klog("PS2", "dectection here...");

    return 0;
}