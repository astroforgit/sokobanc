/*
  duplicator_conio_16x16.c - Console I/O for 16x16 duplicator tiles
  Implementation file
  
  This draws each game tile as a 2x2 block of the same character,
  making tiles appear 4x bigger (2x width, 2x height)
*/

#include "duplicator_conio_16x16.h"

void my_clrscr_16x16(void) {
    // Clear entire screen memory
    memset(SCREEN_MEM, 0, CHAR_COLS * CHAR_ROWS);
}

void my_cputcxy_16x16(byte tx, byte ty, byte tile_char) {
    // Convert tile coordinates to character coordinates
    // Each tile occupies 2x2 characters
    byte char_x = tx * 2;
    byte char_y = ty * 2;

    // Calculate screen memory offsets for all 4 character cells
    word offset_tl = (word)char_y * CHAR_COLS + char_x;           // Top-left
    word offset_tr = (word)char_y * CHAR_COLS + char_x + 1;       // Top-right
    word offset_bl = (word)(char_y + 1) * CHAR_COLS + char_x;     // Bottom-left
    word offset_br = (word)(char_y + 1) * CHAR_COLS + char_x + 1; // Bottom-right

    // Write the same character 4 times to create a 2x2 block
    SCREEN_MEM[offset_tl] = tile_char;
    SCREEN_MEM[offset_tr] = tile_char;
    SCREEN_MEM[offset_bl] = tile_char;
    SCREEN_MEM[offset_br] = tile_char;
}

void wait_vblank_16x16(void) {
    // Wait for vertical blank by monitoring the frame counter at $14
    asm("lda $14");
    __wait:
    asm("cmp $14");
    asm("beq %g", __wait);
}

