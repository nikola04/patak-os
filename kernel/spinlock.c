#include "spinlock.h"
#include "defs.h"

void spinlock_init(struct spinlock *spinlock, const char *name){
    strcpy(spinlock->name, name);
    spinlock->locked = false;
}

int holding(struct spinlock *spinlock) {
    return spinlock->locked == true;
}

void acquire(struct spinlock *spinlock) {
    while(holding(spinlock));
    spinlock->locked = true;
}

void release(struct spinlock *spinlock) {
    spinlock->locked = false;
}
