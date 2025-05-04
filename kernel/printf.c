#include "defs.h"

void printlong(printf_putcfn putc, int64_t num){
    if(num == 0){
        putc('0');
        return;
    }
    if(num < 0)
        putc('-');

    uint64_t n = (num < 0) ? -(uint64_t)num : (uint64_t)num;

    char buff[20];
    uint8_t i = 0;
    while(n > 0){
        buff[i++] = (n % 10) + '0';
        n /= 10;
    }
    while(i-- > 0){
        putc(buff[i]);
    }
}

void printulong(printf_putcfn putc, uint64_t num){
    if(num == 0){
        putc('0');
        return;
    }
    char buff[20];
    uint8_t i = 0;
    while(num > 0){
        buff[i++] = num % 10 + '0';
        num /= 10;
    }
    while(i-- > 0)
        putc(buff[i]);
}

void printhex(printf_putcfn putc, uint64_t num){
    putc('0');
    putc('x');
    if(num == 0){
        putc('0');
        return;
    }

    char hex_map[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    char buff[20];
    uint8_t i = 0;
    while(num > 0){
        buff[i++] = hex_map[num % 16];
        num /= 16;
    }
    while(i-- > 0)
        putc(buff[i]);
}

void fnprintf(printf_putcfn putc, const char* fmt, va_list args){
    char c;
    while((c = *fmt++) > 0){
        if(c != '%' || *fmt == '\0'){
            putc(c);
            continue;
        }
        c = *fmt++;
        switch(c){
            case 'c': // char
                putc((char) va_arg(args, int));
                break;
            case 'd': // int
                printlong(putc, va_arg(args, int));
                break;
            case 'l': // long
                printlong(putc, va_arg(args, long));
                break;
            case 'u': // unsigned
                if(*(fmt + 1) == 'l' && *fmt++) // if next is l and go next
                    printulong(putc, va_arg(args, long));
                printulong(putc, va_arg(args, long)); // for %u -> unsigned int
                break;
            case 'x': // hexa
            case 'p': // address
                printhex(putc, va_arg(args, int));
                break;
            default:
                break;
        }
    }
}
