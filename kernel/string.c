#include "defs.h"

void reverse(char* str){
    int n = strlen(str);
    for(int i = 0; i < n / 2; i++){
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int64_t atoi(const char* str){
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

char *itoa(char * dest, int64_t number)
{
    char *s = dest;
    if(number == 0){
        *s++ = '0';
        *s = '\0';
        return dest;
    }

    bool negative = false;
    if(number < 0){
        negative = true;
        number *= -1;
    }

    while(number != 0){
        *s++ = '0' + number % 10;
        number /= 10;
    }

    if(negative)
        *s++ = '-';
    *s = '\0';
    reverse(dest);
    return dest;
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

void *memset(void *dest, int ch, size_t count) {
    uint8_t *d = dest;
    while (count--) {
        *d++ = (uint8_t)ch;
    }
    return dest;
}
