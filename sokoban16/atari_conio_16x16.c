/*
  atari_conio_16x16.c - Console I/O library for 16x16 tiles
  Implementation file
*/

#include "atari_conio_16x16.h"

void my_clrscr_16x16(void) {
    // Clear entire screen memory
    memset(SCREEN_MEM, 0, CHAR_COLS * CHAR_ROWS);
}

void my_put_tile_16x16(byte tx, byte ty, byte tile_tl) {
    // Convert tile coordinates to character coordinates
    byte char_x = tx * 2;
    byte char_y = ty * 2;

    // Calculate screen memory offsets for all 4 character cells
    word offset_tl = (word)char_y * CHAR_COLS + char_x;           // Top-left
    word offset_tr = (word)char_y * CHAR_COLS + char_x + 1;       // Top-right
    word offset_bl = (word)(char_y + 1) * CHAR_COLS + char_x;     // Bottom-left
    word offset_br = (word)(char_y + 1) * CHAR_COLS + char_x + 1; // Bottom-right

    // If tile_tl is 0, draw empty space (all spaces)
    if (tile_tl == 0) {
        SCREEN_MEM[offset_tl] = 0;  // Space
        SCREEN_MEM[offset_tr] = 0;  // Space
        SCREEN_MEM[offset_bl] = 0;  // Space
        SCREEN_MEM[offset_br] = 0;  // Space
    } else {
        // Write all 4 characters that make up the tile
        SCREEN_MEM[offset_tl] = tile_tl;      // Top-left
        SCREEN_MEM[offset_tr] = tile_tl + 1;  // Top-right
        SCREEN_MEM[offset_bl] = tile_tl + 2;  // Bottom-left
        SCREEN_MEM[offset_br] = tile_tl + 3;  // Bottom-right
    }
}

void my_cputsxy_16x16(byte x, byte y, const char* str) {
    word offset = (word)y * CHAR_COLS + x;
    
    while (*str) {
        byte v = *str++;
        
        // Convert ASCII to ATASCII (Atari internal screen codes)
        if (v >= 0x20 && v <= 0x5f) {
            SCREEN_MEM[offset++] = v - 0x20;
        } else if (v >= 0x60 && v <= 0x7f) {
            SCREEN_MEM[offset++] = v - 0x60;
        }
    }
}

void my_cprintf_status_16x16(byte b, byte t, byte m) {
    char buf[41];
    sprintf(buf, "Boxes: %d/%d  Moves: %d", b, t, m);
    my_cputsxy_16x16(0, 23, buf);
}

void wait_vblank_16x16(void) {
    // Wait for vertical blank by monitoring the frame counter at $14
    asm("lda $14");
    __wait:
    asm("cmp $14");
    asm("beq %g", __wait);
}

