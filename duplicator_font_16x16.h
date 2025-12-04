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

// 16x16 Tile Character Mappings
// Each tile uses 4 consecutive characters (TL, TR, BL, BR)
// Using internal screen codes (not ATASCII)
// Screen codes 0x00-0x3F map to character set 0x20-0x5F
// We'll use screen codes starting from 0x00

// Tile corner offsets - add these to the base tile code
#define TILE_TL  0  // Top-Left
#define TILE_TR  1  // Top-Right
#define TILE_BL  2  // Bottom-Left
#define TILE_BR  3  // Bottom-Right

// Base tile codes (each tile occupies 4 consecutive character codes)
#define TILE_WALL           0x00  // Wall tile (screen codes 0x00-0x03)
#define TILE_PLAYER         0x04  // Player tile (screen codes 0x04-0x07)
#define TILE_CRATE          0x08  // Crate tile (screen codes 0x08-0x0B)
#define TILE_KEY            0x0C  // Key tile (screen codes 0x0C-0x0F)
#define TILE_DOOR           0x10  // Door tile (screen codes 0x10-0x13)
#define TILE_ENEMY          0x14  // Enemy tile (screen codes 0x14-0x17)
#define TILE_HOLE_A         0x18  // Hole A tile (screen codes 0x18-0x1B)
#define TILE_HOLE_B         0x1C  // Hole B tile (screen codes 0x1C-0x1F)
#define TILE_PLATE_A        0x20  // Plate A tile (screen codes 0x20-0x23)
#define TILE_PLATE_B        0x24  // Plate B tile (screen codes 0x24-0x27)
#define TILE_GATE_A         0x28  // Gate A tile (screen codes 0x28-0x2B)
#define TILE_GATE_B         0x2C  // Gate B tile (screen codes 0x2C-0x2F)
#define TILE_EXIT_A         0x30  // Exit A tile (screen codes 0x30-0x33)
#define TILE_EXIT_B         0x34  // Exit B tile (screen codes 0x34-0x37)
#define TILE_EXIT_C         0x38  // Exit C tile (screen codes 0x38-0x3B)
#define TILE_FLOOR          0x3C  // Floor tile (screen codes 0x3C-0x3F)
#define TILE_GATE_A_OPEN    0x40  // Gate A Open tile (screen codes 0x40-0x43)
#define TILE_GATE_B_OPEN    0x44  // Gate B Open tile (screen codes 0x44-0x47)
#define TILE_DOOR_OPEN      0x48  // Door Open tile (screen codes 0x48-0x4B)
#define TILE_HOLE_A_FILL    0x4C  // Hole A Filled tile (screen codes 0x4C-0x4F)
#define TILE_HOLE_B_FILL    0x50  // Hole B Filled tile (screen codes 0x50-0x53)
#define TILE_WALL_LINE_A    0x54  // Wall Line A tile (screen codes 0x54-0x57)
#define TILE_WALL_LINE_B    0x58  // Wall Line B tile (screen codes 0x58-0x5B)
#define TILE_WALL_LINE_G    0x5C  // Wall Line G tile (screen codes 0x5C-0x5F)
#define TILE_LINE_A         0x60  // Line A tile (screen codes 0x60-0x63)
#define TILE_LINE_B         0x64  // Line B tile (screen codes 0x64-0x67)
#define TILE_LINE_C         0x68  // Line C tile (screen codes 0x68-0x6B)
#define TILE_LINE_D         0x6C  // Line D tile (screen codes 0x6C-0x6F)
#define TILE_LINE_E         0x70  // Line E tile (screen codes 0x70-0x73)
#define TILE_LINE_F         0x74  // Line F tile (screen codes 0x74-0x77)
#define TILE_LINE_G         0x78  // Line G tile (screen codes 0x78-0x7B)
#define TILE_LINE_H         0x7C  // Line H tile (screen codes 0x7C-0x7F)

// Legacy compatibility - old _TL definitions point to base tile codes
#define TILE_WALL_TL           TILE_WALL
#define TILE_PLAYER_TL         TILE_PLAYER
#define TILE_CRATE_TL          TILE_CRATE
#define TILE_KEY_TL            TILE_KEY
#define TILE_DOOR_TL           TILE_DOOR
#define TILE_ENEMY_TL          TILE_ENEMY
#define TILE_HOLE_A_TL         TILE_HOLE_A
#define TILE_HOLE_B_TL         TILE_HOLE_B
#define TILE_PLATE_A_TL        TILE_PLATE_A
#define TILE_PLATE_B_TL        TILE_PLATE_B
#define TILE_GATE_A_TL         TILE_GATE_A
#define TILE_GATE_B_TL         TILE_GATE_B
#define TILE_EXIT_A_TL         TILE_EXIT_A
#define TILE_EXIT_B_TL         TILE_EXIT_B
#define TILE_EXIT_C_TL         TILE_EXIT_C
#define TILE_FLOOR_TL          TILE_FLOOR
#define TILE_GATE_A_OPEN_TL    TILE_GATE_A_OPEN
#define TILE_GATE_B_OPEN_TL    TILE_GATE_B_OPEN
#define TILE_DOOR_OPEN_TL      TILE_DOOR_OPEN
#define TILE_HOLE_A_FILL_TL    TILE_HOLE_A_FILL
#define TILE_HOLE_B_FILL_TL    TILE_HOLE_B_FILL
#define TILE_WALL_LINE_A_TL    TILE_WALL_LINE_A
#define TILE_WALL_LINE_B_TL    TILE_WALL_LINE_B
#define TILE_WALL_LINE_G_TL    TILE_WALL_LINE_G
#define TILE_LINE_A_TL         TILE_LINE_A
#define TILE_LINE_B_TL         TILE_LINE_B
#define TILE_LINE_C_TL         TILE_LINE_C
#define TILE_LINE_D_TL         TILE_LINE_D
#define TILE_LINE_E_TL         TILE_LINE_E
#define TILE_LINE_F_TL         TILE_LINE_F
#define TILE_LINE_G_TL         TILE_LINE_G
#define TILE_LINE_H_TL         TILE_LINE_H

#endif // DUPLICATOR_FONT_16X16_H

