#include "satanic_conio_16x16.h"

void my_clrscr_16x16(void) {
    memset(SCREEN_MEM, 0, CHAR_COLS * CHAR_ROWS);
}

void my_cputcxy_16x16(byte tx, byte ty, byte tile_char) {
    byte char_x = tx * 2;
    byte char_y = ty * 2;

    word offset_tl = (word)char_y * CHAR_COLS + char_x;
    word offset_tr = (word)char_y * CHAR_COLS + char_x + 1;
    word offset_bl = (word)(char_y + 1) * CHAR_COLS + char_x;
    word offset_br = (word)(char_y + 1) * CHAR_COLS + char_x + 1;

    if (tile_char == 0) {
        SCREEN_MEM[offset_tl] = 0;
        SCREEN_MEM[offset_tr] = 0;
        SCREEN_MEM[offset_bl] = 0;
        SCREEN_MEM[offset_br] = 0;
    } else {
        SCREEN_MEM[offset_tl] = tile_char + TILE_TL;
        SCREEN_MEM[offset_tr] = tile_char + TILE_TR;
        SCREEN_MEM[offset_bl] = tile_char + TILE_BL;
        SCREEN_MEM[offset_br] = tile_char + TILE_BR;
    }
}

void wait_vblank_16x16(void) {
    asm("lda $14");
    __wait:
    asm("cmp $14");
    asm("beq %g", __wait);
}

