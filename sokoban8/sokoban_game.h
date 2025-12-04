/*
  sokoban_game.h - Sokoban game logic library
  
  This library contains all the game logic for Sokoban including
  level loading, drawing, player movement, and win condition checking.
*/

#ifndef SOKOBAN_GAME_H
#define SOKOBAN_GAME_H

#include <stdlib.h>
#include <string.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions
#define COLS 40
#define ROWS 24

// Game Tile Definitions
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
void load_level(const char** d, byte r_num);

/*
  Draw the entire level to the screen
  Clears the screen and redraws all tiles and the player
*/
void draw_level(void);

/*
  Update the status line showing game statistics
*/
void update_status(void);

/*
  Try to move the player in a given direction
  Handles wall collision, box pushing, and goal detection
  
  @param dx - Delta X (-1 for left, 1 for right, 0 for no horizontal movement)
  @param dy - Delta Y (-1 for up, 1 for down, 0 for no vertical movement)
*/
void try_move_player(signed char dx, signed char dy);

/*
  Get the player's X coordinate
  
  @return Player X position
*/
byte get_player_x(void);

/*
  Get the player's Y coordinate
  
  @return Player Y position
*/
byte get_player_y(void);

/*
  Get the total number of boxes in the level
  
  @return Total boxes count
*/
byte get_total_boxes(void);

/*
  Get the number of boxes currently on goals
  
  @return Boxes on goals count
*/
byte get_boxes_on_goals(void);

/*
  Get the current move count
  
  @return Number of moves made
*/
byte get_moves(void);

/*
  Check if the level is complete (all boxes on goals)
  
  @return 1 if level is complete, 0 otherwise
*/
byte is_level_complete(void);

#endif // SOKOBAN_GAME_H

