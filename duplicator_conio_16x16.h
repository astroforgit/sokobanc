/*
  duplicator_conio_16x16.h - Console I/O for 16x16 duplicator tiles
  Header file
*/

#ifndef DUPLICATOR_CONIO_16X16_H
#define DUPLICATOR_CONIO_16X16_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "duplicator_font_16x16.h"  // For TILE_TL, TILE_TR, TILE_BL, TILE_BR

// Data types
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions for 16x16 tiles
// 40x24 character screen = 20x12 tile grid
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

// Tile mapping function (defined in duplicator_tile_map_16x16.c)
byte map_tile_to_16x16(byte tile);

// Wrapper functions that duplicator_game.c expects
// These are implemented in duplicator_conio_16x16.c
void my_cputcxy(byte x, byte y, byte character);
void my_clrscr(void);
void my_cputsxy(byte x, byte y, const char* str);
void my_cprintf_status(byte b, byte t, byte m);
void wait_vblank(void);

#endif

