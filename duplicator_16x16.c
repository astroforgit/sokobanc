// Use RAM-based configuration instead of ROM cartridge for more space
#define CFGFILE atari-xex.cfg

// Link the 16x16 mode libraries
//#link "duplicator_conio_16x16.c"
//#link "duplicator_font_16x16.c"
//#link "duplicator_game.c"

/*
  Duplicator Game - 16x16 Big Tile Mode
  20x12 tile grid (each tile is 2x2 characters)

  This version displays each game tile as a 2x2 block of characters,
  making tiles 4x bigger and much easier to see!
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <peekpoke.h>
#include <conio.h>
#include <joystick.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Memory definitions
#define CHARSET_MEM ((byte*)0x7000)
#define DLIST_MEM   ((byte*)0x8000)
#define SCREEN_MEM  ((byte*)0x9000)
#define ROM_CHARSET_ADDRESS 0xE000

// Include 16x16 conio first
#include "duplicator_conio_16x16.h"

// Now include the game files
// NOTE: We will provide our own my_cputcxy() wrapper function later
#include "duplicator_font.h"
#include "duplicator_font_16x16.h"
#include "atari_conio.h"
#include "duplicator_game.h"

// Include the scaling function
extern void scale_8x8_to_16x16(const byte* src_8x8, byte* dest_16x16);

// Level data from duplicator.txt
// Note: 'z' = holeA and Player, 'y' = holeB and enemy, 'p' = Player
const char* level_1[] = {
    "########:########",
    "########.########",
    "########.########",
    "########.########",
    "########.########",
    "########.########",
    "#######...#...###",
    "#######.p.#.e.###",
    "#######...#...###",
    "############.####",
    "############.####"
};

const char* level_2[] = {
    "########:########",
    "########.########",
    "########.########",
    "########.########",
    "########.########",
    "########.########",
    "#######...#...###",
    "#######.p.#.e.###",
    "#######...#...###",
    "############.####",
    "############.####"
};

// Add more levels here...
// For now, using just 2 levels for testing

// Array of level pointers
const char** levels[] = {
    level_1,
    level_2
};

#define NUM_LEVELS 2

// Graphics setup function for 16x16 mode
void setup_duplicator_graphics(void) {
    word charset_addr = (word)CHARSET_MEM;
    word dlist_addr = (word)DLIST_MEM;
    word screen_addr = (word)SCREEN_MEM;
    byte i;
    byte temp_16x16[32];  // Temporary buffer for scaled graphics

    // Copy ROM font to RAM
    memcpy(CHARSET_MEM, (byte*)ROM_CHARSET_ADDRESS, 1024);

    // Set character set pointer (CHBAS at 756)
    POKE(756, (byte)(charset_addr >> 8));

    // Scale and install 16x16 graphics
    // Each tile uses 4 consecutive character codes (32 bytes total)

    // Wall (0x80-0x83) - from duplicator_graphics[8]
    scale_8x8_to_16x16(&duplicator_graphics[8], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_WALL_TL * 8), temp_16x16, 32);

    // Player (0x84-0x87) - from duplicator_graphics[0]
    scale_8x8_to_16x16(&duplicator_graphics[0], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_PLAYER_TL * 8), temp_16x16, 32);

    // Crate (0x88-0x8B) - from duplicator_graphics[16]
    scale_8x8_to_16x16(&duplicator_graphics[16], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_CRATE_TL * 8), temp_16x16, 32);

    // Key (0x8C-0x8F) - from duplicator_graphics[24]
    scale_8x8_to_16x16(&duplicator_graphics[24], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_KEY_TL * 8), temp_16x16, 32);

    // Door (0x90-0x93) - from duplicator_graphics[32]
    scale_8x8_to_16x16(&duplicator_graphics[32], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_DOOR_TL * 8), temp_16x16, 32);

    // Enemy (0x94-0x97) - from duplicator_graphics[40]
    scale_8x8_to_16x16(&duplicator_graphics[40], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_ENEMY_TL * 8), temp_16x16, 32);

    // Hole A (0x98-0x9B) - from duplicator_graphics[48]
    scale_8x8_to_16x16(&duplicator_graphics[48], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_HOLE_A_TL * 8), temp_16x16, 32);

    // Hole B (0x9C-0x9F) - from duplicator_graphics[56]
    scale_8x8_to_16x16(&duplicator_graphics[56], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_HOLE_B_TL * 8), temp_16x16, 32);

    // Plate A (0xA0-0xA3) - from duplicator_graphics[64]
    scale_8x8_to_16x16(&duplicator_graphics[64], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_PLATE_A_TL * 8), temp_16x16, 32);

    // Plate B (0xA4-0xA7) - from duplicator_graphics[72]
    scale_8x8_to_16x16(&duplicator_graphics[72], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_PLATE_B_TL * 8), temp_16x16, 32);

    // Gate A (0xA8-0xAB) - from duplicator_graphics[80]
    scale_8x8_to_16x16(&duplicator_graphics[80], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_GATE_A_TL * 8), temp_16x16, 32);

    // Gate B (0xAC-0xAF) - from duplicator_graphics[88]
    scale_8x8_to_16x16(&duplicator_graphics[88], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_GATE_B_TL * 8), temp_16x16, 32);

    // Exit A (0xB0-0xB3) - from duplicator_graphics[96]
    scale_8x8_to_16x16(&duplicator_graphics[96], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_EXIT_A_TL * 8), temp_16x16, 32);

    // Exit B (0xB4-0xB7) - from duplicator_graphics[104]
    scale_8x8_to_16x16(&duplicator_graphics[104], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_EXIT_B_TL * 8), temp_16x16, 32);

    // Exit C (0xB8-0xBB) - from duplicator_graphics[208]
    scale_8x8_to_16x16(&duplicator_graphics[208], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_EXIT_C_TL * 8), temp_16x16, 32);

    // Floor (0xBC-0xBF) - from duplicator_graphics[112]
    scale_8x8_to_16x16(&duplicator_graphics[112], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_FLOOR_TL * 8), temp_16x16, 32);

    // Gate A Open (0xC0-0xC3) - from duplicator_graphics[216]
    scale_8x8_to_16x16(&duplicator_graphics[216], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_GATE_A_OPEN_TL * 8), temp_16x16, 32);

    // Gate B Open (0xC4-0xC7) - from duplicator_graphics[224]
    scale_8x8_to_16x16(&duplicator_graphics[224], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_GATE_B_OPEN_TL * 8), temp_16x16, 32);

    // Door Open (0xC8-0xCB) - from duplicator_graphics[232]
    scale_8x8_to_16x16(&duplicator_graphics[232], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_DOOR_OPEN_TL * 8), temp_16x16, 32);

    // Hole A Filled (0xCC-0xCF) - from duplicator_graphics[240]
    scale_8x8_to_16x16(&duplicator_graphics[240], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_HOLE_A_FILL_TL * 8), temp_16x16, 32);

    // Hole B Filled (0xD0-0xD3) - from duplicator_graphics[248]
    scale_8x8_to_16x16(&duplicator_graphics[248], temp_16x16);
    memcpy(CHARSET_MEM + (TILE_HOLE_B_FILL_TL * 8), temp_16x16, 32);
    
    // Create custom display list
    // 3 blank lines at top
    DLIST_MEM[0] = 0x70;
    DLIST_MEM[1] = 0x70;
    DLIST_MEM[2] = 0x70;
    
    // First line with LMS (Load Memory Scan) - points to screen memory
    DLIST_MEM[3] = 0x42;
    DLIST_MEM[4] = (byte)screen_addr;
    DLIST_MEM[5] = (byte)(screen_addr >> 8);
    
    // 23 more lines of mode 2 (text mode, 40 columns)
    for (i = 0; i < 23; ++i) {
        DLIST_MEM[6 + i] = 0x02;
    }
    
    // Jump back to start of display list
    DLIST_MEM[29] = 0x41;
    DLIST_MEM[30] = (byte)dlist_addr;
    DLIST_MEM[31] = (byte)(dlist_addr >> 8);
    
    // Set colors
    POKE(709, 0);   // Background color (black)
    POKE(710, 14);  // Foreground color (light blue)
    
    // Set display list pointer (SDLSTL/SDLSTH at 560/561)
    POKEW(560, dlist_addr);
    
    // Turn off cursor
    POKE(752, 1);
}

// Map game tile characters to 16x16 tile codes
byte map_tile_to_16x16(char tile) {
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
        case ' ':  return 0;  // Empty space
        default:   return 0;  // Unknown - treat as empty
    }
}

// Wrapper for my_cputcxy that maps tiles to 16x16 codes
void my_cputcxy(byte x, byte y, char tile) {
    byte tile_code = map_tile_to_16x16(tile);
    my_cputcxy_16x16(x, y, tile_code);
}

// Main function
void main(void) {
    byte joy, last_joy = 0;

    GameState* state;
    byte current_level = 0;

    // Initialize joystick
    joy_install(joy_static_stddrv);

    // Setup graphics
    setup_duplicator_graphics();

    // Clear screen
    my_clrscr_16x16();

    // Load first level
    load_level(levels[current_level], 11);
    draw_level();

    // Main game loop
    while (1) {
        // Read joystick
        joy = joy_read(0);

        // Handle joystick input (with debouncing)
        if (joy && !last_joy) {
            if (JOY_UP(joy)) {
                try_move_player(0, -1);
            } else if (JOY_DOWN(joy)) {
                try_move_player(0, 1);
            } else if (JOY_LEFT(joy)) {
                try_move_player(-1, 0);
            } else if (JOY_RIGHT(joy)) {
                try_move_player(1, 0);
            }
        }
        last_joy = joy;

        // Also check keyboard
        if (kbhit()) {
            byte key = cgetc();
            if (key == CH_CURS_UP || key == 'w' || key == 'W') {
                try_move_player(0, -1);
            } else if (key == CH_CURS_DOWN || key == 's' || key == 'S') {
                try_move_player(0, 1);
            } else if (key == CH_CURS_LEFT || key == 'a' || key == 'A') {
                try_move_player(-1, 0);
            } else if (key == CH_CURS_RIGHT || key == 'd' || key == 'D') {
                try_move_player(1, 0);
            } else if (key == 'r' || key == 'R') {
                // Restart level
                load_level(levels[current_level], 11);
                draw_level();
            } else if (key == CH_ESC) {
                break;  // Exit game
            }
        }

        // Check win condition
        state = get_game_state();
        if (state->num_players > 0 && is_level_complete()) {
            // Level complete!
            current_level++;
            if (current_level >= NUM_LEVELS) {
                // Game complete!
                break;
            }
            // Load next level
            load_level(levels[current_level], 11);
            draw_level();
        }

        wait_vblank_16x16();
    }
}

