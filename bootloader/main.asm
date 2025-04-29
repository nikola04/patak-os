; main.asm - simpleOS
; Copyright (C) 2025 Nikola Nedeljkovic
; This program is free software: you can redistribute it and/or modify it under the terms of
; the GNU General Public License, version 3, or (at your option) any later version.
; You can freely share and/or modify this program under the terms of GPL-3.
; See the full license text at: https://www.gnu.org/licenses/gpl-3.0.html

bits 16
org 0x7C00

CODE_SEG equ 0x08
DATA_SEG equ 0x10 
KERNEL_SEG equ 0x1000
KERNEL_ADDR equ 0x10000

main:
    cli
    ; setup segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; load kernel
    mov ax, KERNEL_ADDR / 16
    mov es, ax
    xor bx, bx

    mov ah, 0x02       ; Read sectors
    mov al, 24         ; 24 sectors
    mov ch, 0          ; Cylinder 0
    mov cl, 2          ; Sector 2 (sector 1 je boot)
    mov dh, 0          ; Head 0
    mov dl, 0x80       ; Drive 0 (HDD)
    int 0x13
    jc disk_read_err

load_pm:
    ; moving into 32bit protected mode
    cli

    ; loading GDT
    lgdt [gdt_descriptor]

    ; Set PE bit in cr0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; jump in 32-bit mode
    jmp CODE_SEG:init_pm32

disk_read_err:
    hlt
    ret

gdt_start:
    dq 0x0000000000000000    ; Null descriptor
    dq 0x00CF9A000000FFFF    ; Code segment: base=0, limit=4GB, execute/read
    dq 0x00CF92000000FFFF    ; Data segment: base=0, limit=4GB, read/write
    
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; 32-bit code
init_pm32:
    bits 32

    ; init segments
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; init stack in 32-bit mode
    mov ebp, 0x90000
    mov esp, ebp

    ; allow A20 for more memory
    in al, 0x92
    or al, 2
    out 0x92, al

    mov al, 'A'
    mov ah, 0x0f
    mov [0xb8000], ax

    ; jump to kernel
    jmp CODE_SEG:KERNEL_ADDR

times 510-($-$$) db 0 ; Fill rest of boot sector as 0
dw 0xAA55 ; Required to recognize this as valid boot sector
