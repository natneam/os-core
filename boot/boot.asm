[bits 16]
[org 0x7C00]

KERNEL_OFFSET equ 0x1000
SECTORS_TO_READ equ 15

mov [BOOT_DRIVE], dl

mov bp, 0x9000
mov sp, bp

mov si, MSG_REAL_MODE
call print_string

call load_kernel

jmp KERNEL_OFFSET

jmp $

%include "./boot/print_16.asm"
%include "./boot/disk_load.asm"

load_kernel:
  mov bx, KERNEL_OFFSET
  mov dh, SECTORS_TO_READ
  mov dl, [BOOT_DRIVE]
  call disk_load

  mov si, MSG_LOAD_KERNEL
  call print_string

  ret

BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Started in 16-bit Real Mode" , 0
MSG_LOAD_KERNEL db "Kernel loaded into memory", 0

times 510 - ($ - $$) db 0
dw 0xAA55
