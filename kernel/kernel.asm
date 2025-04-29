bits 32

global _start
extern kmain

_start:
    ; write B on screen for simplest test
    mov al, 'B'
    mov ah, 0x0f
    mov [0xb8000], ax

    call kmain

; times 512-($-$$) db 0 ; Fill this segment with 0
