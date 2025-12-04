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
    // tile_char is the base tile code (e.g., TILE_WALL)
    // The 4 corners are: tile_char+TILE_TL, +TILE_TR, +TILE_BL, +TILE_BR
    // Since TILE_TL=0, TILE_TR=1, TILE_BL=2, TILE_BR=3, this simplifies to:
    // tile_char+0, tile_char+1, tile_char+2, tile_char+3
    if (tile_char == 0) {
        // Empty space - write zeros
        SCREEN_MEM[offset_tl] = 0;
        SCREEN_MEM[offset_tr] = 0;
        SCREEN_MEM[offset_bl] = 0;
        SCREEN_MEM[offset_br] = 0;
    } else {
        SCREEN_MEM[offset_tl] = tile_char + TILE_TL;  // Top-left (tile_char+0)
        SCREEN_MEM[offset_tr] = tile_char + TILE_TR;  // Top-right (tile_char+1)
        SCREEN_MEM[offset_bl] = tile_char + TILE_BL;  // Bottom-left (tile_char+2)
        SCREEN_MEM[offset_br] = tile_char + TILE_BR;  // Bottom-right (tile_char+3)
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

// Wrapper for my_clrscr
void my_clrscr(void) {
    my_clrscr_16x16();
}

// Stub for my_cputsxy (not used in duplicator game)
void my_cputsxy(byte x, byte y, const char* str) {
    x = x; y = y; str = str; // Suppress warnings
}

// Stub for my_cprintf_status (not used in duplicator game)
void my_cprintf_status(byte b, byte t, byte m) {
    b = b; t = t; m = m; // Suppress warnings
}

// Wrapper for wait_vblank
void wait_vblank(void) {
    wait_vblank_16x16();
}

