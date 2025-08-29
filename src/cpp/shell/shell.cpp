// HumbleOS file: shell.cpp
// Purpose: Shell main code

#include <user/shell.h>
#include <console.hpp>
#include <time.hpp>

namespace {
    const char *PROMPT = "> ";

    u16 fake_getch() {
        std::sleep(100);

        return 'A';
    }

    void readline(char *buf, uint maxLength) {
        uint i = 0;
        u16 ch = '\0';

        while ((ch = ::fake_getch()) != '\n') {
            buf[i] = ch;
            i++;
            std::printf("%c", ch);
        }
    }
}

namespace shell {
    void run() {
        while (1) {
            std::printf("%s", PROMPT);
            
            char buf[100];
            readline(buf, 100);
            std::print("\n");

            std::printf("You entered: %s\n", buf);
        }
    }
}