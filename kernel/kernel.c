// kernel.c - patakOS
// Copyright (C) 2025 Nikola Nedeljkovic
// This program is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License, version 3, or (at your option) any later version.
// You can freely share and/or modify this program under the terms of GPL-3.
// See the full license text at: https://www.gnu.org/licenses/gpl-3.0.html
#include "defs.h"

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
