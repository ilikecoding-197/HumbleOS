section .multiboot_header

%define MAGIC 0xe85250d6
%define CODE 0 
%define HEADER_LENGTH (header_end - header_start)
header_start:
    dd MAGIC                     ; magic number
    dd CODE                      ; protected mode code
    dd header_end - header_start ; header length

    ; checksum
    dd 0x100000000 - (MAGIC + CODE + HEADER_LENGTH)

    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .note.GNU-stack
