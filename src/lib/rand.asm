; HumbleOS file: rand.asm
; Purpose: ASM code for rand

[bits 32]

; vars from C
extern rand_randFromAsm
extern rand_successFromAsm

global rand_rdseed

;
; rand_rdseed - Get random from rdseed.
;
rand_rdseed:
push eax ; Save EAX
rdseed eax ; Get random
jnc .success ; Success on no carry
mov byte [rand_successFromAsm], 0 ; Fail
pop eax ; Restore EAX
ret
.success:
mov byte [rand_successFromAsm], 1 ; Success
mov dword [rand_randFromAsm], eax ; Random value
pop eax ; Restore eax
ret

section .note.GNU-stack: ; Stop LD from complaining