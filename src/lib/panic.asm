;
; panic.asm - assembly code for panic
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

[bits 32]

global panic_reg_eax
global panic_reg_ebx
global panic_reg_ecx
global panic_reg_edx
global panic_reg_esi
global panic_reg_edi
global panic_reg_esp
global panic_reg_ebp

section .bss
panic_reg_eax: resd 1
panic_reg_ebx: resd 1
panic_reg_ecx: resd 1
panic_reg_edx: resd 1
panic_reg_esi: resd 1
panic_reg_edi: resd 1
panic_reg_esp: resd 1
panic_reg_ebp: resd 1

global panic_save_regs
section .text

panic_save_regs:
mov dword [panic_reg_eax], eax
mov dword [panic_reg_ebx], ebx
mov dword [panic_reg_ecx], ecx
mov dword [panic_reg_edx], edx
mov dword [panic_reg_esi], esi
mov dword [panic_reg_edi], edi
mov dword [panic_reg_esp], esp
mov dword [panic_reg_ebp], ebp
ret

section .note.GNU-stack
