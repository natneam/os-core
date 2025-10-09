[bits 16]
[org 0x7C00]

mov bp, 0x9000
mov sp, bp

mov si, MSG_REAL_MODE
call print_string

call switch_to_pm

jmp $

%include "./utils/print_16.asm"
%include "./utils/print_32.asm"
%include "./utils/gdt.asm"
%include "./utils/switch_to_pm.asm"

[bits 32]

BEGIN_PM:
  mov ebx, MSG_PROT_MODE
  call print_string_pm

  jmp $

MSG_REAL_MODE db "Started in 16-bit Real Mode" , 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected", 0

times 510 - ($ - $$) db 0
dw 0xAA55
