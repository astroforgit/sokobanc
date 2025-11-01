/*
  sokoban_game_mode6.h - Sokoban game logic library for ANTIC Mode 6
  
  This library contains all the game logic for Sokoban using ANTIC Mode 6.
  Mode 6 provides 20x24 character grid with 5 colors.
*/

#ifndef SOKOBAN_GAME_MODE6_H
#define SOKOBAN_GAME_MODE6_H

#include <stdlib.h>
#include <string.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions in characters for ANTIC Mode 6
#define CHAR_COLS 20
#define CHAR_ROWS 24

// Game Tile Definitions (same as other versions for level data compatibility)
#define TILE_EMPTY       0
#define TILE_WALL       '#'
#define TILE_GOAL       '.'
#define TILE_BOX        '$'
#define TILE_BOX_ON_GOAL '*'
#define TILE_PLAYER     '@'
#define TILE_PLAYER_ON_GOAL '+'

/*
  Load a level from string array data
  
  @param d - Array of strings representing the level layout
  @param r_num - Number of rows in the level
*/
void load_level_mode6(const char** d, byte r_num);

/*
  Draw the entire level to the screen using colored characters
  Clears the screen and redraws all tiles and the player
*/
void draw_level_mode6(void);

/*
  Update the status line showing game statistics
*/
void update_status_mode6(void);

/*
  Try to move the player in a given direction
  Handles wall collision, box pushing, and goal detection
  
  @param dx - Delta X (-1 for left, 1 for right, 0 for no horizontal movement)
  @param dy - Delta Y (-1 for up, 1 for down, 0 for no vertical movement)
*/
void try_move_player_mode6(signed char dx, signed char dy);

/*
  Get the player's X coordinate (in characters)
  
  @return Player X position
*/
byte get_player_x_mode6(void);

/*
  Get the player's Y coordinate (in characters)
  
  @return Player Y position
*/
byte get_player_y_mode6(void);

/*
  Get the total number of boxes in the level
  
  @return Total boxes count
*/
byte get_total_boxes_mode6(void);

/*
  Get the number of boxes currently on goals
  
  @return Boxes on goals count
*/
byte get_boxes_on_goals_mode6(void);

/*
  Get the current move count
  
  @return Number of moves made
*/
byte get_moves_mode6(void);

/*
  Check if the level is complete (all boxes on goals)
  
  @return 1 if level is complete, 0 otherwise
*/
byte is_level_complete_mode6(void);

#endif // SOKOBAN_GAME_MODE6_H

