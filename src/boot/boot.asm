;
; boot.asm - booting code
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

global start

; externs
extern kernel_main ; entry point of C
extern gdt_gdt_c ; C function to set up GDT
extern gdt_gdtr ; C pointer to GDTR

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
	
	xor ebp, ebp ; Clear EBP - for panic stack frame
    call kernel_main ; Kernel time!

    jmp $

section .bss
resb 32767 ; 32KB stack
stack:

eax_backup: resb 4 ; Reserve space for EAX backup
ebx_backup: resb 4 ; Reserve space for EBX backup

section .note.GNU-stack