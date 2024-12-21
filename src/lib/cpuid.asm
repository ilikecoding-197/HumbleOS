; HumbleOS file: cpuid.asm
; Purpose: Assmembly code for CPUID.

[bits 32]

; Variables from cpuid.c
extern cpuid_exists       ; Boolean       from cpuid.c, see cpuid.h for info
extern cpuid_vector_str   ; Character[13] from cpuid.c, see cpuid.h for info
extern cpuid_features_ecx ; Integer       from cpuid.c, see cpuid.h for info
extern cpuid_features_edx ; Integer       from cpuid.c, see cpuid.h for info
extern cpuid_rdseed       ; Boolean       from cpuid.c, see cpuid.h for info

; Exports
global _cpuid_check
global _cpuid_get_vec_str
global _cpuid_get_features

; Constants
%define CPUID_VECTOR_STR 0x0 ; Get vector string command
%define CPUID_FEATURES 0x1   ; Get features command

;
; _cpuid_check - Check for CPUID and fill in cpuid_exists variable.
; Code from https://wiki.osdev.org/CPUID, comments also from there.
;
_cpuid_check:
    pushfd                     ; Save EFLAGS
    pushfd                     ; Store EFLAGS
    xor dword [esp],0x00200000 ; Invert the ID bit in stored EFLAGS
    popfd                      ; Load stored EFLAGS (with ID bit inverted)
    pushfd                     ; Store EFLAGS again (ID bit may or may not be inverted)
    pop eax                    ; eax = modified EFLAGS (ID bit may or may not be inverted)
    xor eax,[esp]              ; eax = whichever bits were changed
    popfd                      ; Restore original EFLAGS
    and eax,0x00200000         ; eax = zero if ID bit can't be changed, else non-zero
    ret

;
; _cpuid_get_vec_str - Get the CPUID vector string. Returnin cpuid_vector_str
_cpuid_get_vec_str:
    pushad                              ; Save registers
    mov byte [cpuid_vector_str+12], 0   ; NUL byte
    mov eax, CPUID_VECTOR_STR           ; Command for cpuid
    cpuid                               ; Call CPUID
    mov dword [cpuid_vector_str], ebx   ; Vector string 0-3
    mov dword [cpuid_vector_str+4], edx ; Vector string 4-7
    mov dword [cpuid_vector_str+8], ecx ; Vector string 8-11
    popad                               ; Restore registers
    ret

_cpuid_get_features:
    pushad                              ; Save registers
    mov eax, CPUID_FEATURES             ; CPUID_FEATURES commands
    cpuid                               ; Call cpuid
    mov dword [cpuid_features_ecx], ecx ; ECX features
    mov dword [cpuid_features_edx], edx ; EDX features
    mov eax, 7                          ; EAX=07h gives RDSEED info
    mov ecx, 0                          ; ECX=00h is also needed
    cpuid                               ; Call CPUID
    shr ebx, 18                         ; Bit 18 gives us RDSEED support boolean
    and ebx, 1                          ; Make sure its only that bit
    mov dword [cpuid_rdseed], ebx       ; Report that
    popad                               ; Restore registers
    ret

section .note.GNU-stack: