; HumbleOS file: panic.asm
; Purpose: Assmebly for panic

[bits 32]

; Regs
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
mov dword [panic_reg_eax], eax ; Save EAX
mov dword [panic_reg_ebx], ebx ; Save EBX
mov dword [panic_reg_ecx], ecx ; Save ECX
mov dword [panic_reg_edx], edx ; Save EDX
mov dword [panic_reg_esi], esi ; Save ESI
mov dword [panic_reg_edi], edi ; Save EDI
mov dword [panic_reg_esp], esp ; Save ESP
mov dword [panic_reg_ebp], ebp ; Save EBP
ret

section .note.GNU-stack ; Stop LD from complaining
