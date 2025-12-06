/* satanic_tiles_16x16.h - Complete tile definitions for 16x16 mode
 *
 * This file contains:
 * 1. Game logic tile character definitions (TILE_*)
 * 2. Screen code definitions for graphics (WALL, PLAYER, etc.)
 * 3. Tile mapping function to convert game chars to screen codes
 */

#ifndef SATANIC_TILES_16X16_H
#define SATANIC_TILES_16X16_H

typedef unsigned char byte;

// Tile corner offsets
#define TILE_TL  0  // Top-Left corner offset
#define TILE_TR  1  // Top-Right corner offset
#define TILE_BL  2  // Bottom-Left corner offset
#define TILE_BR  3  // Bottom-Right corner offset

// GAME LOGIC LAYER - Tile character definitions (TILE_*)
#define TILE_WALL '#'
#define TILE_PLAYER 'p'
#define TILE_FLOOR '.'
#define TILE_EMPTY ' '

// GRAPHICS LAYER - Screen code definitions
#define WALL 0x00  // WALL tile (screen codes 0x00-0x03)
#define PLAYER 0x04  // PLAYER tile (screen codes 0x04-0x07)
#define FLOOR 0x08  // FLOOR tile (screen codes 0x08-0x0B)

// TRANSLATION LAYER - Mapping function
// Converts game tile characters (TILE_*) to screen codes for rendering
// Function declaration (implementation in satanic_tile_map_16x16.c)
byte map_tile_to_16x16(byte tile);

#endif

