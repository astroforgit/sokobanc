/* duplicator_tile_map_16x16.c - Tile character mapping for 16x16 mode */

#include "duplicator_tiles_16x16.h"

// Map game tile characters to 16x16 screen codes
// This converts logical game tiles (TILE_WALL, TILE_PLAYER, etc.) to screen memory codes (WALL, PLAYER, etc.)
byte map_tile_to_16x16(byte tile) {
    switch (tile) {
        case TILE_WALL:        return WALL;
        case TILE_PLAYER:      return PLAYER;
        case TILE_CRATE:       return CRATE;
        case TILE_KEY:         return KEY;
        case TILE_DOOR:        return DOOR;
        case TILE_ENEMY:       return ENEMY;
        case TILE_HOLE_A:      return HOLE_A;
        case TILE_HOLE_B:      return HOLE_B;
        case TILE_PLATE_A:     return PLATE_A;
        case TILE_PLATE_B:     return PLATE_A;  // Plate B uses same graphics as Plate A
        case TILE_GATE_A:      return GATE_A;
        case TILE_GATE_B:      return GATE_B;
        case TILE_EXIT_A:      return EXIT_A;
        case TILE_EXIT_B:      return EXIT_B;
        case TILE_EXIT_C:      return EXIT_C;
        case TILE_FLOOR:       return FLOOR;
        case TILE_GATE_A_OPEN: return GATE_A_OPEN;
        case TILE_GATE_B_OPEN: return GATE_B_OPEN;
        case TILE_DOOR_OPEN:   return DOOR_OPEN;
        case TILE_HOLE_A_FILL: return HOLE_A_FILL;
        case TILE_HOLE_B_FILL: return HOLE_B_FILL;
        case TILE_WALL_LINE_A: return WALL_LINE_A;
        case TILE_WALL_LINE_B: return WALL_LINE_B;
        case TILE_WALL_LINE_G: return WALL_LINE_G;
        case TILE_LINE_A:      return LINE_A;
        case TILE_LINE_B:      return LINE_B;
        case TILE_LINE_C:      return LINE_C;
        case TILE_LINE_D:      return LINE_D;
        case TILE_LINE_E:      return LINE_E;
        case TILE_LINE_F:      return LINE_F;
        case TILE_LINE_G:      return LINE_G;
        case TILE_LINE_H:      return LINE_H;
        case TILE_EMPTY:       return 0;  // Empty space
        default:               return 0;  // Unknown - treat as empty
    }
}

