#ifndef SPINLOCK_H
#define SPINLOCK_H

struct spinlock {
    char* name;
    int locked;
};

#endif
