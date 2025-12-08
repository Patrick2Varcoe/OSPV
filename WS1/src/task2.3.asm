; Include the assembly I/O library for input/output functions
%include "src/asm_io.inc"

; Data segment: defines initialized data
segment .data
    ; Define an array of 100 double words (32-bit integers) initialized to 1 through 100
    array dd 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,\
             21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,\
             41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,\
             61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,\
             81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100

    ; Messages for user interaction
    start_msg db "Enter the start of the range (1-100): ", 0
    end_msg db "Enter the end of the range (1-100): ", 0
    error_msg db "Invalid range. Start must be >=1, end <=100, and start <= end. Try again.", 0
    sum_msg db "The sum of the range is: ", 0

    ; Variables to store user input
    start_range dd 0
    end_range dd 0
    sum_result dd 0

; Text segment: contains the code
segment .text
    ; Declare the main function as global so it can be called from C
    global asm_main

; Main function entry point
asm_main:
    ; Push all general-purpose registers onto the stack to preserve their values
    pusha

input_loop:
    ; Prompt for start range
    mov eax, start_msg
    call print_string
    call read_int
    mov [start_range], eax

    ; Prompt for end range
    mov eax, end_msg
    call print_string
    call read_int
    mov [end_range], eax

    ; Validate range: start >=1, end <=100, start <= end
    mov eax, [start_range]
    cmp eax, 1
    jl invalid_range

    mov eax, [end_range]
    cmp eax, 100
    jg invalid_range

    mov eax, [start_range]
    cmp eax, [end_range]
    jg invalid_range

    ; If valid, proceed to sum
    jmp calculate_sum

invalid_range:
    ; Display error message and loop back
    mov eax, error_msg
    call print_string
    call print_nl
    jmp input_loop

calculate_sum:
    ; Initialize sum to 0
    xor eax, eax
    mov [sum_result], eax

    ; Set up loop: ESI points to start of range in array
    ; Array index starts at 0, so start_range - 1
    mov eax, [start_range]
    dec eax  ; Adjust for 0-based index
    mov esi, array
    lea esi, [esi + eax*4]  ; Point to start element

    ; ECX = number of elements to sum (end - start + 1)
    mov eax, [end_range]
    sub eax, [start_range]
    inc eax
    mov ecx, eax

    ; Reset EAX to 0 for summing
    xor eax, eax

sum_loop:
    ; Add current element to sum
    add eax, [esi]
    ; Move to next element
    add esi, 4
    ; Decrement counter and loop if not zero
    loop sum_loop

    ; Store the sum
    mov [sum_result], eax

    ; Display the sum message
    mov eax, sum_msg
    call print_string

    ; Load and print the sum
    mov eax, [sum_result]
    call print_int
    call print_nl

    ; Pop all general-purpose registers from the stack to restore their values
    popa
    ; Set EAX to 0 (return value indicating success)
    mov eax, 0
    ; Return from the function
    ret
