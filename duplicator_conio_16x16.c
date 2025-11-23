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

    // Write 4 DIFFERENT consecutive characters to create a 16x16 tile
    // tile_char is the top-left character code
    // The other 3 are consecutive: tile_char+1, tile_char+2, tile_char+3
    if (tile_char == 0 || tile_char == ' ') {
        // Empty space - write spaces
        SCREEN_MEM[offset_tl] = 0;
        SCREEN_MEM[offset_tr] = 0;
        SCREEN_MEM[offset_bl] = 0;
        SCREEN_MEM[offset_br] = 0;
    } else {
        SCREEN_MEM[offset_tl] = tile_char;      // Top-left
        SCREEN_MEM[offset_tr] = tile_char + 1;  // Top-right
        SCREEN_MEM[offset_bl] = tile_char + 2;  // Bottom-left
        SCREEN_MEM[offset_br] = tile_char + 3;  // Bottom-right
    }
}

void wait_vblank_16x16(void) {
    // Wait for vertical blank by monitoring the frame counter at $14
    asm("lda $14");
    __wait:
    asm("cmp $14");
    asm("beq %g", __wait);
}

// Provide my_cputcxy as a wrapper to my_cputcxy_16x16
// This is needed because duplicator_game.c calls my_cputcxy
// Maps game tile characters to 16x16 tile codes
void my_cputcxy(byte x, byte y, byte character) {
    byte tile_code = map_tile_to_16x16(character);
    my_cputcxy_16x16(x, y, tile_code);
}

