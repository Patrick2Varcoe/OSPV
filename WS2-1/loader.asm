global loader


KERNEL_STACK_SIZE equ 4096
MAGIC_NUMBER      equ 0x1BADB002
FLAGS             equ 0
CHECKSUM          equ -(MAGIC_NUMBER + FLAGS)
NUM               equ 0xCAFEBABE


extern CURSORPOS

extern sumofthree
extern init_fb
extern square
extern IncWritePos
extern change_fg_color
extern change_bg_color
extern fb_write_cell
extern fb_move_cursor
extern fb_write_string
extern fb_write_number
extern fb_move_cursor2D


section .data
	WRITEPOS	  dd 2
	FG_COL		 dd 15
	BG_COL		dd 8
	MYSTR		db "Operating Systems", 0
	MYSTR2		db "Short";"A very long sentence to take up the whole screen hopefully", 0
section .bss
    align 4
kernel_stack:
    resb KERNEL_STACK_SIZE


section .text
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    ; ─────────────────────────────
    ; Set up the kernel stack
    ; ─────────────────────────────
    mov esp, kernel_stack + KERNEL_STACK_SIZE

    ; Example: load value into eax
    mov eax, NUM

    ; ─────────────────────────────
    ; Call sumofthree(1, 2, 3)
    ; ─────────────────────────────
    push dword 3
    push dword 2
    push dword 1
    call sumofthree
    add esp, 12      ; clean up the 3 arguments from the stack

    ; eax now holds the result
    ;mov word[WRITEPOS], 0x4128
    
    ;call init_fb
    
    ;push dword WRITEPOS
    ;call IncWritePos
    ;mov [WRITEPOS], eax
    
    push dword 3
    call square
    add esp, 4
    ;mov word[0x000B8002], 0x2841
    ;mov word[0x000B8004], 0x2841
    ;mov word[0x000B8006], 0x2841
    
    push dword 15
    call change_fg_color
    add esp, 4
    mov [FG_COL] , EAX
    push dword [FG_COL]
    push dword [BG_COL]
    push dword 'A'
    push dword [WRITEPOS]
    call fb_write_cell
    mov [WRITEPOS], EAX
    add esp, 16
    
    ;add dword [WRITEPOS] , 2
    
    push dword 2
    call change_fg_color
    add esp, 4
    mov [FG_COL] , EAX
    
    push dword 8
    call change_bg_color
    add esp, 4
    mov [BG_COL] , EAX
    
    push dword [FG_COL]
    push dword [BG_COL]
    push dword 'H'
    push dword [WRITEPOS]
    call fb_write_cell
    mov [WRITEPOS], EAX
    add esp, 16
    
    ;add dword [WRITEPOS] , 2
    
    ;push dword 8
    ;call fb_move_cursor
    ;add esp, 4
    
    push dword [FG_COL]
    push dword [BG_COL]
    push dword MYSTR
    push dword [WRITEPOS]
    call fb_write_string
    mov [WRITEPOS], EAX
    add esp, 16
    
    push dword [FG_COL]
    push dword [BG_COL]
    push dword 1234
    push dword [WRITEPOS]
    call fb_write_number
    mov [WRITEPOS], EAX
    add esp, 16
    

    push dword [FG_COL]
    push dword [BG_COL]
    push dword MYSTR2
    push dword [WRITEPOS]
    call fb_write_string
    mov [WRITEPOS], EAX
    add esp, 16
    
    push dword 4
    push dword 4
    call fb_move_cursor2D
    add esp, 8
    
    

.hang:
    jmp .hang        ; stop here

