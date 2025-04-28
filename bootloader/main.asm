; main.asm - simpleOS
; Copyright (C) 2025 Nikola Nedeljkovic
; This program is free software: you can redistribute it and/or modify it under the terms of
; the GNU General Public License, version 3, or (at your option) any later version.
; You can freely share and/or modify this program under the terms of GPL-3.
; See the full license text at: https://www.gnu.org/licenses/gpl-3.0.html

org 0x7C00
bits 16

start:
    jmp main

; Prints string to screen
; Params:
;   - ds:si string
puts:
    push si
    push ax

.loop:
    lodsb
    or al, al
    jz .done

    mov ah, 0x0e
    mov bh, 0
    int 0x10

    jmp .loop

.done:
    pop ax
    pop si
    ret

putln:
    mov ah, 0x0e
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
    ret

main:

    ; setup data segments
    mov ax, 0
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7C00

    ; new line
    call putln

    ; print boot message
    mov si, boot_msg
    call puts

    ; new line
    call putln

    ; print version message
    mov si, version_msg
    call puts

    ; double new line
    call putln
    call putln

    hlt

.halt:
    jmp .halt


boot_msg: db 'Welcome to simpleOS', 0
version_msg: db 'Version 0.1', 0

times 510-($-$$) db 0
dw 0xAA55
