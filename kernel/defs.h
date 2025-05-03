#ifndef DEFS_H
#define DEFS_H

#include "types.h"
#include "spinlock.h"

// console.c
void                    puts(const char *);
void                    console_intr(int32_t (*)(void));
void                    console_init();
void                    console_read(char *, size_t);
void                    panic(const char *);

// spinlock.c
void                    spinlock_init(struct spinlock *, const char *);
void                    acquire(struct spinlock *);
void                    release(struct spinlock *);

// string.c
int64_t                 atoi(const char *);
char *                  itoa(char *, int64_t);
size_t                  strlen(const char *);
int32_t                 strcmp(const char *, const char *);
char *                  strcpy(char *, const char *);
void *                  memset(void *dest, int ch, size_t count);

// idt.c
void                    idt_init();

// pic.c
void                    pic_remap(void);

// .asm
extern void             isr_keyboard(void);

#endif // DEFS_H
