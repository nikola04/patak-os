#ifndef DEFS_H
#define DEFS_H

#include "types.h"
#include "spinlock.h"

// console.c
void consolewrite(const char *, size_t);
void consoleinit();
void panic(const char *);

// spinlock.c
void spinlockinit(struct spinlock *, const char *);
void acquire(struct spinlock *);
void release(struct spinlock *);

// string.c
int64_t atoi(const char *);
size_t strlen(const char *);
int32_t strcmp(const char *, const char *);
char* strcpy(char *, const char *);

#endif // DEFS_H
