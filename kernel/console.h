#ifndef CONSOLE_H
#define CONSOLE_H

#define CRTPORT                 0x3d4
#define INPUT_BUF               256
#define VGA_CHAR_ATR            0x0700

struct input {
    char buffer[INPUT_BUF];
    uint16_t r;
    uint16_t w;
    uint16_t e;
};

struct console {
    struct input input;
    struct spinlock lock;
} console;

static bool panicked = false;

#endif
