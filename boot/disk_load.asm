; =============================================================================
; DEPRECATED - HISTORICAL REFERENCE ONLY
; =============================================================================
; This file was part of the original 16-bit bootloader and is no longer
; used in the current GRUB/Multiboot-based build. It is preserved here for
; educational and historical purposes to show the transition from a custom
; bootloader to a standard one.
;
; The functionality of this file has been replaced by the GRUB bootloader.
; =============================================================================

[bits 16]

disk_load:
  push dx
  mov ah, 0x02
  mov al, dh
  mov ch , 0x00
  mov dh , 0x00
  mov cl , 0x02

  int 0x13

  jc disk_error

  pop dx
  cmp dh , al
  jne disk_error
  ret

disk_error:
  mov si, DISK_ERROR_MESSAGE
  call print_string
  jmp $

DISK_ERROR_MESSAGE db "Disk read error!" , 0
