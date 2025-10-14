[extern interrupt_handler]

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1

interrupt_handler_%1:
    push dword 0
    push dword %1
    jmp common_interrupt_handler

%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
    
interrupt_handler_%1:
    push dword %1
    jmp common_interrupt_handler
%endmacro

common_interrupt_handler:
    pushad
    push    ds
    push    es
    push    fs
    push    gs

    push esp
    call    interrupt_handler
    add     esp, 4

    pop     gs
    pop     fs
    pop     es
    pop     ds
    popad

    add     esp, 8

    iret

; |---------------------------------|
; | ISR definitions                 |
; |---------------------------------|

; 0-7: No Error Code
no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7

; 8: Error Code
error_code_interrupt_handler 8

; 9: No Error Code
no_error_code_interrupt_handler 9

; 10-14: Error Code
error_code_interrupt_handler 10
error_code_interrupt_handler 11
error_code_interrupt_handler 12
error_code_interrupt_handler 13
error_code_interrupt_handler 14

; 15-16: No Error Code
no_error_code_interrupt_handler 15
no_error_code_interrupt_handler 16

; 17: Error Code
error_code_interrupt_handler 17

; 18-31: No Error Code
no_error_code_interrupt_handler 18
no_error_code_interrupt_handler 19
no_error_code_interrupt_handler 20
no_error_code_interrupt_handler 21
no_error_code_interrupt_handler 22
no_error_code_interrupt_handler 23
no_error_code_interrupt_handler 24
no_error_code_interrupt_handler 25
no_error_code_interrupt_handler 26
no_error_code_interrupt_handler 27
no_error_code_interrupt_handler 28
no_error_code_interrupt_handler 29
no_error_code_interrupt_handler 30
no_error_code_interrupt_handler 31

; 32-255: Hardware and Software Interrupts (No Error Code)
%assign i 32
%rep 224 ; 256 - 32 = 224
    no_error_code_interrupt_handler i
%assign i i+1
%endrep

section .data
global interrupt_handlers_stub_table

interrupt_handlers_stub_table:
    %assign i 0  
    %rep 256
        dd interrupt_handler_%+i
    %assign i i+1
    %endrep