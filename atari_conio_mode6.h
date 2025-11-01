/*
  atari_conio_mode6.h - Console I/O library for ANTIC Mode 6
  
  ANTIC Mode 6 (Graphics 1) provides:
  - 20 columns x 24 rows
  - 5 colors (background + 4 playfield colors)
  - Characters 0-127 use PF0 color
  - Characters 128-255 use PF1 color
*/

#ifndef ATARI_CONIO_MODE6_H
#define ATARI_CONIO_MODE6_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <peekpoke.h>
#include <atari.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Memory locations
#define SCREEN_MEM_MODE6 0x9000       // Screen memory location

// Screen dimensions for ANTIC Mode 6
#define CHAR_COLS_MODE6 20
#define CHAR_ROWS_MODE6 24

/*
  Clear the screen
  
  Fills screen memory with spaces (0)
*/
void my_clrscr_mode6(void);

/*
  Put a character at screen position with color control
  
  @param x - Column (0-19)
  @param y - Row (0-23)
  @param character - Character code (0-127 for PF0 color, 128-255 for PF1 color)
*/
void my_cputcxy_mode6(byte x, byte y, byte character);

/*
  Put a string at screen position
  
  @param x - Column (0-19)
  @param y - Row (0-23)
  @param str - Null-terminated string
  
  Note: In Mode 6, strings will use PF0 color by default
*/
void my_cputsxy_mode6(byte x, byte y, const char* str);

/*
  Put a colored string at screen position
  
  @param x - Column (0-19)
  @param y - Row (0-23)
  @param str - Null-terminated string
  @param use_pf1 - 0 for PF0 color (brown), 1 for PF1 color (green)
*/
void my_cputsxy_color_mode6(byte x, byte y, const char* str, byte use_pf1);

/*
  Print formatted status line
  
  @param b - Boxes on goals
  @param t - Total boxes
  @param m - Moves count
*/
void my_cprintf_status_mode6(byte b, byte t, byte m);

/*
  Wait for vertical blank
  
  Synchronizes to screen refresh (60Hz NTSC / 50Hz PAL)
*/
void wait_vblank_mode6(void);

#endif // ATARI_CONIO_MODE6_H

