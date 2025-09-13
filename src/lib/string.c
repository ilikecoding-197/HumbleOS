/*
    string.c - STRIING FUNCTIONS

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

#include <string.h>
#include <stddef.h>
#include <ints.h>
#include <heap.h>

// All function defs (not the code) are taken from https://en.cppreference.com/w/c/header/string.
// Removed restrict so I could call this from C++ code

char *strcpy(char *dest, const char *src)
{
    char *result = dest;
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return result;
}

char *strncpy(char *dest, const char *src, size_t count)
{
    char *result = dest;
    while (count > 0 && *src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
        count--;
    }
    *dest = '\0';
    return result;
}

char *strcat(char *dest, const char *src)
{
    char *result = dest;
    while (*dest != '\0')
    {
        dest++;
    }
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return result;
}

char *strdup(const char *src)
{
    size_t length = strlen(src) + 1;
    char *result = (char *)heap_malloc(length);
    if (result == NULL)
        return NULL;
    strcpy(result, src);
    return result;
}

char *strndup(const char *src, size_t size)
{
    size_t length = strlen(src) + 1;
    if (length > size)
        length = size;
    char *result = (char *)heap_malloc(length);
    if (result == NULL)
        return NULL;
    strncpy(result, src, size);
    return result;
}

size_t strlen(const char *str)
{
    size_t length = 0;
    while (*str != '\0')
    {
        length++;
        str++;
    }
    return length;
}

int strcmp(const char *lhs, const char *rhs)
{
    while (*lhs != '\0' && *rhs != '\0')
    {
        if (*lhs < *rhs)
            return -1;
        if (*lhs > *rhs)
            return 1;
        lhs++;
        rhs++;
    }
    if (*lhs == '\0' && *rhs == '\0')
        return 0;
    if (*lhs == '\0')
        return -1;
    return 1;
}

int strncmp(const char *lhs, const char *rhs, size_t count)
{
    while (*lhs != '\0' && *rhs != '\0' && count > 0)
    {
        if (*lhs < *rhs)
            return -1;
        if (*lhs > *rhs)
            return 1;
        lhs++;
        rhs++;
        count--;
    }
    if (count == 0)
        return 0;
    if (*lhs == '\0' && *rhs == '\0')
        return 0;
    if (*lhs == '\0')
        return -1;
    return 1;
}

char *strchr(const char *str, int ch)
{
    while (*str != '\0')
    {
        if (*str == ch)
            return (char *)str;
        str++;
    }
    return NULL;
}

char *strrchr(const char *str, int ch)
{
    char *result = NULL;
    while (*str != '\0')
    {
        if (*str == ch)
            result = (char *)str;
        str++;
    }
    return result;
}

size_t strspn(const char *dest, const char *src)
{
    size_t length = 0;
    while (*dest != '\0')
    {
        if (strchr(src, *dest) == NULL)
            return length;
        length++;
        dest++;
    }
    return length;
}

size_t strcspn(const char *dest, const char *src)
{
    size_t length = 0;
    while (*dest != '\0')
    {
        if (strchr(src, *dest) != NULL)
            return length;
        length++;
        dest++;
    }
    return length;
}

char *strpbrk(const char *dest, const char *breakset)
{
    while (*dest != '\0')
    {
        if (strchr(breakset, *dest) != NULL)
            return (char *)dest;
        dest++;
    }
    return NULL;
}

char *strstr(const char *str, const char *substr)
{
    while (*str != '\0')
    {
        if (strncmp(str, substr, strlen(substr)) == 0)
            return (char *)str;
        str++;
    }
    return NULL;
}

char *strtok(char *str, const char *delim)
{
    static char *last_str = NULL;
    if (str != NULL)
        last_str = str;
    if (last_str == NULL)
        return NULL;
    char *result = last_str;
    while (*last_str != '\0')
    {
        if (strchr(delim, *last_str) != NULL)
        {
            *last_str = '\0';
            last_str++;
            return result;
        }
        last_str++;
    }
    last_str = NULL;
    return result;
}

void *memchr(const void *ptr, int ch, size_t count)
{
    char *result = (char *)ptr;
    while (count > 0)
    {
        if (*result == ch)
            return result;
        result++;
        count--;
    }
    return NULL;
}

int memcmp(const void *lhs, const void *rhs, size_t count)
{
    char *l = (char *)lhs;
    char *r = (char *)rhs;
    while (count > 0)
    {
        if (*l < *r)
            return -1;
        if (*l > *r)
            return 1;
        l++;
        r++;
        count--;
    }
    return 0;
}

void *memset(void *dest, int ch, size_t count)
{
    char *result = (char *)dest;
    while (count > 0)
    {
        *result = ch;
        result++;
        count--;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *result = (char *)dest;
    char *s = (char *)src;
    while (count > 0)
    {
        *result = *s;
        result++;
        s++;
        count--;
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
    char *result = (char *)dest;
    char *s = (char *)src;
    if (dest < src)
    {
        while (count > 0)
        {
            *result = *s;
            result++;
            s++;
            count--;
        }
    }
    else
    {
        result += count;
        s += count;
        while (count > 0)
        {
            *result = *s;
            result--;
            s--;
            count--;
        }
    }
    return dest;
}

void *memccpy(void *dest, const void *src, int c, size_t count)
{
    char *result = (char *)dest;
    char *s = (char *)src;
    while (count > 0)
    {
        *result = *s;
        if (*result == c)
            return result + 1;
        result++;
        s++;
        count--;
    }
    return NULL;
}