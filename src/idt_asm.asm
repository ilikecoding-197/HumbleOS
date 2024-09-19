%macro interupt 1
interupt_stub_%+%1:
	pushad
	mov word [irq], %+%1
	call interupt_handler
	popad
    iret
%endmacro

extern interupt_handler
%assign i 0 
%rep    48 
    interupt i
%assign i i+1 
%endrep

global interupt_stub_table
interupt_stub_table:
%assign i 0 
%rep    48 
    dd interupt_stub_%+i
%assign i i+1 
%endrep

global irq
irq: resb 1
