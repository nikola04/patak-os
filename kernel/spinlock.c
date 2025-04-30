#include "spinlock.h"
#include "defs.h"

void spinlockinit(struct spinlock *spinlock, const char *name){
    strcpy(spinlock->name, name);
    spinlock->locked = 0;
}

int holding(struct spinlock *spinlock) {
    return spinlock->locked != 0;
}

void acquire(struct spinlock *spinlock) {
    if(holding(spinlock)){
        panic("spinlock already aquired");
    }
    spinlock->locked = 1;
}

void release(struct spinlock *spinlock) {
    spinlock->locked = 0;
}
