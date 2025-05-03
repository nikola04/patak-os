#ifndef KERNEL_x86_H
#define KERNEL_x86_H

#include "types.h"

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline unsigned char inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                       : "=a"(ret)
                       : "Nd"(port) );
    return ret;
}

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void load_cr3(uint64_t pml4_addr) {
    __asm__ volatile (
        "mov %0, %%cr3"      // Premesti PML4 u CR3
        :                    // Nema izlaznih operacija
        : "r" (pml4_addr)    // Ulazni operand: fizička adresa PML4
    );
}

static inline __attribute__((noreturn)) void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

static inline void cli(void) {
	__asm__ volatile("cli");
}

static inline void hlt(void) {
    __asm__ volatile("hlt");
}

static inline void sti(void) {
    __asm__ volatile ("sti");
}

#endif
