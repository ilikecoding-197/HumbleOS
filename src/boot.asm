global start
extern kernel_main
section .text
bits 32
start:
    call kernel_main
    hlt
