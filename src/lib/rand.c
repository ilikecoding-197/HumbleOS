#include <rand.h>
#include <cpuid.h>

int rand_randFromAsm;
int rand_successFromAsm;

int rand() {
    if (cpuid_rdseed) {
        __asm__ __volatile__ ("call rand_rdseed");

        if (rand_successFromAsm) return rand_randFromAsm;
        return -1;
    }

    return -1;
}