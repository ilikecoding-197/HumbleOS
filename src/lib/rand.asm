[bits 32]

extern rand_randFromAsm
extern rand_successFromAsm
global rand_rdseed

rand_rdseed:
push eax
rdseed eax
jnc .success
mov byte [rand_successFromAsm], 1
ret
.success:
mov byte [rand_successFromAsm], 0
mov dword [rand_randFromAsm], eax
pop eax
ret

section .note.GNU-stack: