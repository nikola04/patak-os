#include "kernel.h"

void kmain(){
    const char *message = "Hello from kernel C!\n";
    char *video_memory = (char *)0xb8000;

    for (int i = 0; message[i] != '\0'; i++) {
        video_memory[i * 2] = message[i];
        video_memory[i * 2 + 1] = 0x07; // White on black
    }
    while(1);
    return;
}
