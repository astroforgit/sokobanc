/*
  atari_font_16x16.h - 16x16 tile font and graphics library
  
  This library handles 16x16 pixel tiles using 4 character cells (2x2 grid).
  Each 16x16 tile is composed of 4 8x8 characters arranged as:
  
  [Top-Left]    [Top-Right]
  [Bottom-Left] [Bottom-Right]
*/

#ifndef ATARI_FONT_16X16_H
#define ATARI_FONT_16X16_H

#include <stdlib.h>
#include <string.h>
#include <atari.h>
#include <peekpoke.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Memory Layout
#define CHARSET_MEM ((byte*)0x7000)
#define DLIST_MEM   ((byte*)0x8000)
#define ROM_CHARSET_ADDRESS 0xE000

// 16x16 Tile Character Mappings
// Each tile uses 4 consecutive characters

// Wall tile (uses characters 0x40-0x43)
#define TILE_WALL_TL  0x40  // Top-Left
#define TILE_WALL_TR  0x41  // Top-Right
#define TILE_WALL_BL  0x42  // Bottom-Left
#define TILE_WALL_BR  0x43  // Bottom-Right

// Box tile (uses characters 0x44-0x47)
#define TILE_BOX_TL   0x44
#define TILE_BOX_TR   0x45
#define TILE_BOX_BL   0x46
#define TILE_BOX_BR   0x47

// Goal tile (uses characters 0x48-0x4B)
#define TILE_GOAL_TL  0x48
#define TILE_GOAL_TR  0x49
#define TILE_GOAL_BL  0x4A
#define TILE_GOAL_BR  0x4B

// Box on Goal tile (uses characters 0x4C-0x4F)
#define TILE_BOXGOAL_TL  0x4C
#define TILE_BOXGOAL_TR  0x4D
#define TILE_BOXGOAL_BL  0x4E
#define TILE_BOXGOAL_BR  0x4F

// Player tile (uses characters 0x50-0x53)
#define TILE_PLAYER_TL  0x50
#define TILE_PLAYER_TR  0x51
#define TILE_PLAYER_BL  0x52
#define TILE_PLAYER_BR  0x53

// Player on Goal tile (uses characters 0x54-0x57)
#define TILE_PLAYERGOAL_TL  0x54
#define TILE_PLAYERGOAL_TR  0x55
#define TILE_PLAYERGOAL_BL  0x56
#define TILE_PLAYERGOAL_BR  0x57

/*
  Initialize custom graphics mode with 16x16 tile character set
  Sets up display list, copies ROM charset, and installs custom 16x16 graphics
*/
void setup_graphics_16x16(void);

/*
  Animate the player character (16x16 tile)
  Updates all 4 character cells that make up the player sprite
*/
void animate_player_16x16(void);

/*
  Get current animation state
  @return 0 = idle, 1 = walking
*/
byte get_anim_state_16x16(void);

/*
  Set animation state
  @param state - 0 = idle, 1 = walking
*/
void set_anim_state_16x16(byte state);

/*
  Get current frame counter
  @return Current frame number
*/
byte get_frame_counter_16x16(void);

/*
  Set frame counter
  @param counter - Frame number to set
*/
void set_frame_counter_16x16(byte counter);

/*
  Increment frame counter
  Increments the animation frame counter
*/
void increment_frame_counter_16x16(void);

#endif // ATARI_FONT_16X16_H

