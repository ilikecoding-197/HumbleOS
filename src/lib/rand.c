// HumbleOS file: rand.c
// Purpose: C code for rand

#include <rand.h>
#include <cpuid.h>

int rand_randFromAsm;    // Random value from ASM
int rand_successFromAsm; // If ASM success

int rand() {
    // RDSEED
    if (cpuid_rdseed) {
        __asm__ __volatile__ ("call rand_rdseed"); // Call the ASM function

        if (rand_successFromAsm) return rand_randFromAsm; // Return random if success
        return -1;
    }

    return -1;
}