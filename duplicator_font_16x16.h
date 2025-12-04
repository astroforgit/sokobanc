/*
  duplicator_font_16x16.h - 16x16 tile graphics for Duplicator game
  
  Each 16x16 tile uses 4 consecutive character codes (32 bytes total):
  - Top-Left (8 bytes)
  - Top-Right (8 bytes)
  - Bottom-Left (8 bytes)
  - Bottom-Right (8 bytes)
  
  Graphics are scaled from 8x8 originals by doubling each pixel.
*/

#ifndef DUPLICATOR_FONT_16X16_H
#define DUPLICATOR_FONT_16X16_H

// 16x16 Tile Screen Code Mappings
// Each tile uses 4 consecutive screen codes (TL, TR, BL, BR)
// These are SCREEN CODES written to screen memory, NOT game tile characters
// Game tile characters ('#', 'p', etc.) are defined in duplicator_game.h
// Using internal screen codes (not ATASCII)

// Tile corner offsets - add these to the base screen code
#define TILE_TL  0  // Top-Left corner offset
#define TILE_TR  1  // Top-Right corner offset
#define TILE_BL  2  // Bottom-Left corner offset
#define TILE_BR  3  // Bottom-Right corner offset

// Base screen codes for 16x16 tiles (each tile occupies 4 consecutive codes)
// These are what gets written to screen memory after mapping from game characters
#define SC_WALL           0x00  // Wall tile (screen codes 0x00-0x03)
#define SC_PLAYER         0x04  // Player tile (screen codes 0x04-0x07)
#define SC_CRATE          0x08  // Crate tile (screen codes 0x08-0x0B)
#define SC_KEY            0x0C  // Key tile (screen codes 0x0C-0x0F)
#define SC_DOOR           0x10  // Door tile (screen codes 0x10-0x13)
#define SC_ENEMY          0x14  // Enemy tile (screen codes 0x14-0x17)
#define SC_HOLE_A         0x18  // Hole A tile (screen codes 0x18-0x1B)
#define SC_HOLE_B         0x1C  // Hole B tile (screen codes 0x1C-0x1F)
#define SC_PLATE_A        0x20  // Plate A tile (screen codes 0x20-0x23)
#define SC_PLATE_B        0x24  // Plate B tile (screen codes 0x24-0x27)
#define SC_GATE_A         0x28  // Gate A tile (screen codes 0x28-0x2B)
#define SC_GATE_B         0x2C  // Gate B tile (screen codes 0x2C-0x2F)
#define SC_EXIT_A         0x30  // Exit A tile (screen codes 0x30-0x33)
#define SC_EXIT_B         0x34  // Exit B tile (screen codes 0x34-0x37)
#define SC_EXIT_C         0x38  // Exit C tile (screen codes 0x38-0x3B)
#define SC_FLOOR          0x3C  // Floor tile (screen codes 0x3C-0x3F)
#define SC_GATE_A_OPEN    0x40  // Gate A Open tile (screen codes 0x40-0x43)
#define SC_GATE_B_OPEN    0x44  // Gate B Open tile (screen codes 0x44-0x47)
#define SC_DOOR_OPEN      0x48  // Door Open tile (screen codes 0x48-0x4B)
#define SC_HOLE_A_FILL    0x4C  // Hole A Filled tile (screen codes 0x4C-0x4F)
#define SC_HOLE_B_FILL    0x50  // Hole B Filled tile (screen codes 0x50-0x53)
#define SC_WALL_LINE_A    0x54  // Wall Line A tile (screen codes 0x54-0x57)
#define SC_WALL_LINE_B    0x58  // Wall Line B tile (screen codes 0x58-0x5B)
#define SC_WALL_LINE_G    0x5C  // Wall Line G tile (screen codes 0x5C-0x5F)
#define SC_LINE_A         0x60  // Line A tile (screen codes 0x60-0x63)
#define SC_LINE_B         0x64  // Line B tile (screen codes 0x64-0x67)
#define SC_LINE_C         0x68  // Line C tile (screen codes 0x68-0x6B)
#define SC_LINE_D         0x6C  // Line D tile (screen codes 0x6C-0x6F)
#define SC_LINE_E         0x70  // Line E tile (screen codes 0x70-0x73)
#define SC_LINE_F         0x74  // Line F tile (screen codes 0x74-0x77)
#define SC_LINE_G         0x78  // Line G tile (screen codes 0x78-0x7B)
#define SC_LINE_H         0x7C  // Line H tile (screen codes 0x7C-0x7F)



#endif // DUPLICATOR_FONT_16X16_H

