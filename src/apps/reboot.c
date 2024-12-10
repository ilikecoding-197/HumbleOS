#include <port.h>

void reboot_main() {
    outb(0x64, 0xFE);
}