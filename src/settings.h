// HumbleOS file: settings.h
// Purpose: Settings for the kernel.

#ifndef SETTINGS_H
#define SETTINGS_H

// Name, version, and release type
#define NAME "Humble OS"
#define VERSION "0.0.1"
#ifdef DEBUG
#define RELEASE_TYPE "Debug"
#else
#define RELEASE_TYPE "Release"
#endif


// Actual options, comment out to disable
// #define USE_USER_MAIN // call user_main() after kernel initialization
#define USE_TESTS // call tests_run() after kernel initialization (disabling will also not compile tests.c) itself
#define PANIC_ON_TEST_FAIL // panic if a test fails, disabled by default

#endif
