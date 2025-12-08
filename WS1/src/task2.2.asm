; Include the assembly I/O library
%include "src/asm_io.inc"

segment .data
    array dd 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,\
             21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,\
             41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,\
             61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,\
             81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100

    sum_msg db "The sum of the array is: ", 0

    sum_result dd 0       ; <--- NEW VARIABLE to store the sum

segment .text
    global asm_main

asm_main:
    pusha

    mov esi, array        ; pointer to array
    mov ecx, 100          ; loop counter
    xor eax, eax          ; accumulator = 0

sum_loop:
    add eax, [esi]        ; add element to sum
    add esi, 4            ; next element
    loop sum_loop

    ; Store the sum in the variable
    mov [sum_result], eax ; <--- store sum in memory

    ; Print the message
    mov eax, sum_msg
    call print_string

    ; Load the sum from memory and print it
    mov eax, [sum_result] ; <--- load from memory to print
    call print_int
    call print_nl

    popa
    mov eax, 0
    ret
