; HumbleOS file: boot.asm
; Purpose: Booting code. Initalizes GDT (using gdt.c), sets up segments and stack.

global start

; Externs
extern kernel_main ; Entry point of C
extern gdt_gdt_c ; C function to set up GDT
extern gdt_gdtr ; C pointer to GDTR
extern multiboot_info ; Multiboot info struct pointer

section .text ; Code section
bits 32 ; 32 bits code
start:
	mov [multiboot_info], ebx ; Multiboot info struct pointer
	
	; GDT
	call gdt_gdt_c ; C handles creating the GDT
	lgdt [gdt_gdtr] ; Load the C GDT
	
	jmp 0x08:.reload_CS ; 0x08 is code segment
.reload_CS:

	mov ax, 0x10 ; 0x10 is data segment
	mov ds, ax   ; Data segment
	mov es, ax   ; Extra segment
	mov fs, ax   ; Another extra segment
	mov gs, ax   ; Yet another extra segment
	mov ss, ax   ; Stack segment

	mov esp, stack ; Set up stack
	
    call kernel_main ; Jump into 

    jmp $ ; Hang if kernel returns

section .bss
resb 32767 ; 32KB stack
stack:

section .note.GNU-stack ; Needed for LD to not complain.