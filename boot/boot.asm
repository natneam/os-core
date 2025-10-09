[bits 16]
org 0x7C00

xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax

mov [BOOT_DRIVE], dl
mov bp, 0x8000
mov sp, bp

mov bx, 0x9000
mov dh, 5
mov dl, [BOOT_DRIVE]
call disk_load

mov dx, [0x9000]
call print_hex

mov dx, [0x9000 + 512]
call print_hex

jmp $

%include 'utils/print_16.asm'
%include 'utils/disk_load.asm'

BOOT_DRIVE:
  db 0

times 510 - ($ - $$) db 0
dw 0xaa55

;Add more data we can read with disk_load
times 256 dw 0xDADA
times 256 dw 0xFACE
