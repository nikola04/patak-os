#include "defs.h"
#include "spinlock.h"
#include "x86.h"

#define CRTPORT 0x3d4

struct console {
    struct spinlock lock;
} console;

static int panicked = 0;

void consoleputc(char c);

void panic(const char *panic){
    cli();

    const char* prefix = "\npanic: ";
    char suffix = '!';
    for(size_t i = 0; i < strlen(prefix); i++)
        consoleputc(prefix[i]);
    for(size_t i = 0; i < strlen(panic); i++)
        consoleputc(panic[i]);
    consoleputc(suffix);

    panicked = 1; // free othe cpus
    for(;;) // freeze cpu
		;
}

static short *vga = (short *)0xb8000;

void vgaputc(char c){
    int pos = 0;

    outb(CRTPORT, 0x0E);
    pos |= inb(CRTPORT + 1) << 8;
    outb(CRTPORT, 0x0F);
    pos |= inb(CRTPORT + 1);

    if(pos >= 80 * 25){
        panic("console overflow");
    }if(c == '\n'){
        pos += 80 - pos % 80;
    }else{
        vga[pos++] = (c&0xff) | 0x0700;
    }

    outb(CRTPORT, 0x0E);
    outb(CRTPORT + 1, pos >> 8);
    outb(CRTPORT, 0x0F);
    outb(CRTPORT + 1, pos);
}

void consoleputc(char c){
    if(panicked == 1){
        for(;;)
            ;
    }
    vgaputc(c);
}

void consolewrite(const char *buf, size_t len) {
    acquire(&console.lock);
    for(size_t i = 0; i < len; i++){
        consoleputc(buf[i]);
    }
    release(&console.lock);
}

void consoleinit(){
    spinlockinit(&console.lock, "console");
}
