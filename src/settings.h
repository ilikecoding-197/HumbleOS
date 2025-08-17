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
//#define USE_USER_MAIN // call user_main() after kernel initialization
#define PANIC_ON_TEST_FAIL // panic if a test fails, disabled by default

#endif
