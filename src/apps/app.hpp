// HumbleOS file: app.hpp
// Purpose: App struct

#ifndef APP_H
#define APP_H

typedef void (*AppMain)();

typedef struct {
    const char *name;
    const char *desc;
    const char *author;
    AppMain main;
} App;

#endif // APP_H