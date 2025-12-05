/* satanic_game.h - Game state and logic
 *
 * This file defines the game state structure and provides
 * an interface for accessing and managing game state.
 */

#ifndef SATANIC_GAME_H
#define SATANIC_GAME_H

#include <stdlib.h>
#include <string.h>
#include "satanic_tiles_16x16.h"

// Data types
typedef unsigned char byte;
typedef unsigned short word;

// Game constants
#define MAX_LEVEL_WIDTH 20
#define MAX_LEVEL_HEIGHT 12

// Game state structure
typedef struct {
    byte player_x;
    byte player_y;
    byte current_level;
    byte level_width;
    byte level_height;
    char level_map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
} GameState;

/*
 * Get the current game state
 *
 * @return Pointer to the game state structure
 */
GameState* get_game_state(void);

/*
 * Initialize game state
 */
void init_game_state(void);

/*
 * Load a level into game state
 *
 * @param level_num - Level number to load
 */
void load_level(byte level_num);

/*
 * Try to move the player
 *
 * @param dx - Horizontal movement (-1, 0, or 1)
 * @param dy - Vertical movement (-1, 0, or 1)
 */
void try_move(signed char dx, signed char dy);

#endif // SATANIC_GAME_H

/* satanic_game.h - Game state and logic
 *
 * This file defines the game state structure and provides
 * an interface for accessing and managing game state.
 */

#ifndef SATANIC_GAME_H
#define SATANIC_GAME_H

#include <stdlib.h>
#include <string.h>
#include "satanic_tiles_16x16.h"

// Data types
typedef unsigned char byte;
typedef unsigned short word;

// Game constants
#define MAX_LEVEL_WIDTH 20
#define MAX_LEVEL_HEIGHT 12

// Game state structure
typedef struct {
    byte player_x;
    byte player_y;
    byte current_level;
    byte level_width;
    byte level_height;
    char level_map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
} GameState;

/*
 * Get the current game state
 *
 * @return Pointer to the game state structure
 */
GameState* get_game_state(void);

/*
 * Initialize game state
 */
void init_game_state(void);

/*
 * Load a level into game state
 *
 * @param level_num - Level number to load
 */
void load_level(byte level_num);

/*
 * Try to move the player
 *
 * @param dx - Horizontal movement (-1, 0, or 1)
 * @param dy - Vertical movement (-1, 0, or 1)
 */
void try_move(signed char dx, signed char dy);

#endif // SATANIC_GAME_H

