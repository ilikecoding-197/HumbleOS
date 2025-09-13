/*
    string.hpp - string class header

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

#pragma once

extern "C"
{
#include <string.h>
#include <heap.h>
}

namespace std
{
    // a string
    class string
    {
    private:
        char *data; // string data
        size_t len; // string length
        size_t capacity; // string capacity - what it can hold

    public:
        string(); // create empty string
        string(const char *str); // create string from c string
        string(const string &other); // copy another string
        ~string(); // destroy a string

        string &operator=(const string &other); // assign a string to another

        const char *c_str() const; // gets the c string
        size_t length() const; // gets the length of the string
        bool empty() const; // returns true if string is empty

        string operator+(const string &other) const; // cobmine this string with another
        string &operator+=(const string &other); // add another string to this one

        operator char *(); // char * -> string
        operator const char *() const; // const char * -> string
    };
}
