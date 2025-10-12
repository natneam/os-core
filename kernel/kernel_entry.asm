[bits 16]

call switch_to_pm

jmp $

%include "./arch/print_32.asm"
%include "./arch/switch_to_pm.asm"
%include "./arch/GDT.asm"

[bits 32]
[extern main]

BEGIN_PM:
  mov ebx, MSG_PROT_MODE
  call print_string_pm

  call main
  
  jmp $

MSG_PROT_MODE   db "Successfully landed in 32-bit Protected", 0