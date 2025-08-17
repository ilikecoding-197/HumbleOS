// HumbleOS file: menu.cpp
// Purpose: Code for TUI menu

#include <tui/menu.hpp>
#include <tui/helpers.hpp>
#include <tui/box.hpp>

extern "C" {
    #include <console.h>
}

extern "C" uint time_ms;

namespace tui {
    
    void open_menu(MenuContext *context, Menu *menu, tui::Point at, tui::Size size, uint startingIndex) {
        context->menu = menu;
        context->at = at;
        context->size = size;
        context->currentIndex = startingIndex;
        context->lastIndex = 9999;
        context->foreground = YELLOW;
        context->background = BLUE;
        context->isDone = false;
        
        tui::copy(context->prevContent, at, size);
    }

    void draw_menu(MenuContext *context) {
        if (context->isDone) return;

        if (context->currentIndex != context->lastIndex) {
            tui::box(context->at, context->size, context->foreground, context->background);
            uint oldCursorX = console_cursorX;
            uint oldCursorY = console_cursorY;
            uint oldColor = console_color;

            for (int i = 0; i < context->menu->amtItems; i++) {
                MenuItem *item = &context->menu->items[i];

                u8 foreground = context->foreground;
                u8 background = context->background;

                if (i == context->currentIndex) {
                    u8 temp = background;
                    background = foreground;
                    foreground = temp;
                }

                console_move_cursor(context->at.x+1, context->at.y+1+i);
                console_set_color((background << 4) |  foreground);
                print(item->text);

                while (console_cursorX < context->at.x+context->size.width-1) {
                    putchar(' ');
                }
            }

            console_move_cursor(oldCursorX, oldCursorY);
            console_set_color(oldColor);
        }

        context->lastIndex = context->currentIndex;
    }

    bool update_menu(MenuContext *context) {
        // for now just open the menu until the current uptime is 1 second;
        if (context->isDone) return false;

        if (time_ms >= 10000) {
            context->isDone = true;
            return true;
        }

        return false;
    }

    uint get_menu(MenuContext *context) {
        return context->menu->items[context->currentIndex].id;
    }

    void close_menu(MenuContext *context) {
        tui::paste(context->prevContent, context->at, context->size);
    }

    uint menu_block(Menu *menu, tui::Point at, tui::Size size, uint startingIndex) {
        MenuContext context;

        open_menu(&context, menu, at, size, startingIndex);

        while (!context.isDone) {
            draw_menu(&context);
            update_menu(&context);
            
            asm volatile("hlt");
        }

        uint id = get_menu(&context);
        close_menu(&context);
        return id;
    }
}