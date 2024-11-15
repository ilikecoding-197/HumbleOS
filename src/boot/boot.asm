global start

; Externs
extern kernel_main ; Entry point of C
extern gdt_c ; C function to set up GDT
extern gdtr ; C pointer to GDTR

section .text
bits 32
start:
	; GDT
	call gdt_c ; C handles creating the GDT
	lgdt [gdtr] ; Load the C GDT
	
	jmp 0x08:.reload_CS ; 0x08 is code segment
.reload_CS:

	mov ax, 0x10 ; 0x10 is data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
    call kernel_main ; Jump into C

    jmp $ ; Hang if kernel returns; shouldn't happen though.

section .bss
resb 32767 ; 32KB stack
stack:

section .note.GNU-stack
