[bits 16]
org 0x7C00

start:
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, 0x7C00
  mov cx, 10

.print_loop:
  mov si, message
  call print_message
  call print_newline
  loop .print_loop
  jmp $

print_message:
  mov ah, 0x0E
  .loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
  .done
    ret

print_newline:
  push ax
  mov ah,0x0E
  mov al, 0x0D
  int 0x10
  mov al, 0x0A
  int 0x10

  pop ax
  ret

message db 'Booting OS ...', 0 

times 510 - ($ - $$) db 0
dw 0xAA55
