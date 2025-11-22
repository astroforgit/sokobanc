/*
  duplicator_conio_16x16.h - Console I/O for 16x16 duplicator tiles
  Header file
*/

#ifndef DUPLICATOR_CONIO_16X16_H
#define DUPLICATOR_CONIO_16X16_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

// Function prototypes
void my_clrscr_16x16(void);
void my_cputcxy_16x16(byte tx, byte ty, byte tile_char);
void wait_vblank_16x16(void);

#endif

