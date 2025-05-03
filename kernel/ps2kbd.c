#include "defs.h"
#include "x86.h"
#include "ps2kbd.h"

#define PS2_DATA_PORT 0x60

char parse_sc(int32_t code){
    static bool extended = false;
    static uint8_t mask = 0;


    if(code & 0x80){
        if(extended){
            extended = false;
        }else code &= 0x7F;
        mask &= ~mask_map[code];
        return 0;
    }else if(code == 0xE0){
        extended = true;
        return 0;
    }

    if(extended){
        extended = false;
        switch (code) {
            case 0x48: return ARROWUP;
            case 0x50: return ARROWDOWN;
        }
        return 0;
    }

    mask ^= toggle_map[code];
    mask |= mask_map[code];
    
    if(mask & SHIFT)
        return scancode_map_shift[code];

    char c = scancode_map[code];
    if(mask & CAPS && c >= 'a' && c <= 'z')
        c -= 'a' - 'A';
    return c;
}

int32_t kbd_getc(){
    if ((inb(0x64) & 0x01) == 0)
        return -1;

    int32_t scancode = inb(PS2_DATA_PORT);
    char c = parse_sc(scancode);
    return c;
}

void kbdintr(void){
    console_intr(kbd_getc);
    outb(0x20, 0x20);
}
