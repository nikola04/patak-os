#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "types.h"

struct spinlock {
    char* name;
    bool locked;
};

#endif
