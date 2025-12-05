#ifndef SATANIC_CONIO_16X16_H
#define SATANIC_CONIO_16X16_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "satanic_font_16x16.h"  // For TILE_TL, TILE_TR, TILE_BL, TILE_BR

// Data types
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions for 16x16 tiles
#define CHAR_COLS 40
#define CHAR_ROWS 24
#define TILE_COLS 20
#define TILE_ROWS 12

// Memory locations
#define SCREEN_MEM  ((byte*)0x9000)

// Function prototypes for 16x16 mode
void my_clrscr_16x16(void);
void my_cputcxy_16x16(byte tx, byte ty, byte tile_char);
void wait_vblank_16x16(void);

// Tile mapping function
byte map_tile_to_16x16(byte tile);

#endif

