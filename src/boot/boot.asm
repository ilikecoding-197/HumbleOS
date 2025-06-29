; HumbleOS file: boot.asm
; Purpose: Booting code. Initalizes GDT (using gdt.c), sets up segments and stack.

global start

; Externs
extern kernel_main ; Entry point of C
extern gdt_gdt_c ; C function to set up GDT
extern gdt_gdtr ; C pointer to GDTR

section .text ; Code section
bits 32 ; 32 bits code
start:
	mov esp, stack-8 ; Set up stack
    push eax
    push ebx
	
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
	
    call kernel_main ; Kernel time!

    jmp $ ; Hang if kernel returns

section .bss
resb 32767 ; 32KB stack
stack:

eax_backup: resb 4 ; Reserve space for EAX backup
ebx_backup: resb 4 ; Reserve space for EBX backup

section .note.GNU-stack ; Needed for LD to not complain.
