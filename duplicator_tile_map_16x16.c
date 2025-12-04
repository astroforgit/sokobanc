/* duplicator_tile_map_16x16.c - Tile character mapping for 16x16 mode */

#include "duplicator_font_16x16.h"

typedef unsigned char byte;

// Map game tile characters to 16x16 screen codes
// This converts logical game tiles ('#', 'p', etc.) to screen memory codes
byte map_tile_to_16x16(byte tile) {
    switch (tile) {
        case '#':  return SC_WALL;
        case 'p':  return SC_PLAYER;
        case '*':  return SC_CRATE;
        case 'k':  return SC_KEY;
        case 'd':  return SC_DOOR;
        case 'e':  return SC_ENEMY;
        case '?':  return SC_HOLE_A;
        case '!':  return SC_HOLE_B;
        case 'b':  return SC_PLATE_A;  // Plate A graphics
        case 'c':  return SC_PLATE_A;  // Plate B uses same graphics as Plate A
        case 'g':  return SC_GATE_A;
        case 'h':  return SC_GATE_B;
        case '@':  return SC_EXIT_A;
        case ':':  return SC_EXIT_B;
        case ';':  return SC_EXIT_C;
        case '.':  return SC_FLOOR;
        case 'G':  return SC_GATE_A_OPEN;
        case 'H':  return SC_GATE_B_OPEN;
        case 'D':  return SC_DOOR_OPEN;
        case '[':  return SC_HOLE_A_FILL;
        case ']':  return SC_HOLE_B_FILL;
        // Wall lines
        case '$':  return SC_WALL_LINE_A;
        case '%':  return SC_WALL_LINE_B;
        case '&':  return SC_WALL_LINE_G;
        // Lines
        case '1':  return SC_LINE_A;
        case '2':  return SC_LINE_B;
        case '3':  return SC_LINE_C;
        case '4':  return SC_LINE_D;
        case '5':  return SC_LINE_E;
        case '6':  return SC_LINE_F;
        case '7':  return SC_LINE_G;
        case '8':  return SC_LINE_H;
        case ' ':  return 0;  // Empty space
        default:   return 0;  // Unknown - treat as empty
    }
}

