#include <port.h>

void shutdown_main() {
    outw(0x604, 0x2000); // Only for QEMU
}