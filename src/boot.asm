global start
extern kernel_main
extern gdt_c ; C function to set up GDT
extern gdtr ; C pointer to GDTR
section .text
bits 32
start:
jmp a
	; GDT
	cli ; Disable interupts, just in case.
	call gdt_c ; Call the C function (C is very good)

	; Code from wiki.osdev.org/GDT_Tutorial
	mov ax, [esp + 4]
	mov [gdtr], ax
	mov eax, [esp + 8]
	mov [gdtr + 2], eax
	lgdt [gdtr]
	
	jmp 0x08:.reload_CS ; 0x08 is code segment
.reload_CS:
	mov ax, 0x10 ; 0x10 is data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

a:
	; Jump into C! Yay!
    call kernel_main

    jmp $ ; Hang if kernel returns; shouldn't happen though.

section .bss
resb 32000 ; 32KB stack
stack:
