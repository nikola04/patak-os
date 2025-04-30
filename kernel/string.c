#include "defs.h"

int32_t atoi(const char* str){
    while(*str == ' ' || *str == '\t') // remove spaces
        str++;
    
    int sign = 1;
    int number = 0;
    if(*str == '-') {
        sign = -1;
        str++;
    }else if(*str == '+'){
        str++;
    }

    while(*str >= '0' && *str <= '9'){
        number = number * 10 + (*str - '0');
        str++;
    }
    number *= sign;
    return number;
}

size_t strlen(const char* str){
    const char* s = str;
    while(*s != '\0'){
        s++;
    }
    return s - str;
}

int32_t strcmp(const char* a, const char* b){
    while(*a != '\0' && *b != '\0'){
        if(*a != *b) return (int32_t)((unsigned char)*a - (unsigned char)*b); // 2ˆ16 enough to put ascii diff
        a++, b++;
    }
    return (int32_t)((unsigned char)*a - (unsigned char)*b); 
}

char* strcpy(char* dest, const char* src){
    char *d = dest;
    while((*d++ = *src++) != 0);
    *d = '\0';
    return dest;
}
