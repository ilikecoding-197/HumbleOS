// HumbleOS file: string.hpp
// Purpose: C++ string class header

#pragma once

extern "C" {
    #include <string.h>
    #include <heap.h>
}

class string {
private:
    char* data;
    size_t len;
    size_t capacity;

public:
    // Constructors
    string();
    string(const char* str);
    string(const string& other);
    
    // Destructor
    ~string();
    
    // Assignment
    string& operator=(const string& other);
    
    // Basic operations
    const char* c_str() const;
    size_t length() const;
    bool empty() const;
    
    // Concatenation
    string operator+(const string& other) const;
    string& operator+=(const string& other);
    
    // Implicit conversion operators
    operator char*();
    operator const char*() const;
};
