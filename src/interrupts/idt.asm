;
; idt.asm - assembly code for the IDT
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

%macro interrupt 1
interrupt_stub_%+%1:
	pushad                 ; Push all registers
	mov word [irq], %+%1   ; Store the IRQ
	call interrupt_handler ; Call the interrupt handler
	mov al, 0x20           ; PIC EOI command
	out 0x20, al           ; Master PIC
	out 0xa0, al           ; Slave PIC
	popad                  ; Pop all registers
    iret                   ; Return from interrupt
%endmacro

extern interrupt_handler
%assign i 0 
%rep    48 
    interrupt i
%assign i i+1 
%endrep

global interrupt_stub_table
interrupt_stub_table:
%assign i 0 
%rep    48 
    dd interrupt_stub_%+i
%assign i i+1 
%endrep

section .bss
global irq
global _error_code
irq: resb 1
_error_code: resd 1

section .note.GNU-stack