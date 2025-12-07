%include "asm_io.inc"

section .data
    integer1        dd      15
    integer2        dd      6

section .bss
    result          resd    1


section .text
    global asm_main                 ;
    asm_main:
        enter   0,0
        pusha
        mov     eax, [integer1]
        add     eax, [integer2]
        mov     [result], eax
        call    print_int
        popa
        mov     eax, 0
        leave
        ret
