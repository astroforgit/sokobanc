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
// Each tile uses 4 consecutive characters
// Using internal screen codes (not ATASCII)
// Screen codes 0x00-0x3F map to character set 0x20-0x5F
// We'll use screen codes starting from 0x00

// Wall tile (screen codes 0x00-0x03, charset 0x20-0x23)
#define TILE_WALL_TL      0x00
#define TILE_WALL_TR      0x01
#define TILE_WALL_BL      0x02
#define TILE_WALL_BR      0x03

// Player tile (screen codes 0x04-0x07, charset 0x24-0x27)
#define TILE_PLAYER_TL    0x04
#define TILE_PLAYER_TR    0x05
#define TILE_PLAYER_BL    0x06
#define TILE_PLAYER_BR    0x07

// Crate tile (screen codes 0x08-0x0B, charset 0x28-0x2B)
#define TILE_CRATE_TL     0x08
#define TILE_CRATE_TR     0x09
#define TILE_CRATE_BL     0x0A
#define TILE_CRATE_BR     0x0B

// Key tile (screen codes 0x0C-0x0F, charset 0x2C-0x2F)
#define TILE_KEY_TL       0x0C
#define TILE_KEY_TR       0x0D
#define TILE_KEY_BL       0x0E
#define TILE_KEY_BR       0x0F

// Door tile (screen codes 0x10-0x13, charset 0x30-0x33)
#define TILE_DOOR_TL      0x10
#define TILE_DOOR_TR      0x11
#define TILE_DOOR_BL      0x12
#define TILE_DOOR_BR      0x13

// Enemy tile (screen codes 0x14-0x17, charset 0x34-0x37)
#define TILE_ENEMY_TL     0x14
#define TILE_ENEMY_TR     0x15
#define TILE_ENEMY_BL     0x16
#define TILE_ENEMY_BR     0x17

// Hole A tile (screen codes 0x18-0x1B, charset 0x38-0x3B)
#define TILE_HOLE_A_TL    0x18
#define TILE_HOLE_A_TR    0x19
#define TILE_HOLE_A_BL    0x1A
#define TILE_HOLE_A_BR    0x1B

// Hole B tile (screen codes 0x1C-0x1F, charset 0x3C-0x3F)
#define TILE_HOLE_B_TL    0x1C
#define TILE_HOLE_B_TR    0x1D
#define TILE_HOLE_B_BL    0x1E
#define TILE_HOLE_B_BR    0x1F

// Plate A tile (screen codes 0x20-0x23, charset 0x40-0x43)
#define TILE_PLATE_A_TL   0x20
#define TILE_PLATE_A_TR   0x21
#define TILE_PLATE_A_BL   0x22
#define TILE_PLATE_A_BR   0x23

// Plate B tile (screen codes 0x24-0x27, charset 0x44-0x47)
#define TILE_PLATE_B_TL   0x24
#define TILE_PLATE_B_TR   0x25
#define TILE_PLATE_B_BL   0x26
#define TILE_PLATE_B_BR   0x27

// Gate A tile (screen codes 0x28-0x2B, charset 0x48-0x4B)
#define TILE_GATE_A_TL    0x28
#define TILE_GATE_A_TR    0x29
#define TILE_GATE_A_BL    0x2A
#define TILE_GATE_A_BR    0x2B

// Gate B tile (screen codes 0x2C-0x2F, charset 0x4C-0x4F)
#define TILE_GATE_B_TL    0x2C
#define TILE_GATE_B_TR    0x2D
#define TILE_GATE_B_BL    0x2E
#define TILE_GATE_B_BR    0x2F

// Exit A tile (screen codes 0x30-0x33, charset 0x50-0x53)
#define TILE_EXIT_A_TL    0x30
#define TILE_EXIT_A_TR    0x31
#define TILE_EXIT_A_BL    0x32
#define TILE_EXIT_A_BR    0x33

// Exit B tile (screen codes 0x34-0x37, charset 0x54-0x57)
#define TILE_EXIT_B_TL    0x34
#define TILE_EXIT_B_TR    0x35
#define TILE_EXIT_B_BL    0x36
#define TILE_EXIT_B_BR    0x37

// Exit C tile (screen codes 0x38-0x3B, charset 0x58-0x5B)
#define TILE_EXIT_C_TL    0x38
#define TILE_EXIT_C_TR    0x39
#define TILE_EXIT_C_BL    0x3A
#define TILE_EXIT_C_BR    0x3B

// Floor tile (screen codes 0x3C-0x3F, charset 0x5C-0x5F)
#define TILE_FLOOR_TL     0x3C
#define TILE_FLOOR_TR     0x3D
#define TILE_FLOOR_BL     0x3E
#define TILE_FLOOR_BR     0x3F

