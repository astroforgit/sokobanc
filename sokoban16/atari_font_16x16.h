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
// Each tile uses 4 consecutive characters (TL, TR, BL, BR)

// Tile corner offsets - add these to the base tile code
#define TILE_TL  0  // Top-Left
#define TILE_TR  1  // Top-Right
#define TILE_BL  2  // Bottom-Left
#define TILE_BR  3  // Bottom-Right

// Base tile codes (each tile occupies 4 consecutive character codes)
#define TILE_WALL        0x40  // Wall tile (uses characters 0x40-0x43)
#define TILE_BOX         0x44  // Box tile (uses characters 0x44-0x47)
#define TILE_GOAL        0x48  // Goal tile (uses characters 0x48-0x4B)
#define TILE_BOXGOAL     0x4C  // Box on Goal tile (uses characters 0x4C-0x4F)
#define TILE_PLAYER      0x50  // Player tile (uses characters 0x50-0x53)
#define TILE_PLAYERGOAL  0x54  // Player on Goal tile (uses characters 0x54-0x57)

// Legacy compatibility - old _TL definitions point to base tile codes
#define TILE_WALL_TL        TILE_WALL
#define TILE_BOX_TL         TILE_BOX
#define TILE_GOAL_TL        TILE_GOAL
#define TILE_BOXGOAL_TL     TILE_BOXGOAL
#define TILE_PLAYER_TL      TILE_PLAYER
#define TILE_PLAYERGOAL_TL  TILE_PLAYERGOAL

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

