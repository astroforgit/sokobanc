/*
  atari_conio.h - Alternative console I/O library for Atari 8-bit
  
  This library provides custom screen manipulation functions that work
  directly with screen memory for better performance and control.
*/

#ifndef ATARI_CONIO_H
#define ATARI_CONIO_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions
#define COLS 40
#define ROWS 24

// Manual Memory Layout
#define SCREEN_MEM  ((byte*)0x9000)

/*
  Clear the screen by filling screen memory with zeros
*/
void my_clrscr(void);

/*
  Put a character at specific x,y coordinates
  
  @param x - Column position (0-39)
  @param y - Row position (0-23)
  @param character - Character code to display
*/
void my_cputcxy(byte x, byte y, byte character);

/*
  Put a string at specific x,y coordinates
  Handles ASCII to ATASCII conversion
  
  @param x - Column position (0-39)
  @param y - Row position (0-23)
  @param str - Null-terminated string to display
*/
void my_cputsxy(byte x, byte y, const char* str);

/*
  Print formatted status line showing game statistics
  
  @param b - Boxes on goals count
  @param t - Total boxes count
  @param m - Moves count
*/
void my_cprintf_status(byte b, byte t, byte m);

/*
  Wait for vertical blank (screen refresh)
  This synchronizes the game loop to 60Hz (NTSC) or 50Hz (PAL)
*/
void wait_vblank(void);

#endif // ATARI_CONIO_H

