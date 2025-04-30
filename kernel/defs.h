#ifndef DEFS_H
#define DEFS_H

#include "spinlock.h"

// console.c
void consolewrite(const char *, int);
void consoleinit();
void panic(const char *);

// spinlock.c
void spinlockinit(struct spinlock *, const char *);
void acquire(struct spinlock *);
void release(struct spinlock *);

// string.c
int atoi(const char *);
int strlen(const char *);
int strcmp(const char *, const char *);
char* strcpy(char *, const char *);

#endif // DEFS_H
