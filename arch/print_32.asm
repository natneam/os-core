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

[bits 32]

VIDEO_MEMORY equ 0xB8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
  pusha
  mov edx, VIDEO_MEMORY

print_string_pm_loop:
  mov al, [ebx]
  mov ah, WHITE_ON_BLACK

  cmp al, 0
  je .done

  mov [edx], ax
  add ebx, 1
  add edx, 2

  jmp print_string_pm_loop

.done:
  popa
  ret
