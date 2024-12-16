[bits 32]

extern cpuid_exists
extern cpuid_vector_str
extern cpuid_features_ecx
extern cpuid_features_edx
extern cpuid_rdseed

global _cpuid_check
global _cpuid_get_vec_str
global _cpuid_get_features

%define CPUID_VECTOR_STR 0x0
%define CPUID_FEATURES 0x1

_cpuid_check:
pushad
pushfd
pushfd
xor dword [esp],0x00200000
popfd
pushfd
pop eax
xor eax,[esp]
popfd
and eax, 0x00200000
mov [cpuid_exists], eax
popad
ret

_cpuid_get_vec_str:
pushad
mov byte [cpuid_vector_str+12], 0
mov eax, CPUID_VECTOR_STR
cpuid
mov dword [cpuid_vector_str], ebx
mov dword [cpuid_vector_str+4], edx
mov dword [cpuid_vector_str+8], ecx
mov eax, 7
mov ecx, 0
cpuid
shr ebx, 18
and ebx, 1
mov dword [cpuid_rdseed], ebx
popad
ret

_cpuid_get_features:
pushad
mov eax, CPUID_FEATURES
cpuid
mov dword [cpuid_features_ecx], ecx
mov dword [cpuid_features_edx], edx
popad
ret

section .note.GNU-stack: