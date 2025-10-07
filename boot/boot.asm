[bits 16]
org 0x7C00

xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7C00

;mov si, msg
mov bx, 0x1234
call print_hex_16

jmp $

%include 'utils/print.asm'

msg:
  db 'Hello OS World', 0

times 510 - ($ - $$) db 0
dw 0xaa55
