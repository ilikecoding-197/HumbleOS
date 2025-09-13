;
; multiboot_header.asm - multiboot header
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

section .multiboot_header ; the header has its own section
bits 32

%define MAGIC 0x1BADB002 ; magic code for multiboot
%define FLAGS 0x00000003 ; flags for multiboot (memory info and video mode)
%define CHECKSUM -(MAGIC + FLAGS) ; checksum for multiboot header

header_start:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
header_end:

section .note.GNU-stack