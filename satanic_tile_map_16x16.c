/* satanic_tile_map_16x16.c - Tile character mapping for 16x16 mode */

#include "satanic_tiles_16x16.h"

// Map game tile characters to 16x16 screen codes
// This converts logical game tiles (TILE_*) to screen memory codes
byte map_tile_to_16x16(byte tile) {
    switch (tile) {
        case TILE_WALL: return WALL;
        case TILE_PLAYER: return PLAYER;
        case TILE_EMPTY: return 0;  // Empty space
        default: return 0;   // Unknown - treat as empty
    }
}

