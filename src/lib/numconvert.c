// HumbleOS file: numconvert.c
// Purpose: Number conversion functions

#include <numconvert.h>
#include <string.h>

const char NUMCONVERT_BASE_CHARS[] = "0123456789ABCDEF";
const char NUMCONVERT_ERROR[] = "?";

unsigned int num_to_str(int num, char *buffer, unsigned int base, unsigned int pad) {
    // Check for invalid base
    if (base < 2 || base > 16) {
        strcpy(buffer, (char *) NUMCONVERT_ERROR);
        return 2;
    }

    // If pad is 0, make it one
    if (pad == 0) {
        pad = 1;
    }

    unsigned int i = 0;

    // Check sign
    if (num < 0) {
        num = -num;
        buffer[i++] = '-';
    }

    // Convert that number!
    int temp = num;
    unsigned int start = i;
    
    // Get digits in reverse order
    while (temp > 0) {
        buffer[i++] = NUMCONVERT_BASE_CHARS[temp % base];
        temp /= base;
    }
    
    // Fill with padding if needed
    while (i < pad) {
        buffer[i++] = '0';
    }

    // Reverse the digits
    unsigned int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    buffer[i] = '\0';
    return i + 1;
}