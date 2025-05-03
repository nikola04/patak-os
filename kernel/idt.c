#include "defs.h"
#include "idt.h"
#include "x86.h"

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];


void idt_init() {
    static idtr_t idtr;
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1; // max 256

    idt_set_descriptor(0x21, isr_keyboard, 0x8E);

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT

    sti();
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}
