#include "defs.h"
#include "spinlock.h"
#include "x86.h"
#include "console.h"
#include "kbd.h"

void console_putc(char c);

void panic(const char *panic){
    cli();

    puts("\npanic: ");
    puts(panic);
    puts("!");

    panicked = true; // free othe cpus
    for(;;) // freeze cpu
		;
}

static uint16_t *vga = (uint16_t *)0xb8000;

const uint16_t _blank = 0xFF | VGA_CHAR_ATR;
void vga_putc(char c){
    uint32_t pos = 0;

    outb(CRTPORT, 0x0E);
    pos |= inb(CRTPORT + 1) << 8;
    outb(CRTPORT, 0x0F);
    pos |= inb(CRTPORT + 1);

    if(pos >= 80 * 25){
        panic("console overflow");
    }

    if(c == '\n'){
        pos += 80 - pos % 80;
    }else if(c == BACKSPACE){
        if(pos <= 0) return;
        vga[--pos] = _blank;
    }else{
        vga[pos++] = (c & 0xFF) | VGA_CHAR_ATR;
    }

    if(pos >= 25 * 80){
        memmove(vga, vga + 80, 24 * 80 * sizeof(uint16_t));
        for(int i = 24 * 80; i < 25 * 80; i++)
            vga[i] = _blank;
        pos -= 80;
    }

    outb(CRTPORT, 0x0E);
    outb(CRTPORT + 1, pos >> 8);
    outb(CRTPORT, 0x0F);
    outb(CRTPORT + 1, pos);
}

void console_putc(char c){
    if(panicked == true){
        for(;;);
    }
    vga_putc(c);
}

void console_intr(int32_t (*kbd_getc)(void)){
    acquire(&console.lock);

    char c;
    while((c = kbd_getc()) >= 0){
        if(c == 0)
            continue;
        else if(c == BACKSPACE){
            if(console.input.e == console.input.w) // if not available to delete
                continue;
            if(--console.input.e < 0) console.input.e = INPUT_BUF - 1;
        }else {
            if((console.input.e + 1) % INPUT_BUF == console.input.w)
                continue;

            console.input.buffer[console.input.e] = c;
            console.input.e = (console.input.e + 1) % INPUT_BUF;

            if(c == NEWLINE){
                console.input.w = console.input.e; // lock line
            }
        }
        console_putc((char) c);
    }

    release(&console.lock);
}

void console_read(char *dst, size_t n){ // should implement locking
    while(n > 0){
        while(console.input.r == console.input.w); // should sleep 

        char c = console.input.buffer[console.input.r++];
        console.input.r %= INPUT_BUF;

        *dst++ = c;
        n--;

        if(c == NEWLINE)
            break;
    }
    *dst = '\0';
    puts(""); // <- magically works with this...
    return;
}

void puts(const char *str){
    for(int i = 0; i < strlen(str); i++){
        console_putc(str[i]);
    }
}

void cprintf(const char* fmt, ...){
    va_list args;
    acquire(&console.lock);

    va_start(args, fmt);
    fnprintf(console_putc, fmt, args);
    va_end(args);

    release(&console.lock);
}

void console_init(){
    spinlock_init(&console.lock, "console");
    memset(console.input.buffer, 0, INPUT_BUF);
    console.input.r = console.input.w = console.input.e = 0;
}
