#ifndef KERNEL_x86_H
#define KERNEL_x86_H

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline unsigned char inb(uint16_t port) {
    unsigned char ret;
    __asm__ volatile ( "inb %1, %0"
                       : "=a"(ret)
                       : "Nd"(port) );
    return ret;
}

static inline void cli(void) {
	__asm__ volatile("cli");
}

static inline void hlt(void) {
    __asm__ volatile("hlt");
}

#endif
