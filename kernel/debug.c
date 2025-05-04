#include "defs.h"
#include "x86.h"

void e9putc(char c){
	outb(0xe9, (uint8_t) c);
}

void e9printf(const char* fmt, ...){
    va_list args;

    va_start(args, fmt);
    fnprintf(e9putc, fmt, args);
    va_end(args);
}
