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

// Game constants (exact size needed for levels)
#define MAX_LEVEL_WIDTH 18   // Exact width of largest level (17 + 1)
#define MAX_LEVEL_HEIGHT 11  // Exact height of levels
#define SCREEN_TOP_MARGIN 2  // Number of lines reserved for title/UI at top

// Tile types
#define TILE_EMPTY      ' '
#define TILE_WALL       '#'
#define TILE_PLAYER     'p'
#define TILE_CRATE      '*'
#define TILE_KEY        'k'
#define TILE_DOOR       'd'
#define TILE_DOOR_OPEN  'D'
#define TILE_ENEMY      'e'
#define TILE_HOLE_A     '?'
#define TILE_HOLE_B     '!'
#define TILE_PLATE_A    'b'
#define TILE_PLATE_B    'c'
#define TILE_GATE_A     'g'
#define TILE_GATE_B     'h'
#define TILE_EXIT_A     '@'
#define TILE_EXIT_B     ':'
#define TILE_EXIT_C     ';'
#define TILE_FLOOR      '.'

// Tile categories (bit flags)
#define TILE_CAT_PASSABLE    0x01  // Can walk through
#define TILE_CAT_BLOCKING    0x02  // Blocks movement
#define TILE_CAT_PUSHABLE    0x04  // Can be pushed
#define TILE_CAT_EXIT        0x08  // Win condition
#define TILE_CAT_HOLE        0x10  // Duplication hole
#define TILE_CAT_PLATE       0x20  // Pressure plate
#define TILE_CAT_GATE        0x40  // Gate (open/closed)

// Maximum players (optimized for memory)
#define MAX_PLAYERS 6  // Allows up to 2 duplications (1->2->4, or 1->2->3->4->5->6)
#define MAX_OBJECTS 8  // Max pushable objects (keys, crates, enemies)

// Player structure (compact)
typedef struct {
    byte x;
    byte y;
    char under;  // What tile is underneath
} Player;

// Pushable object structure (for keys, crates, enemies)
typedef struct {
    byte x;
    byte y;
    char type;   // 'k', '*', 'e'
    char under;  // What tile is underneath
} PushableObject;

// Game state structure with multiple players
typedef struct {
    Player players[MAX_PLAYERS];
    byte num_players;
    PushableObject objects[MAX_OBJECTS];
    byte num_objects;
    byte level_width;
    byte level_height;
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

/*
  Check if a tile blocks movement

  @param tile - The tile character to check
  @return 1 if tile blocks movement, 0 otherwise
*/
byte is_blocking(char tile);

/*
  Check if a tile is passable

  @param tile - The tile character to check
  @return 1 if tile is passable, 0 otherwise
*/
byte is_passable(char tile);

/*
  Tile category lookup table - external declaration
  Defined in duplicator_game.c
*/
extern const byte tile_categories[256];

/*
  Check if a tile is an exit - INLINE MACRO using category table
*/
#define is_exit(tile) ((tile_categories[(byte)(tile)] & TILE_CAT_EXIT) != 0)

/*
  Check if a tile is pushable - INLINE MACRO using category table
*/
#define is_pushable(tile) ((tile_categories[(byte)(tile)] & TILE_CAT_PUSHABLE) != 0)

/*
  Check if a tile is a hole - INLINE MACRO using category table
*/
#define is_hole(tile) ((tile_categories[(byte)(tile)] & TILE_CAT_HOLE) != 0)

/*
  Check if a tile is a pressure plate - INLINE MACRO using category table
*/
#define is_plate(tile) ((tile_categories[(byte)(tile)] & TILE_CAT_PLATE) != 0)

/*
  Check if a tile is a gate - INLINE MACRO using category table
*/
#define is_gate(tile) ((tile_categories[(byte)(tile)] & TILE_CAT_GATE) != 0)

/*
  Try to push an object at a position in a direction

  @param x - X coordinate of object to push
  @param y - Y coordinate of object to push
  @param dx - Horizontal push direction (-1, 0, or 1)
  @param dy - Vertical push direction (-1, 0, or 1)
  @return 1 if push was successful, 0 otherwise
*/
byte try_push(byte x, byte y, char dx, char dy);

/*
  Flood fill to spread door_open state to adjacent doors

  @param x - X coordinate of starting door
  @param y - Y coordinate of starting door
*/
void door_flood_fill(byte x, byte y);

/*
  Remove all open doors (door_) from the level
*/
void remove_open_doors(void);

/*
  Handle key touching door interaction

  @param key_x - X coordinate of key
  @param key_y - Y coordinate of key
  @param door_x - X coordinate of door
  @param door_y - Y coordinate of door
*/
void handle_key_door(byte key_x, byte key_y, byte door_x, byte door_y);

/*
  Update gate states based on plate activation

  Checks all plates and opens/closes corresponding gates:
  - plateA with object -> gateA opens
  - plateA without object -> gateA closes
  - plateB with object -> gateB opens
  - plateB without object -> gateB closes
*/
void update_gates(void);

/*
  Handle duplication mechanics

  When player enters a hole:
  - If paired hole is empty -> duplicate player to paired hole
  - If paired hole has player -> both players disappear
  - Mark holes as filled when occupied
*/
void handle_duplication(void);

/*
  Set tile in both level map and screen display

  Helper function that combines set_tile() and my_cputcxy() to update
  both the game state and screen display in one call.

  @param x - X coordinate
  @param y - Y coordinate
  @param tile - Tile character to set
*/
void set_tile_and_draw(byte x, byte y, char tile);

#endif // DUPLICATOR_GAME_H

