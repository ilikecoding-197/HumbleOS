// HumbleOS file: string.h
// Purpose: Header file for string. Does not include documenation, you know how to use string.

#ifndef STRING_H
#define STRING_H

#include <string.h>
#include <stddef.h>
#include <ints.h>

char* strcpy( char* dest, const char* src );
char *strncpy( char *dest, const char *src, size_t count );
char *strcat( char *dest, const char *src );
char *strdup( const char *src );
char *strndup( const char *src, size_t size );
size_t strlen( const char* str );
int strcmp( const char* lhs, const char* rhs );
int strncmp( const char* lhs, const char* rhs, size_t count );
char* strchr( const char* str, int ch );
char* strrchr( const char* str, int ch );
size_t strspn( const char* dest, const char* src );
size_t strcspn( const char *dest, const char *src );
char *strpbrk( const char *dest, const char *breakset );
char* strstr( const char* str, const char* substr );
char* strtok( char* str, const char* delim );
void* memchr( const void* ptr, int ch, size_t count );
int memcmp( const void* lhs, const void* rhs, size_t count );
void *memset( void *dest, int ch, size_t count );
void* memcpy( void *dest, const void *src, size_t count );
void* memmove( void* dest, const void* src, size_t count );
void* memccpy( void* dest, const void* src, int c, size_t count );

#endif // STRING_H