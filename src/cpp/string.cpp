#include <string.hpp>

string::string() : data(nullptr), len(0), capacity(0) {}

string::string(const char* str) {
    len = strlen(str);
    capacity = len + 1;
    data = (char*)heap_malloc(capacity);
    strcpy(data, str);
}

string::string(const string& other) {
    len = other.len;
    capacity = other.capacity;
    data = (char*)heap_malloc(capacity);
    strcpy(data, other.data);
}

string::~string() {
    if (data) heap_free(data);
}

string& string::operator=(const string& other) {
    if (this != &other) {
        if (data) heap_free(data);
        len = other.len;
        capacity = other.capacity;
        data = (char*)heap_malloc(capacity);
        strcpy(data, other.data);
    }
    return *this;
}

const char* string::c_str() const { 
    return data ? data : ""; 
}

size_t string::length() const { 
    return len; 
}

bool string::empty() const { 
    return len == 0; 
}

string string::operator+(const string& other) const {
    string result;
    result.len = len + other.len;
    result.capacity = result.len + 1;
    result.data = (char*)heap_malloc(result.capacity);
    strcpy(result.data, data ? data : "");
    strcat(result.data, other.data ? other.data : "");
    return result;
}

string& string::operator+=(const string& other) {
    *this = *this + other;
    return *this;
}

string::operator char*() {
    return data ? data : (char*)"";
}

string::operator const char*() const {
    return data ? data : "";
}
