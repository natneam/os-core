%include "./kernel/multiboot_header.asm"
%include "./arch/GDT.asm"

[bits 32]
[extern main]
global start

section .text
load_gdt_and_segments:
    lgdt [gdt_descriptor] ; Load the GDT table pointer

    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp CODE_SEG:reload_cs_target

reload_cs_target:
    ret

start:
    mov esp, stack_top
    
    push eax
    push ebx

    ; Load GDT and re-initialize segments
    call load_gdt_and_segments

    call main

    cli
    hlt

section .bss
    resb 16384           ; Reserve 16 KB for the stack
    stack_bottom:
    stack_top: