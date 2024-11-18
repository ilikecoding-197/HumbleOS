#ifndef PANIC_H
#define PANIC_H

#define PANIC(msg) __asm__ __volatile__ ("call panic_save_regs"); panic_panic(msg);
void panic_panic(char *msg);

#endif // PANIC_H
