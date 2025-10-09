[bits 16]

print_string:
  mov ah, 0x0E
  .loop:
    mov al, [si]
    cmp al, 0
    je .end
    int 0x10
    inc si
    jmp .loop

  .end:
    ret

print_hex:
    mov si, HEX_OUT
    inc si
    inc si
    mov cx, 4

.loop:
    rol dx, 4

    mov al, dl
    and al, 0x0F

    add al, '0'
    cmp al, '9'
    jle .store

    add al, 'A' - '9' - 1 ; ('A' - '0' - 10) -> 7

.store:
    mov [si], al
    inc si

    dec cx
    jnz .loop

    mov si, HEX_OUT
    call print_string
    ret

HEX_OUT:
  db '0x0000', 0
