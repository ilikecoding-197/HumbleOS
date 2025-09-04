/*
    build_info.h - get build info at compile time

    you can skip any of the values by defining SKIP_[value_name]. for example, to skip
    COMPILER_NAME, you can do SKIP_COMPILER_NAME.

    Copyright 2025 Thomas Shrader

    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the “Software”), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify,
    merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BUILD_INFO_H
#define BUILD_INFO_H

// helpers
#define _STR(n) #n
#define STR(n) _STR(n)

// COMPILER_NAME - the name of the compiler
// possible values: "clang", "gcc", "msvc", "unknown"
#ifndef SKIP_COMPILER_NAME
    #if defined(__clang__)
        #define COMPILER_NAME "clang"
    #elif defined(__GNUC__) || defined (__GNUG__)
        #define COMPILER_NAME "gcc"
    #elif defined(_MSC_VER)
        #define COMPILER_NAME "msvc"
    #else
        #define COMPILER_NAME "unknown"
    #endif // defined(__clang__)
#endif // SKIP_COMPILER_NAME

// COMPILER_VERSION_[MAJOR/MINOR/PATCH] and COMPILER_VERSION
// COMPILER_VERSION - always avaiable. on gcc and clang, in the format "v[major].
// [minor].[patch]". in MSVC, the version number as a string. if the compiler is
// unknown, its "v1.0.0".
// COMPILER_VERSION_[MAJOR/MINOR/PATCH] - only avaiable on gcc and clang (and
// unknown compilers). seperates the major, minor, and patch versions.
#ifndef SKIP_COMPILER_VERSION
    #if defined(__clang__)
        #define COMPILER_VERSION_MAJOR __clang_major__
        #define COMPILER_VERSION_MINOR __clang_minor__
        #define COMPILER_VERSION_PATCH __clang_patchlevel__
        #define COMPILER_VERSION "v" STR(__clang_major__) "." STR(__clang_minor__) "." STR(__clang_patchlevel__)
    #elif defined(__GNUC__) || defined (__GNUG__)
        #define COMPILER_VERSION_MAJOR __GNUC__
        #define COMPILER_VERSION_MINOR __GNUC_MINOR__
        #define COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
        #define COMPILER_VERSION "v" STR(__GNUC__) "." STR(__GNUC_MINOR__) "." STR(__GNUC_PATCHLEVEL__)
    #elif defined(_MSC_VER)
        #define COMPILER_VERSION STR(_MSC_VER)
    #else
        #define COMPILER_VERSION_MAJOR 1
        #define COMPILER_VERSION_MINOR 0
        #define COMPILER_VERSION_PATCH 0
        #define COMPILER_VERSION "v1.0.0"
    #endif
#endif // SKIP_COMPILER_VERSION

// COMPILER_HUMAN_READABLE
// Provides a human readable text, describing the compiler.
#ifndef SKIP_COMPILER_HUMAN_READABLE
    #if defined(__clang__)
        #define COMPILER_HUMAN_READABLE "Clang " STR(__clang_major__) "." STR(__clang_minor__) "." STR(__clang_patchlevel__)
    #elif defined(__GNUC__) || defined (__GNUG__)
        #define COMPILER_HUMAN_READABLE "GCC " STR(__GNUC__) "." STR(__GNUC_MINOR__) "." STR(__GNUC_PATCHLEVEL__)
    #elif defined(_MSC_VER)
        #if _MSC_VER >= 1930
            #define _VS_VERSION_STR_ "VS 2022"
        #elif _MSC_VER >= 1920
            #define _VS_VERSION_STR_ "VS 2019"
        #elif _MSC_VER >= 1910
            #define _VS_VERSION_STR_ "VS 2017"
        #elif _MSC_VER == 1900
            #define _VS_VERSION_STR_ "VS 2015"
        #elif _MSC_VER == 1800
            #define _VS_VERSION_STR_ "VS 2013"
        #elif _MSC_VER == 1700
            #define _VS_VERSION_STR_ "VS 2012"
        #elif _MSC_VER == 1600
            #define _VS_VERSION_STR_ "VS 2010"
        #elif _MSC_VER == 1500
            #define _VS_VERSION_STR_ "VS 2008"
        #elif _MSC_VER == 1400
            #define _VS_VERSION_STR_ "VS 2005"
        #elif _MSC_VER == 1310
            #define _VS_VERSION_STR_ "VS 2005 (old)"
        #elif _MSC_VER == 1200
            #define _VS_VERSION_STR_ "VS .NET 2003"
        #elif _MSC_VER == 1100
            #define _VS_VERSION_STR_ "VS .NET 2002"
        #elif _MSC_VER == 1000
            #define _VS_VERSION_STR_ "VS 7.0"
        #elif _MSC_VER == 900
            #define _VS_VERSION_STR_ "VS 6.0"
        #elif _MSC_VER == 800
            #define _VS_VERSION_STR_ "VS 97"
        #else
            #define _VS_VERSION_STR_ "VS unknown"
        #endif

        #define COMPILER_HUMAN_READABLE "MSVC " STR(_MSC_VER) " (" _VS_VERSION_STR_ ")"
    #else
        #define COMPILER_HUMAN_READABLE "Unknown Compiler"
    #endif
#endif // SKIP_COMPILER_HUMAN_READABLE

// COMPILER_OPTIMIZAION - string containing the current compilers optimization
#ifndef SKIP_COMPILER_OPTIMIZAION
    #if defined(__clang__) || defined(__GNUC__) || defined (__GNUG__)
        #if defined(__OPTIMIZE__)
            #if defined(__OPTIMIZE_SIZE__)
                #define COMPILER_OPTIMIZAION "Size optimized (-Os)"
            #else
                #define COMPILER_OPTIMIZAION "Optimized (-O1, -O2, -O3, etc.)"
            #endif
        #else
            #define COMPILER_OPTIMIZAION "No optimization (-O0)"
        #endif
    #else
        #define COMPILER_OPTIMIZAION "Unknown"
    #endif
#endif // SKIP_COMPILER_OPTIMIZAION

#endif // BUILD_INFO_H