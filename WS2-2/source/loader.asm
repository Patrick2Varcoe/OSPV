[BITS 32]

[GLOBAL loader]
[GLOBAL _start]

section .text
_start:
loader:

; ===============================================================
; Multiboot header (must appear in the first 8 KB of .text)
; ===============================================================
ALIGN 4
MULTIBOOT_MAGIC      equ 0x1BADB002
MULTIBOOT_FLAGS      equ 0x0
MULTIBOOT_CHECKSUM   equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

; ===============================================================
; External C functions
; ===============================================================
extern init_fb
extern interrupts_install_idt
extern terminal_run

; ===============================================================
; Set up stack
; ===============================================================
mov esp, 0x9FB00
mov ebp, esp

; ===============================================================
; Initialize framebuffer
; ===============================================================
call init_fb

; ===============================================================
; Install IDT + enable keyboard IRQ
; ===============================================================
call interrupts_install_idt

; ===============================================================
; Enter OS shell (never returns)
; ===============================================================
call terminal_run

.hang:
    hlt
    jmp .hang

