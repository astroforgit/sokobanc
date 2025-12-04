/*
  atari_conio.c - Alternative console I/O library for Atari 8-bit
  Implementation file
*/

#include "atari_conio.h"

void my_clrscr(void) {
    memset(SCREEN_MEM, 0, COLS * ROWS);
}

void my_cputcxy(byte x, byte y, byte character) {
    word offset = (word)y * COLS + x;
    SCREEN_MEM[offset] = character;
}

void my_cputsxy(byte x, byte y, const char* str) {
    word offset = (word)y * COLS + x;
    while (*str) {
        byte v = *str++;
        // Convert ASCII to ATASCII (internal screen codes)
        if (v >= 0x20 && v <= 0x5f) {
            SCREEN_MEM[offset++] = v - 0x20;
        } else if (v >= 0x60 && v <= 0x7f) {
            SCREEN_MEM[offset++] = v - 0x60;
        }
    }
}

void my_cprintf_status(byte b, byte t, byte m) {
    char buf[41];
    sprintf(buf, "Boxes: %d/%d  Moves: %d", b, t, m);
    my_cputsxy(0, 23, buf);
}

void wait_vblank(void) {
    // Wait for vertical blank by monitoring the frame counter at $14
    // This ensures smooth animation and prevents screen tearing
    asm("lda $14");
    __wait:
    asm("cmp $14");
    asm("beq %g", __wait);
}

