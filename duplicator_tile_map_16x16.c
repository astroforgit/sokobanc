/* duplicator_tile_map_16x16.c - Tile character mapping for 16x16 mode */

#include "duplicator_font_16x16.h"

typedef unsigned char byte;

// Map game tile characters to 16x16 tile codes
byte map_tile_to_16x16(byte tile) {
    switch (tile) {
        case '#':  return TILE_WALL_TL;
        case 'p':  return TILE_PLAYER_TL;
        case '*':  return TILE_CRATE_TL;
        case 'k':  return TILE_KEY_TL;
        case 'd':  return TILE_DOOR_TL;
        case 'e':  return TILE_ENEMY_TL;
        case '?':  return TILE_HOLE_A_TL;
        case '!':  return TILE_HOLE_B_TL;
        case 'b':  return TILE_PLATE_A_TL;
        case 'c':  return TILE_PLATE_B_TL;
        case 'g':  return TILE_GATE_A_TL;
        case 'h':  return TILE_GATE_B_TL;
        case '@':  return TILE_EXIT_A_TL;
        case ':':  return TILE_EXIT_B_TL;
        case ';':  return TILE_EXIT_C_TL;
        case '.':  return TILE_FLOOR_TL;
        case 'G':  return TILE_GATE_A_OPEN_TL;
        case 'H':  return TILE_GATE_B_OPEN_TL;
        case 'D':  return TILE_DOOR_OPEN_TL;
        case '[':  return TILE_HOLE_A_FILL_TL;
        case ']':  return TILE_HOLE_B_FILL_TL;
        // Wall lines
        case '$':  return TILE_WALL_LINE_A_TL;
        case '%':  return TILE_WALL_LINE_B_TL;
        case '&':  return TILE_WALL_LINE_G_TL;
        // Lines
        case '1':  return TILE_LINE_A_TL;
        case '2':  return TILE_LINE_B_TL;
        case '3':  return TILE_LINE_C_TL;
        case '4':  return TILE_LINE_D_TL;
        case '5':  return TILE_LINE_E_TL;
        case '6':  return TILE_LINE_F_TL;
        case '7':  return TILE_LINE_G_TL;
        case '8':  return TILE_LINE_H_TL;
        case ' ':  return 0;  // Empty space
        default:   return 0;  // Unknown - treat as empty
    }
}

