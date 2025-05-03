#include "defs.h"
#include "spinlock.h"
#include "x86.h"
#include "kbd.h"

#define CRTPORT 0x3d4

struct console {
    struct spinlock lock;
} console;

static int panicked = 0;

void console_putc(char c);

void panic(const char *panic){
    cli();

    puts("\npanic: ");
    puts(panic);
    puts("!");

    panicked = 1; // free othe cpus
    for(;;) // freeze cpu
		;
}

static short *vga = (short *)0xb8000;

void vga_putc(char c){
    int pos = 0;

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
        vga[--pos] = 0xFF | 0x0700;
    }else{
        vga[pos++] = (c&0xFF) | 0x0700;
    }

    outb(CRTPORT, 0x0E);
    outb(CRTPORT + 1, pos >> 8);
    outb(CRTPORT, 0x0F);
    outb(CRTPORT + 1, pos);
}

void console_putc(char c){
    if(panicked == 1){
        for(;;)
            ;
    }
    vga_putc(c);
}

#define INPUT_BUF 256
struct {
    char buffer[INPUT_BUF];
    uint16_t r;  // Indeks za čitanje
    uint16_t w;  // Indeks za pisanje
    uint16_t e;  // Indeks za poziciju za uređivanje (npr. backspace)
} input;

void console_intr(int32_t (*kbd_getc)(void)){
    acquire(&console.lock);

    char c;
    while((c = kbd_getc()) >= 0){
        if(c == 0)
            continue;
        else if(c == BACKSPACE){
            if(input.e == input.w) // if not available to delete
                continue;
            if(--input.e < 0) input.e = INPUT_BUF - 1;
        }else {
            if((input.e + 1) % INPUT_BUF == input.w)
                continue;
            input.buffer[input.e] = c;
            input.e = (input.e + 1) % INPUT_BUF;
            if(c == NEWLINE)
                input.w = input.e; // lock line
        }
        console_putc((char) c);
    }

    release(&console.lock);
}

void console_read(char *str, size_t n){ // should implement locking
    char *s = str;
    while(n > 0){
        while(input.r == input.w); // should sleep 

        char c = input.buffer[input.r++];
        input.r %= INPUT_BUF;

        *s++ = c;
        if(c == NEWLINE){
            break;
        }
        n--;
    }
    *s = '\0';
    puts(""); // <- magically works with this...
    return;
}

void puts(const char *str){
    for(int i = 0; i < strlen(str); i++){
        console_putc(str[i]);
    }
}

void console_init(){
    spinlock_init(&console.lock, "console");
    memset(input.buffer, 0, INPUT_BUF);
    input.r = input.w = input.e = 0;
}
