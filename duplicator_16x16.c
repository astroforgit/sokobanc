// Use RAM-based configuration instead of ROM cartridge for more space
#define CFGFILE atari-xex.cfg

// Link the 16x16 mode libraries
//#link "duplicator_conio_16x16.c"
//#link "duplicator_tile_map_16x16.c"
//#link "duplicator_game_16x16.c"

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

// Include 16x16 conio first (this provides my_cputcxy wrapper)
#include "duplicator_conio_16x16.h"

// Now include the game files
// NOTE: duplicator_conio_16x16.h provides my_cputcxy, so we DON'T include atari_conio.h
// The DUPLICATOR_16X16_MODE macro prevents duplicator_game.c from including atari_conio.h
#include "duplicator_graphics_16x16.h"  // Pre-scaled 16x16 graphics
#include "duplicator_font_16x16.h"      // Character code definitions

#include "duplicator_game.h"

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

    // Copy ROM font to RAM
    memcpy(CHARSET_MEM, (byte*)ROM_CHARSET_ADDRESS, 1024);

    // Set character set pointer (CHBAS at 756)
    POKE(756, (byte)(charset_addr >> 8));

    // Install pre-scaled 16x16 graphics
    // The duplicator_graphics_16x16 array contains all tiles already scaled
    // Each tile is 32 bytes (4 characters x 8 bytes)
    // Tiles are in order: Player, Wall, Crate, Key, Door, Enemy, etc.

    // Copy all graphics at once
    // Player starts at character 0x84, which is offset 0x84 * 8 = 0x420 in charset
    // We have 21 tiles * 32 bytes = 672 bytes total
    memcpy(CHARSET_MEM + (0x80 * 8), duplicator_graphics_16x16, sizeof(duplicator_graphics_16x16));
    
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

