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
// Each tile uses 4 consecutive characters starting from 0x80 (128)
// This avoids collision with ASCII characters

// Wall tile (0x80-0x83)
#define TILE_WALL_TL      0x80
#define TILE_WALL_TR      0x81
#define TILE_WALL_BL      0x82
#define TILE_WALL_BR      0x83

// Player tile (0x84-0x87)
#define TILE_PLAYER_TL    0x84
#define TILE_PLAYER_TR    0x85
#define TILE_PLAYER_BL    0x86
#define TILE_PLAYER_BR    0x87

// Crate tile (0x88-0x8B)
#define TILE_CRATE_TL     0x88
#define TILE_CRATE_TR     0x89
#define TILE_CRATE_BL     0x8A
#define TILE_CRATE_BR     0x8B

// Key tile (0x8C-0x8F)
#define TILE_KEY_TL       0x8C
#define TILE_KEY_TR       0x8D
#define TILE_KEY_BL       0x8E
#define TILE_KEY_BR       0x8F

// Door tile (0x90-0x93)
#define TILE_DOOR_TL      0x90
#define TILE_DOOR_TR      0x91
#define TILE_DOOR_BL      0x92
#define TILE_DOOR_BR      0x93

// Enemy tile (0x94-0x97)
#define TILE_ENEMY_TL     0x94
#define TILE_ENEMY_TR     0x95
#define TILE_ENEMY_BL     0x96
#define TILE_ENEMY_BR     0x97

// Hole A tile (0x98-0x9B)
#define TILE_HOLE_A_TL    0x98
#define TILE_HOLE_A_TR    0x99
#define TILE_HOLE_A_BL    0x9A
#define TILE_HOLE_A_BR    0x9B

// Hole B tile (0x9C-0x9F)
#define TILE_HOLE_B_TL    0x9C
#define TILE_HOLE_B_TR    0x9D
#define TILE_HOLE_B_BL    0x9E
#define TILE_HOLE_B_BR    0x9F

// Plate A tile (0xA0-0xA3)
#define TILE_PLATE_A_TL   0xA0
#define TILE_PLATE_A_TR   0xA1
#define TILE_PLATE_A_BL   0xA2
#define TILE_PLATE_A_BR   0xA3

// Plate B tile (0xA4-0xA7)
#define TILE_PLATE_B_TL   0xA4
#define TILE_PLATE_B_TR   0xA5
#define TILE_PLATE_B_BL   0xA6
#define TILE_PLATE_B_BR   0xA7

// Gate A tile (0xA8-0xAB)
#define TILE_GATE_A_TL    0xA8
#define TILE_GATE_A_TR    0xA9
#define TILE_GATE_A_BL    0xAA
#define TILE_GATE_A_BR    0xAB

// Gate B tile (0xAC-0xAF)
#define TILE_GATE_B_TL    0xAC
#define TILE_GATE_B_TR    0xAD
#define TILE_GATE_B_BL    0xAE
#define TILE_GATE_B_BR    0xAF

// Exit A tile (0xB0-0xB3)
#define TILE_EXIT_A_TL    0xB0
#define TILE_EXIT_A_TR    0xB1
#define TILE_EXIT_A_BL    0xB2
#define TILE_EXIT_A_BR    0xB3

// Exit B tile (0xB4-0xB7)
#define TILE_EXIT_B_TL    0xB4
#define TILE_EXIT_B_TR    0xB5
#define TILE_EXIT_B_BL    0xB6
#define TILE_EXIT_B_BR    0xB7

// Exit C tile (0xB8-0xBB)
#define TILE_EXIT_C_TL    0xB8
#define TILE_EXIT_C_TR    0xB9
#define TILE_EXIT_C_BL    0xBA
#define TILE_EXIT_C_BR    0xBB

// Floor tile (0xBC-0xBF)
#define TILE_FLOOR_TL     0xBC
#define TILE_FLOOR_TR     0xBD
#define TILE_FLOOR_BL     0xBE
#define TILE_FLOOR_BR     0xBF

// Gate A Open tile (0xC0-0xC3)
#define TILE_GATE_A_OPEN_TL  0xC0
#define TILE_GATE_A_OPEN_TR  0xC1
#define TILE_GATE_A_OPEN_BL  0xC2
#define TILE_GATE_A_OPEN_BR  0xC3

// Gate B Open tile (0xC4-0xC7)
#define TILE_GATE_B_OPEN_TL  0xC4
#define TILE_GATE_B_OPEN_TR  0xC5
#define TILE_GATE_B_OPEN_BL  0xC6
#define TILE_GATE_B_OPEN_BR  0xC7

// Door Open tile (0xC8-0xCB)
#define TILE_DOOR_OPEN_TL    0xC8
#define TILE_DOOR_OPEN_TR    0xC9
#define TILE_DOOR_OPEN_BL    0xCA
#define TILE_DOOR_OPEN_BR    0xCB

// Hole A Filled tile (0xCC-0xCF)
#define TILE_HOLE_A_FILL_TL  0xCC
#define TILE_HOLE_A_FILL_TR  0xCD
#define TILE_HOLE_A_FILL_BL  0xCE
#define TILE_HOLE_A_FILL_BR  0xCF

// Hole B Filled tile (0xD0-0xD3)
#define TILE_HOLE_B_FILL_TL  0xD0
#define TILE_HOLE_B_FILL_TR  0xD1
#define TILE_HOLE_B_FILL_BL  0xD2
#define TILE_HOLE_B_FILL_BR  0xD3

#endif // DUPLICATOR_FONT_16X16_H

