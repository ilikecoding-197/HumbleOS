/*
    shell.cpp - shell main code

    Part of HumbleOS

    Copyright 2025 Thomas Shrader

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <user/shell.hpp>
#include <console.hpp>
#include <time.hpp>

// private namespace
namespace {
    // prompt - hardcoded for now
    const char *PROMPT = "> ";

    // fake getch - as the ps2 keyboard isnt working at the moment
    u16 fake_getch() {
        std::sleep(100);

        return 'A';
    }

    // read a line into a buf
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

// exposed shell namespace
namespace shell {
    // run the shell
    void run() {
        while (1) {
            // get input
            std::printf("%s", PROMPT);
            char buf[100];
            readline(buf, 100);
            std::print("\n");

            // just repeat it back to the user for now
            std::printf("You entered: %s\n", buf);
        }
    }
}