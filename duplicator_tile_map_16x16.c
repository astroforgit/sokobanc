/* duplicator_tile_map_16x16.c - Tile character mapping for 16x16 mode */

#include "duplicator_font_16x16.h"

typedef unsigned char byte;

// Map game tile characters to 16x16 screen codes
// This converts logical game tiles ('#', 'p', etc.) to screen memory codes
byte map_tile_to_16x16(byte tile) {
    switch (tile) {
        case '#':  return WALL;
        case 'p':  return PLAYER;
        case '*':  return CRATE;
        case 'k':  return KEY;
        case 'd':  return DOOR;
        case 'e':  return ENEMY;
        case '?':  return HOLE_A;
        case '!':  return HOLE_B;
        case 'b':  return PLATE_A;  // Plate A graphics
        case 'c':  return PLATE_A;  // Plate B uses same graphics as Plate A
        case 'g':  return GATE_A;
        case 'h':  return GATE_B;
        case '@':  return EXIT_A;
        case ':':  return EXIT_B;
        case ';':  return EXIT_C;
        case '.':  return FLOOR;
        case 'G':  return GATE_A_OPEN;
        case 'H':  return GATE_B_OPEN;
        case 'D':  return DOOR_OPEN;
        case '[':  return HOLE_A_FILL;
        case ']':  return HOLE_B_FILL;
        // Wall lines
        case '$':  return WALL_LINE_A;
        case '%':  return WALL_LINE_B;
        case '&':  return WALL_LINE_G;
        // Lines
        case '1':  return LINE_A;
        case '2':  return LINE_B;
        case '3':  return LINE_C;
        case '4':  return LINE_D;
        case '5':  return LINE_E;
        case '6':  return LINE_F;
        case '7':  return LINE_G;
        case '8':  return LINE_H;
        case ' ':  return 0;  // Empty space
        default:   return 0;  // Unknown - treat as empty
    }
}

