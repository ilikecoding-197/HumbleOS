;
; cpuid.asm - asm code for cpuid
;
; Part of HumbleOS
;
; Copyright 2025 Thomas Shrader
;
; Permission is hereby granted, free of charge, to any person obtaining a copy of this software
; and associated documentation files (the "Software"), to deal in the Software without restriction,
; including without limitation the rights to use, copy, modify, merge, publish, distribute,
; sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all copies or substantial
; portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
; NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
; NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
; DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;

section .text
bits 32

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

section .bss
global _cpuid_supported

_cpuid_supported: resb 1

section .note.GNU-stack