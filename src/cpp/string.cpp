/*
    string.cpp - string class

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



#include <string.hpp>

namespace std
{
    string::string() : data(nullptr), len(0), capacity(0) {}

    string::string(const char *str)
    {
        len = strlen(str);
        capacity = len + 1;
        data = (char *)heap_malloc(capacity);
        strcpy(data, str);
    }

    string::string(const string &other)
    {
        len = other.len;
        capacity = other.capacity;
        data = (char *)heap_malloc(capacity);
        strcpy(data, other.data);
    }

    string::~string()
    {
        if (data)
            heap_free(data);
    }

    string &string::operator=(const string &other)
    {
        if (this != &other)
        {
            if (data)
                heap_free(data);
            len = other.len;
            capacity = other.capacity;
            data = (char *)heap_malloc(capacity);
            strcpy(data, other.data);
        }
        return *this;
    }

    const char *string::c_str() const
    {
        return data ? data : "";
    }

    size_t string::length() const
    {
        return len;
    }

    bool string::empty() const
    {
        return len == 0;
    }

    string string::operator+(const string &other) const
    {
        string result;
        result.len = len + other.len;
        result.capacity = result.len + 1;
        result.data = (char *)heap_malloc(result.capacity);
        strcpy(result.data, data ? data : "");
        strcat(result.data, other.data ? other.data : "");
        return result;
    }

    string &string::operator+=(const string &other)
    {
        *this = *this + other;
        return *this;
    }

    string::operator char *()
    {
        return data ? data : (char *)"";
    }

    string::operator const char *() const
    {
        return data ? data : "";
    }

}
