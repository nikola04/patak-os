#include "defs.h"

#define STACK_CHK_GUARD 0x595DEA45E8725F46

__attribute__((noreturn)) void __stack_chk_fail(void) {
    panic("stack_chk: fail");
    while (1);
}

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
