%include "asm_io.inc"
segment .data
    prompt_name db "Enter your name: ", 0
    prompt_count db "Enter the number of times to print the welcome message (51-99): ", 0
    error_msg db "Error: The number must be greater than 50 and less than 100.", 0
    welcome_prefix db "Welcome ", 0
segment .bss
    name_buffer resb 100 ; buffer for name
    count resd 1 ; number of times
segment .text
    global asm_main
asm_main:
    pusha
    ; Prompt for name
    mov eax, prompt_name
    call print_string
    ; Read name character by character until newline
    mov edi, name_buffer
    mov ecx, 99
read_name_loop:
    call read_char
    cmp al, 10 ; newline
    je end_read_name
    mov [edi], al
    inc edi
    loop read_name_loop
end_read_name:
    mov byte [edi], 0 ; null terminate
    ; Prompt for count
    mov eax, prompt_count
    call print_string
    call read_int
    mov [count], eax
    ; Validate count: >50 and <100
    cmp eax, 50
    jle invalid
    cmp eax, 100
    jge invalid
    ; Valid: loop to print welcome
    mov ecx, [count]
print_loop:
    mov eax, welcome_prefix
    call print_string
    mov eax, name_buffer
    call print_string
    call print_nl
    loop print_loop
    jmp done
invalid:
    mov eax, error_msg
    call print_string
    call print_nl
done:
    popa
    mov eax, 0
    ret
