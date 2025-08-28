// HumbleOS file: info.h
// Purpose: Name, version, and build information

#ifndef INFO_H
#define INFO_H

#define NAME "Humble OS"
#define VERSION "0.0.1"
#ifdef DEBUG
#define RELEASE_TYPE "Debug"
#else
#define RELEASE_TYPE "Release"
#endif

#endif // INFO_H