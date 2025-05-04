; main.asm - patakOS
; Copyright (C) 2025 Nikola Nedeljkovic
; This program is free software: you can redistribute it and/or modify it under the terms of
; the GNU General Public License, version 3, or (at your option) any later version.
; You can freely share and/or modify this program under the terms of GPL-3.
; See the full license text at: https://www.gnu.org/licenses/gpl-3.0.html

bits 16
org 0x7C00

CODE_SEG equ 0x08
DATA_SEG equ 0x10
STAGE2_SEG equ 0x800
STAGE2_ADDR equ 0x8000
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
    mov ax, KERNEL_ADDR >> 4
    mov es, ax
    xor bx, bx

    mov ah, 0x02       ; Read sectors
    mov al, 24         ; 24 sectors
    mov ch, 0          ; Cylinder 0
    mov cl, 2          ; Sector 2 (first after boot)
    mov dh, 0          ; Head 0
    mov dl, 0x80       ; Drive 0 (HDD)
    int 0x13

    jc disk_read_err

load_pm:
    cli
    lgdt [gdt_descriptor]

    ; Set PE bit in cr0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; jump to 32-bit mode
    jmp CODE_SEG:init_pm32

disk_read_err:
    jmp $

; -------------------------
; Protected Mode GDT
; -------------------------
gdt_start:
    dq 0x0000000000000000    ; Null descriptor
    dq 0x00CF9A000000FFFF    ; Code segment: base=0, limit=4GB, execute/read
    dq 0x00CF92000000FFFF    ; Data segment: base=0, limit=4GB, read/write

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; -------------------------
; 32-bit protected mode
; -------------------------
init_pm32:
    bits 32

    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0x90000
    mov esp, ebp

    ; Enable A20 - fast gate
    in al, 0x92
    or al, 2
    and al, 0xFE
    out 0x92, al

main64:

    ; Page table for 64    
    mov edi, 0x100000
    xor eax, eax
    mov ecx, 4096 * 4 / 4      ; 16 KB
    rep stosd

    ; PML4[0] = PDP @ 0x101000 | present + rw
    mov dword [0x100000], 0x101000 | 0x03
    ; PDP[0] = PD @ 0x102000 | present + rw
    mov dword [0x101000], 0x102000 | 0x03
    ; PD[0] = 2MB page @ 0x00000000 | present + rw + PS (bit 7)
    mov dword [0x102000], 0x00000083

    ; GDT long mode
    lgdt [gdt64_descriptor]

    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set CR3
    mov eax, 0x100000
    mov cr3, eax

    ; enable long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; jmp in long mode
    jmp 0x18:start64

; -------------------------
; Long Mode GDT
; -------------------------
align 8
gdt64:
    dq 0x0000000000000000          ; 0x00 - null
    dq 0x00AF9A000000FFFF          ; 0x08 - 32-bit code (ne koristi se)
    dq 0x00AF92000000FFFF          ; 0x10 - 32-bit data (ne koristi se)
    dq 0x00209A0000000000          ; 0x18 - 64-bit code
    dq 0x0000920000000000          ; 0x20 - 64-bit data

gdt64_descriptor:
    dw gdt64_end - gdt64 - 1
    dd gdt64

gdt64_end:

; -------------------------
; 64-bit kernel entry
; -------------------------
bits 64
start64:
    mov ax, 0x20
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov rsp, 0x90000
    mov rbp, rsp

    jmp KERNEL_ADDR

    hlt 
    jmp $

times 510-($-$$) db 0
dw 0xAA55
