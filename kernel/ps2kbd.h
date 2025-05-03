#ifndef PS2_KBD_H
#define PS2_KBD_H

#include "kbd.h"

#define SHIFT           1 << 0
#define ALT             1 << 1
#define CTRL            1 << 2
#define CAPS            1 << 3

static const uint8_t toggle_map[256] = {
    [0x3A] = CAPS
};

static const uint8_t mask_map[256] = {
    [0x2A] = SHIFT,
    [0x36] = SHIFT,
};

static const uint8_t scancode_map[256] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0,
};

static const uint8_t scancode_map_shift[256] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0, ' ', 0,
};

#endif
