#include "defs.h"
#include "x86.h"
#include "ps2kbd.h"
#include "kbd.h"

#define PS2_DATA_PORT 0x60

char parse_sc(int32_t code){
    static uint8_t mask = 0;
    if(code & 0x80){
        return 0;
    }
    return scancode_map[code];
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
