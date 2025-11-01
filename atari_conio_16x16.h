/*
  atari_conio_16x16.h - Console I/O library for 16x16 tiles
  
  This library provides screen manipulation functions for 16x16 pixel tiles.
  Each tile occupies a 2x2 grid of character cells on screen.
  
  Screen layout with 16x16 tiles:
  - Physical screen: 40x24 characters
  - Logical tiles: 20x12 tiles (each tile = 2x2 characters)
*/

#ifndef ATARI_CONIO_16X16_H
#define ATARI_CONIO_16X16_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions in TILES (not characters)
#define TILE_COLS 20  // 40 characters / 2
#define TILE_ROWS 12  // 24 characters / 2

// Screen memory
#define SCREEN_MEM ((byte*)0x9000)

// Character screen dimensions
#define CHAR_COLS 40
#define CHAR_ROWS 24

/*
  Clear the screen
  Fills entire screen with empty tiles (character 0)
*/
void my_clrscr_16x16(void);

/*
  Put a 16x16 tile at the specified tile position
  
  @param tx - Tile X coordinate (0-19)
  @param ty - Tile Y coordinate (0-11)
  @param tile_tl - Top-left character of the tile
  
  Note: The function assumes tiles use 4 consecutive characters:
  tile_tl, tile_tl+1, tile_tl+2, tile_tl+3
*/
void my_put_tile_16x16(byte tx, byte ty, byte tile_tl);

/*
  Put a string of text at character position (for status line, etc.)
  This works with regular 8x8 characters, not tiles
  
  @param x - Character X coordinate (0-39)
  @param y - Character Y coordinate (0-23)
  @param str - String to display
*/
void my_cputsxy_16x16(byte x, byte y, const char* str);

/*
  Print formatted status line
  
  @param b - Boxes on goals
  @param t - Total boxes
  @param m - Moves count
*/
void my_cprintf_status_16x16(byte b, byte t, byte m);

/*
  Wait for vertical blank (screen refresh)
  Synchronizes to 60Hz NTSC / 50Hz PAL
*/
void wait_vblank_16x16(void);

#endif // ATARI_CONIO_16X16_H

