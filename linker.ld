ENTRY(_start)

SECTIONS
{
    . = 0x10000;

    _kernel_start = .;
    
    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : { 
        *(.bss)
    }

    _kernel_end = .;
}
