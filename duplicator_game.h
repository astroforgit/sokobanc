/*
  duplicator_game.h - Game logic for Duplicator game
  
  This library handles game state, level loading, player movement,
  and win conditions.
*/

#ifndef DUPLICATOR_GAME_H
#define DUPLICATOR_GAME_H

#include <stdlib.h>
#include <string.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Game constants
#define MAX_LEVEL_WIDTH 40
#define MAX_LEVEL_HEIGHT 24

// Tile types
#define TILE_EMPTY      ' '
#define TILE_WALL       '#'
#define TILE_PLAYER     '@'
#define TILE_CRATE      '*'
#define TILE_KEY        'k'
#define TILE_DOOR       'd'
#define TILE_ENEMY      'e'
#define TILE_HOLE_A     '?'
#define TILE_HOLE_B     '!'
#define TILE_PLATE_A    'b'
#define TILE_PLATE_B    'c'
#define TILE_GATE_A     'g'
#define TILE_GATE_B     'h'
#define TILE_EXIT_A     ':'
#define TILE_EXIT_B     ';'
#define TILE_FLOOR      '.'

// Game state structure
typedef struct {
    byte player_x;
    byte player_y;
    byte level_width;
    byte level_height;
    byte moves;
    byte level_complete;
    byte current_level;
} GameState;

/*
  Load a level from string array
  
  @param level_data - Array of strings representing the level
  @param num_rows - Number of rows in the level
*/
void load_level(const char* level_data[], byte num_rows);

/*
  Draw the entire level to screen
*/
void draw_level(void);

/*
  Try to move the player in the given direction
  
  @param dx - Horizontal movement (-1, 0, or 1)
  @param dy - Vertical movement (-1, 0, or 1)
  @return 1 if move was successful, 0 otherwise
*/
byte try_move_player(char dx, char dy);

/*
  Check if the level is complete (player reached exit)
  
  @return 1 if level is complete, 0 otherwise
*/
byte is_level_complete(void);

/*
  Get the current game state
  
  @return Pointer to the game state structure
*/
GameState* get_game_state(void);

/*
  Get the tile at a specific position
  
  @param x - X coordinate
  @param y - Y coordinate
  @return The tile character at that position
*/
byte get_tile(byte x, byte y);

/*
  Set the tile at a specific position
  
  @param x - X coordinate
  @param y - Y coordinate
  @param tile - The tile character to set
*/
void set_tile(byte x, byte y, byte tile);

#endif // DUPLICATOR_GAME_H

