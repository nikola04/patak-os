ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 0x100000; /* Kernel load address */
    
    .text : ALIGN(4096){
        *(.text)
    }

    .rodata : ALIGN(4096){
        *(.rodata)
    }

    .data : ALIGN(4096){
        *(.data)
    }

    .bss : ALIGN(4096){
        *(COMMON)
        *(.bss)
    }
}
