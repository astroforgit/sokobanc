/* duplicator_tiles_16x16.h - Complete tile definitions for 16x16 mode
 * 
 * This file contains:
 * 1. Game logic tile character definitions (TILE_*)
 * 2. Screen code definitions for graphics (WALL, PLAYER, etc.)
 * 3. Tile mapping function to convert game chars to screen codes
 */

#ifndef DUPLICATOR_TILES_16X16_H
#define DUPLICATOR_TILES_16X16_H

typedef unsigned char byte;

// ============================================================================
// TILE CORNER OFFSETS
// ============================================================================
// Add these to any base screen code to get the 4 corners of a 16x16 tile
#define TILE_TL  0  // Top-Left corner offset
#define TILE_TR  1  // Top-Right corner offset
#define TILE_BL  2  // Bottom-Left corner offset
#define TILE_BR  3  // Bottom-Right corner offset

// ============================================================================
// GAME LOGIC LAYER - Tile character definitions (TILE_*)
// ============================================================================
// These are the characters used in level data and game logic
#define TILE_EMPTY       ' '
#define TILE_WALL        '#'
#define TILE_PLAYER      'p'
#define TILE_CRATE       '*'
#define TILE_KEY         'k'
#define TILE_DOOR        'd'
#define TILE_DOOR_OPEN   'D'
#define TILE_ENEMY       'e'
#define TILE_HOLE_A      '?'
#define TILE_HOLE_B      '!'
#define TILE_PLATE_A     'b'
#define TILE_PLATE_B     'c'
#define TILE_GATE_A      'g'
#define TILE_GATE_B      'h'
#define TILE_GATE_A_OPEN 'G'
#define TILE_GATE_B_OPEN 'H'
#define TILE_EXIT_A      '@'
#define TILE_EXIT_B      ':'
#define TILE_EXIT_C      ';'
#define TILE_FLOOR       '.'
#define TILE_HOLE_A_FILL '['
#define TILE_HOLE_B_FILL ']'
#define TILE_WALL_LINE_A '$'
#define TILE_WALL_LINE_B '%'
#define TILE_WALL_LINE_G '&'
#define TILE_LINE_A      '1'
#define TILE_LINE_B      '2'
#define TILE_LINE_C      '3'
#define TILE_LINE_D      '4'
#define TILE_LINE_E      '5'
#define TILE_LINE_F      '6'
#define TILE_LINE_G      '7'
#define TILE_LINE_H      '8'

// ============================================================================
// GRAPHICS LAYER - Screen code definitions
// ============================================================================
// These are the codes written to screen memory (each tile uses 4 consecutive codes)
#define WALL           0x00  // Wall tile (screen codes 0x00-0x03)
#define PLAYER         0x04  // Player tile (screen codes 0x04-0x07)
#define CRATE          0x08  // Crate tile (screen codes 0x08-0x0B)
#define KEY            0x0C  // Key tile (screen codes 0x0C-0x0F)
#define DOOR           0x10  // Door tile (screen codes 0x10-0x13)
#define ENEMY          0x14  // Enemy tile (screen codes 0x14-0x17)
#define HOLE_A         0x18  // Hole A tile (screen codes 0x18-0x1B)
#define HOLE_B         0x1C  // Hole B tile (screen codes 0x1C-0x1F)
#define PLATE_A        0x20  // Plate A tile (screen codes 0x20-0x23)
#define PLATE_B        0x24  // Plate B tile (screen codes 0x24-0x27)
#define GATE_A         0x28  // Gate A tile (screen codes 0x28-0x2B)
#define GATE_B         0x2C  // Gate B tile (screen codes 0x2C-0x2F)
#define EXIT_A         0x30  // Exit A tile (screen codes 0x30-0x33)
#define EXIT_B         0x34  // Exit B tile (screen codes 0x34-0x37)
#define EXIT_C         0x38  // Exit C tile (screen codes 0x38-0x3B)
#define FLOOR          0x3C  // Floor tile (screen codes 0x3C-0x3F)
#define GATE_A_OPEN    0x40  // Gate A Open tile (screen codes 0x40-0x43)
#define GATE_B_OPEN    0x44  // Gate B Open tile (screen codes 0x44-0x47)
#define DOOR_OPEN      0x48  // Door Open tile (screen codes 0x48-0x4B)
#define HOLE_A_FILL    0x4C  // Hole A Filled tile (screen codes 0x4C-0x4F)
#define HOLE_B_FILL    0x50  // Hole B Filled tile (screen codes 0x50-0x53)
#define WALL_LINE_A    0x54  // Wall Line A tile (screen codes 0x54-0x57)
#define WALL_LINE_B    0x58  // Wall Line B tile (screen codes 0x58-0x5B)
#define WALL_LINE_G    0x5C  // Wall Line G tile (screen codes 0x5C-0x5F)
#define LINE_A         0x60  // Line A tile (screen codes 0x60-0x63)
#define LINE_B         0x64  // Line B tile (screen codes 0x64-0x67)
#define LINE_C         0x68  // Line C tile (screen codes 0x68-0x6B)
#define LINE_D         0x6C  // Line D tile (screen codes 0x6C-0x6F)
#define LINE_E         0x70  // Line E tile (screen codes 0x70-0x73)
#define LINE_F         0x74  // Line F tile (screen codes 0x74-0x77)
#define LINE_G         0x78  // Line G tile (screen codes 0x78-0x7B)
#define LINE_H         0x7C  // Line H tile (screen codes 0x7C-0x7F)

// ============================================================================
// TRANSLATION LAYER - Mapping function
// ============================================================================
// Converts game tile characters (TILE_*) to screen codes for rendering
// Function declaration (implementation in duplicator_tile_map_16x16.c)
byte map_tile_to_16x16(byte tile);

#endif // DUPLICATOR_TILES_16X16_H

