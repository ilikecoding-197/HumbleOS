; HumbleOS file: cpuid.asm
; Purpose: ASM code for cpuid

[bits 32]

section .bss
global _cpuid_supported

_cpuid_supported: resb 1

section .text

global _cpuid_check

; Check if CPUID is supported (most taken from https://wiki.osdev.org/CPUID)
_cpuid_check:
    pushad                               ; Save registers
    pushfd                               ; Save EFLAGS
    pushfd                               ; Store EFLAGS
    xor dword [esp],0x00200000           ; Invert the ID bit in stored EFLAGS
    popfd                                ; Load stored EFLAGS (with ID bit inverted)
    pushfd                               ; Store EFLAGS again (ID bit may or may not be inverted)
    pop eax                              ; eax = modified EFLAGS (ID bit may or may not be inverted)
    xor eax,[esp]                        ; eax = whichever bits were changed
    popfd                                ; Restore original EFLAGS
    and eax,0x00200000                   ; eax = zero if ID bit can't be changed, else non-zero
    jz .no_cpuid                         ; If zero, no CPUID
    mov byte [_cpuid_supported], 1       ; If we didn't jump to .no_cpuid, then set it to supported
    jmp .done                            ; Exit
.no_cpuid:
    mov byte [_cpuid_supported], 0       ; No CPUID
    jmp .done                            ; Exit
.done:
    popad                                ; Restore regisgters
    ret                                  ; Return