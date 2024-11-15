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
irq: resb 1

section .note.GNU-stack
