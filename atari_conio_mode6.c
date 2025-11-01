/*
  atari_conio_mode6.c - Console I/O library for ANTIC Mode 6
  Implementation file
*/

#include "atari_conio_mode6.h"

void my_clrscr_mode6(void) {
    // Clear entire screen memory
    memset((void*)SCREEN_MEM, 0, CHAR_COLS * CHAR_ROWS);
}

void my_cputcxy_mode6(byte x, byte y, byte character) {
    word offset;
    
    // Bounds checking
    if (x >= CHAR_COLS || y >= CHAR_ROWS) {
        return;
    }
    
    // Calculate screen memory offset
    offset = (word)y * CHAR_COLS + x;
    
    // Write character to screen memory
    POKE(SCREEN_MEM + offset, character);
}

void my_cputsxy_mode6(byte x, byte y, const char* str) {
    word offset;
    byte v;
    
    // Bounds checking
    if (x >= CHAR_COLS || y >= CHAR_ROWS) {
        return;
    }
    
    offset = (word)y * CHAR_COLS + x;
    
    while (*str && x < CHAR_COLS) {
        v = *str++;
        
        // Convert ASCII to ATASCII (Atari internal screen codes)
        if (v >= 0x20 && v <= 0x5f) {
            POKE(SCREEN_MEM + offset, v - 0x20);
        } else if (v >= 0x60 && v <= 0x7f) {
            POKE(SCREEN_MEM + offset, v - 0x60);
        } else {
            POKE(SCREEN_MEM + offset, v);
        }
        
        offset++;
        x++;
    }
}

void my_cputsxy_color_mode6(byte x, byte y, const char* str, byte use_pf1) {
    word offset;
    byte v;
    
    // Bounds checking
    if (x >= CHAR_COLS || y >= CHAR_ROWS) {
        return;
    }
    
    offset = (word)y * CHAR_COLS + x;
    
    while (*str && x < CHAR_COLS) {
        v = *str++;
        
        // Convert ASCII to ATASCII (Atari internal screen codes)
        if (v >= 0x20 && v <= 0x5f) {
            v = v - 0x20;
        } else if (v >= 0x60 && v <= 0x7f) {
            v = v - 0x60;
        }
        
        // Add 128 to use PF1 color (green) instead of PF0 (brown)
        if (use_pf1) {
            v |= 0x80;
        }
        
        POKE(SCREEN_MEM + offset, v);
        
        offset++;
        x++;
    }
}

void my_cprintf_status_mode6(byte b, byte t, byte m) {
    char buf[21];  // 20 chars + null terminator
    
    // Format status string (max 20 characters for Mode 6)
    sprintf(buf, "Box:%d/%d Moves:%d", b, t, m);
    
    // Clear the status line first
    my_cputsxy_mode6(0, 23, "                    ");
    
    // Print the status (use PF0 color - brown)
    my_cputsxy_mode6(0, 23, buf);
}

void wait_vblank_mode6(void) {
    // Wait for vertical blank by monitoring the frame counter at $14
    asm("lda $14");
    __wait:
    asm("cmp $14");
    asm("beq %g", __wait);
}

