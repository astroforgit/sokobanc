/*
  atari_font.h - Custom font and graphics library for Atari 8-bit
  
  This library handles character set redefinition and custom graphics
  for game sprites and tiles.
*/

#ifndef ATARI_FONT_H
#define ATARI_FONT_H

#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Manual Memory Layout
#define CHARSET_MEM ((byte*)0x7000)
#define DLIST_MEM   ((byte*)0x8000)
#define SCREEN_MEM  ((byte*)0x9000)
#define ROM_CHARSET_ADDRESS 0xE000

// Screen dimensions
#define COLS 40
#define ROWS 24

/*
  Setup custom graphics mode with character set redefinition
  
  This function:
  - Creates a custom display list
  - Copies ROM character set to RAM
  - Installs custom graphics for game tiles
  - Sets up colors
*/
void setup_graphics(void);

/*
  Animate the player character
  
  This function updates the player character graphics in the character set
  to create walking and blinking animations. It modifies both '@' and '+'
  characters (player on empty space and player on goal).
  
  Should be called every frame from the main game loop.
*/
void animate_player(void);

/*
  Get the current animation state
  
  @return 0 for idle animation, 1 for walking animation
*/
byte get_anim_state(void);

/*
  Set the animation state
  
  @param state - 0 for idle, 1 for walking
*/
void set_anim_state(byte state);

/*
  Get the current frame counter
  
  @return Current frame counter value
*/
byte get_frame_counter(void);

/*
  Set the frame counter
  
  @param counter - Frame counter value to set
*/
void set_frame_counter(byte counter);

/*
  Increment the frame counter
  Called once per frame from the main loop
*/
void increment_frame_counter(void);

#endif // ATARI_FONT_H