// Gate A Open tile (screen codes 0x40-0x43, charset 0x00-0x03 inverse)
#define TILE_GATE_A_OPEN_TL  0x40
#define TILE_GATE_A_OPEN_TR  0x41
#define TILE_GATE_A_OPEN_BL  0x42
#define TILE_GATE_A_OPEN_BR  0x43

// Gate B Open tile (screen codes 0x44-0x47, charset 0x04-0x07 inverse)
#define TILE_GATE_B_OPEN_TL  0x44
#define TILE_GATE_B_OPEN_TR  0x45
#define TILE_GATE_B_OPEN_BL  0x46
#define TILE_GATE_B_OPEN_BR  0x47

// Door Open tile (screen codes 0x48-0x4B, charset 0x08-0x0B inverse)
#define TILE_DOOR_OPEN_TL    0x48
#define TILE_DOOR_OPEN_TR    0x49
#define TILE_DOOR_OPEN_BL    0x4A
#define TILE_DOOR_OPEN_BR    0x4B

// Hole A Filled tile (screen codes 0x4C-0x4F, charset 0x0C-0x0F inverse)
#define TILE_HOLE_A_FILL_TL  0x4C
#define TILE_HOLE_A_FILL_TR  0x4D
#define TILE_HOLE_A_FILL_BL  0x4E
#define TILE_HOLE_A_FILL_BR  0x4F

// Hole B Filled tile (screen codes 0x50-0x53, charset 0x10-0x13 inverse)
#define TILE_HOLE_B_FILL_TL  0x50
#define TILE_HOLE_B_FILL_TR  0x51
#define TILE_HOLE_B_FILL_BL  0x52
#define TILE_HOLE_B_FILL_BR  0x53

// Wall Line A tile (screen codes 0x54-0x57)
#define TILE_WALL_LINE_A_TL  0x54
#define TILE_WALL_LINE_A_TR  0x55
#define TILE_WALL_LINE_A_BL  0x56
#define TILE_WALL_LINE_A_BR  0x57

// Wall Line B tile (screen codes 0x58-0x5B)
#define TILE_WALL_LINE_B_TL  0x58
#define TILE_WALL_LINE_B_TR  0x59
#define TILE_WALL_LINE_B_BL  0x5A
#define TILE_WALL_LINE_B_BR  0x5B

// Wall Line G tile (screen codes 0x5C-0x5F)
#define TILE_WALL_LINE_G_TL  0x5C
#define TILE_WALL_LINE_G_TR  0x5D
#define TILE_WALL_LINE_G_BL  0x5E
#define TILE_WALL_LINE_G_BR  0x5F

// Line A tile (screen codes 0x60-0x63)
#define TILE_LINE_A_TL       0x60
#define TILE_LINE_A_TR       0x61
#define TILE_LINE_A_BL       0x62
#define TILE_LINE_A_BR       0x63

// Line B tile (screen codes 0x64-0x67)
#define TILE_LINE_B_TL       0x64
#define TILE_LINE_B_TR       0x65
#define TILE_LINE_B_BL       0x66
#define TILE_LINE_B_BR       0x67

// Line C tile (screen codes 0x68-0x6B)
#define TILE_LINE_C_TL       0x68
#define TILE_LINE_C_TR       0x69
#define TILE_LINE_C_BL       0x6A
#define TILE_LINE_C_BR       0x6B

// Line D tile (screen codes 0x6C-0x6F)
#define TILE_LINE_D_TL       0x6C
#define TILE_LINE_D_TR       0x6D
#define TILE_LINE_D_BL       0x6E
#define TILE_LINE_D_BR       0x6F

// Line E tile (screen codes 0x70-0x73)
#define TILE_LINE_E_TL       0x70
#define TILE_LINE_E_TR       0x71
#define TILE_LINE_E_BL       0x72
#define TILE_LINE_E_BR       0x73

// Line F tile (screen codes 0x74-0x77)
#define TILE_LINE_F_TL       0x74
#define TILE_LINE_F_TR       0x75
#define TILE_LINE_F_BL       0x76
#define TILE_LINE_F_BR       0x77

// Line G tile (screen codes 0x78-0x7B)
#define TILE_LINE_G_TL       0x78
#define TILE_LINE_G_TR       0x79
#define TILE_LINE_G_BL       0x7A
#define TILE_LINE_G_BR       0x7B

// Line H tile (screen codes 0x7C-0x7F)
#define TILE_LINE_H_TL       0x7C
#define TILE_LINE_H_TR       0x7D
#define TILE_LINE_H_BL       0x7E
#define TILE_LINE_H_BR       0x7F

#endif // DUPLICATOR_FONT_16X16_H

