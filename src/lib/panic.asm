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
