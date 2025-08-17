// HumbleOS file: helpers.cpp
// Purpose: Code for TUI helpers

#include <tui/helpers.hpp>

extern "C" {
    #include <console.h>
}

namespace tui {
    void copy(u8 *data, Point at, Size size) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                char character = get_char_at(at.x + x, at.y + y);
                char color = get_color_at(at.x + x, at.y + y);

                u16 pos = (y * size.width + x) * 2;

                data[pos] = character;
                data[pos + 1] = color;
            }
        }
    }

    void paste(const u8 *data, Point at, Size size) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                u16 pos = (y * size.width + x) * 2;

                char character = data[pos];
                char color = data[pos + 1];

                put_char_at(at.x + x, at.y + y, character);
                put_color_at(at.x + x, at.y + y, color);
            }
        }
    }

    void fill_character(Point at, Size size, char character) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                put_char_at(at.x + x, at.y + y, character);
            }
        }
    }

    void fill_color(Point at, Size size, char color) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                put_color_at(at.x + x, at.y + y, color);
            }
        }
    }

    void fill(Point at, Size size, char character, char color) {
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                put_char_at(at.x + x, at.y + y, character);
                put_color_at(at.x + x, at.y + y, color);
            }
        }
    }
}
