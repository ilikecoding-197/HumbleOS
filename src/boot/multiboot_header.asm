; HumbleOS file: multiboot_header.asm
; Purpose: Header for multiboot

section .multiboot_header ; The header has its own section

%define MAGIC 0xe85250d6 ; Nagic code for multiboot
%define CODE 0 ; Protected mode code
%define HEADER_LENGTH (header_end - header_start) ; Length of header
header_start:
    dd MAGIC
    dd CODE
    dd header_end - header_start

    ; Checksum
    dd 0x100000000 - (MAGIC + CODE + HEADER_LENGTH)

    ; Required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .note.GNU-stack ; Needed for LD to not complain
