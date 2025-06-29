; HumbleOS file: multiboot_header.asm
; Purpose: Header for multiboot

section .multiboot_header ; The header has its own section

; struct multiboot_header
; {
;   /* Must be MULTIBOOT_MAGIC - see above. */
;   multiboot_uint32_t magic;

;   /* Feature flags. */
;   multiboot_uint32_t flags;

;   /* The above fields plus this one must equal 0 mod 2^32. */
;   multiboot_uint32_t checksum;

;   /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
;   multiboot_uint32_t header_addr;
;   multiboot_uint32_t load_addr;
;   multiboot_uint32_t load_end_addr;
;   multiboot_uint32_t bss_end_addr;
;   multiboot_uint32_t entry_addr;

;   /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
;   multiboot_uint32_t mode_type;
;   multiboot_uint32_t width;
;   multiboot_uint32_t height;
;   multiboot_uint32_t depth;
; };

%define MAGIC 0x1BADB002 ; Magic code for multiboot
%define FLAGS 0x00000003 ; Flags for multiboot (memory info and video mode)
%define CHECKSUM -(MAGIC + FLAGS) ; Checksum for multiboot header
; Multiboot header
header_start:
    dd MAGIC ; Magic number
    dd FLAGS ; Flags
    dd CHECKSUM ; Checksum
header_end

section .note.GNU-stack ; Needed for LD to not complain
