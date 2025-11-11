/*
  test_conio.h - Mock console I/O library for testing duplicator game on modern computers
  
  This library provides the same interface as atari_conio.h but works on any platform
  with standard C. It maintains an internal screen buffer for testing.
*/

#ifndef TEST_CONIO_H
#define TEST_CONIO_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Data types for clarity (same as Atari version)
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions (same as Atari)
#define COLS 40
#define ROWS 24

// Screen memory buffer (simulated)
extern byte test_screen_buffer[ROWS][COLS];

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
  
  @param x - Column position (0-39)
  @param y - Row position (0-23)
  @param str - Null-terminated string to display
*/
void my_cputsxy(byte x, byte y, const char* str);

/*
  Print formatted status line showing game statistics
  (Not used in duplicator game, but provided for compatibility)
  
  @param b - Boxes on goals count
  @param t - Total boxes count
  @param m - Moves count
*/
void my_cprintf_status(byte b, byte t, byte m);

/*
  Wait for vertical blank (screen refresh)
  In test mode, this does nothing
*/
void wait_vblank(void);

/*
  Print the screen buffer to stdout (for debugging)
*/
void print_screen(void);

/*
  Get character at specific position (for testing)
  
  @param x - Column position (0-39)
  @param y - Row position (0-23)
  @return Character at that position
*/
byte get_screen_char(byte x, byte y);

#endif // TEST_CONIO_H

