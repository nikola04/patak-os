; kernel.asm - patakOS
; Copyright (C) 2025 Nikola Nedeljkovic
; This program is free software: you can redistribute it and/or modify it under the terms of
; the GNU General Public License, version 3, or (at your option) any later version.
; You can freely share and/or modify this program under the terms of GPL-3.
; See the full license text at: https://www.gnu.org/licenses/gpl-3.0.html

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
