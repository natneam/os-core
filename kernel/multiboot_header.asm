[bits 32]

; Multiboot specification constants
MB_MAGIC        equ 0x1BADB002 ; The magic number for the header
MB_FLAGS        equ 0x03        ; Ask for memory map and video mode info
MB_CHECKSUM     equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM