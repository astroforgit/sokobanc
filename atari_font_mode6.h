/*
  atari_font_mode6.h - ANTIC Mode 6 (Graphics 1) color graphics library
  
  ANTIC Mode 6 provides:
  - 20 columns x 24 rows
  - 5 colors (background + 4 playfield colors)
  - Each character can use one of two color pairs:
    * Characters 0-127: Use PF0 color
    * Characters 128-255: Use PF1 color
  
  Color scheme for Sokoban:
  - Background: Black
  - PF0: Brown (walls, boxes)
  - PF1: Green (player)
  - PF2: Yellow (goals)
  - PF3: Red (boxes on goals)
*/

#ifndef ATARI_FONT_MODE6_H
#define ATARI_FONT_MODE6_H

#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <atari.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Memory locations
#define CHARSET_MEM_MODE6 0x7000      // Custom character set location
#define DLIST_MEM_MODE6   0x8000      // Display list location
#define SCREEN_MEM_MODE6  0x9000      // Screen memory location

// Screen dimensions for ANTIC Mode 6
#define CHAR_COLS_MODE6 20
#define CHAR_ROWS_MODE6 24

// Atari color values (hue + luminance) for Mode 6
#define MODE6_COLOR_BLACK     0x00
#define MODE6_COLOR_BROWN     0x14    // Brown for walls/boxes
#define MODE6_COLOR_GREEN     0xC4    // Green for player
#define MODE6_COLOR_YELLOW    0xE8    // Yellow for goals
#define MODE6_COLOR_RED       0x32    // Red for boxes on goals
#define MODE6_COLOR_ORANGE    0x28    // Orange alternative
#define MODE6_COLOR_BLUE      0x96    // Blue alternative

// Character definitions for game elements
// ANTIC Mode 6 color mapping:
// Characters 0-63:    Use PF0 color (brown) - walls, boxes
// Characters 64-127:  Use PF2 color (yellow) - goals
// Characters 128-191: Use PF1 color (green) - player
// Characters 192-255: Use PF3 color (red) - boxes on goals

// Custom character codes for special graphics
#define CHAR_WALL_CUSTOM       0x40    // PF0 (brown) - character 64
#define CHAR_BOX_CUSTOM        0x41    // PF0 (brown) - character 65
#define CHAR_GOAL_CUSTOM       (64 + 0x02)  // PF2 (yellow) - character 66
#define CHAR_BOXGOAL_CUSTOM    (192 + 0x03) // PF3 (red) - character 195
#define CHAR_PLAYER_CUSTOM     (128 + 0x00) // PF1 (green) - character 128
#define CHAR_PLAYERGOAL_CUSTOM (128 + 0x01) // PF1 (green) - character 129

/*
  Setup ANTIC Mode 6 graphics and colors
  
  Initializes:
  - Display list for ANTIC Mode 6
  - Color registers
  - Custom character set (optional)
*/
void setup_graphics_mode6(void);

/*
  Animate the player character
  
  Cycles through animation frames:
  - Standing (normal)
  - Walking (when moving)
  - Blinking (idle animation)
*/
void animate_player_mode6(void);

/*
  Increment the frame counter for animation timing
*/
void increment_frame_counter_mode6(void);

/*
  Set the animation state
  
  @param state - 0: standing, 1: walking, 2: blinking
*/
void set_anim_state_mode6(byte state);

/*
  Set the frame counter
  
  @param frame - Frame number
*/
void set_frame_counter_mode6(byte frame);

#endif // ATARI_FONT_MODE6_H

