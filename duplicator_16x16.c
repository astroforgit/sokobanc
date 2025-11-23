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
    "#################",
    "#################",
    "###...#...#...###",
    "###.!2%2?...b.###",
    "###...#...#.1.###",
    "####.#######$####",
    "####.##...##$####",
    "####........g...@",
    "#######...#######",
    "########.########",
    "########p########"
};

const char* level_3[] = {
    "#################",
    "#################",
    "###...#...#...###",
    "###.!2%2?...b.###",
    "###...#...#.1.###",
    "####.#######$####",
    "###.......##$####",
    "p.......c2h.g...@",
    "###.......#######",
    "#################",
    "#################"
};

const char* level_4[] = {
    "############.####",
    "############.####",
    "###...#...#...###",
    "###.!2%2?.#......",
    "###...#...#...###",
    "####.###.########",
    "###...#...d.d.###",
    "p.....#.k.d.d...@",
    "###...#...d.d.###",
    "#################",
    "#################"
};

const char* level_5[] = {
    "########@########",
    "########.########",
    "##6222cg2%%%223##",
    "##1....g.###..1##",
    "##1..#######..1##",
    "##1?2%%%%%%%2!1##",
    "##1..#######..1##",
    "p.1..#######..1##",
    "##1....h......1##",
    "##52222hb222224##",
    "#################"
};

const char* level_6[] = {
    "########@########",
    "########g########",
    "###622%24.#...###",
    "###1..#.......###",
    "###1..#...#...###",
    "###$!%%%2%%%?####",
    "###1..#...#...###",
    "###5b.#...#.*.###",
    "###...#...#...###",
    "########.########",
    "########p########"
};

const char* level_7[] = {
    "###@#############",
    "###.#############",
    "###.d...d..*!k.##",
    "###.d...d6224..##",
    "#########$####.##",
    "###......?.*.#.##",
    "###............##",
    "###............##",
    "###............##",
    "########.########",
    "########p########"
};

const char* level_8[] = {
    "#################",
    "##...##...##...##",
    "##.!....k......##",
    "##.1.##...##...##",
    "###$####.####.###",
    "###$####.####.###",
    "##.1.##...##...##",
    "##.?.##.e......d@",
    "##...##...##...##",
    "###.#############",
    "###p#############"
};

const char* level_9[] = {
    "#################",
    "##.....###.....##",
    "##......*...e..##",
    "##.....###.....##",
    "##......*...e..##",
    "##.....###.....##",
    "##......*...e..##",
    "p......###......@",
    "#################",
    "#################",
    "#################"
};

const char* level_10[] = {
    "#################",
    "#######...h2223.@",
    "#######.!.####$##",
    "#######.1.####$##",
    "########$#####$##",
    "#####...1....e5c#",
    "#####.##$##.#####",
    "p..bg...?...#####",
    "#################",
    "#################",
    "#################"
};

const char* level_11[] = {
    "########@########",
    "p..hh23..########",
    "#..###$##########",
    "#..?2%&!.########",
    "#....#$..########",
    "######c..########",
    "#################",
    ".................",
    "#################",
    ".................",
    "#################"
};

const char* level_12[] = {
    "#################",
    "#################",
    "#####......######",
    "##..g....*6?...b#",
    "##*##.....1####h#",
    "##*#######$####1@",
    "##*##622227%%%%h#",
    "##.ch4...*5!....#",
    "##.##......######",
    "##.##############",
    "##p##############"
};

const char* level_13[] = {
    "#############@###",
    "####e.....###.###",
    "####.###..###.###",
    "#b..!222362g..###",
    "#$##.###11#######",
    "p5222%%%74#######",
    "##.*....?...#####",
    "##...############",
    "##.*.############",
    "##...############",
    "#################"
};

const char* level_14[] = {
    "#################",
    "#.........#######",
    "#eke......#######",
    "#.........#######",
    "#...###...#######",
    "#...###...#######",
    "#...###....bg.d.#",
    "#.........###.#.#",
    "#.......!2%%?.#.#",
    "#.........###.#.@",
    "#############p###"
};

const char* level_15[] = {
    "#################",
    "#.....!.?...ed.d#",
    "#.....524....d.d#",
    "#............d.d#",
    "#............d.d#",
    "#.e..........d.d#",
    "#............d.d@",
    "#............d.d#",
    "######.k.########",
    "p...ch...########",
    "#################"
};

const char* level_16[] = {
    "#################",
    "############..d.@",
    "############..d##",
    "###6222%223##g###",
    "###1...#.#!##g%b#",
    "###1...#.#....#.#",
    "p..1...h........#",
    "###?.c2h.....k..#",
    "###############.#",
    "###############e#",
    "#################"
};

const char* level_17[] = {
    "#################",
    "p.ch....?...62g.@",
    "####6%%%7%%%4####",
    "####1###53#######",
    "####1#...1##.####",
    "####1...e522!####",
    "####1##....#.####",
    "####$##.*###.####",
    "####1###*###.####",
    "#b224........####",
    "#################"
};

const char* level_18[] = {
    "#############@###",
    "p.ch.....?236g###",
    "#####****.*11.###",
    "#####......11.###",
    "#####......11.###",
    "#####......11.###",
    "#####......11.###",
    "#####......11.###",
    "#####......57!###",
    "#####eeeee..5b###",
    "#################"
};

const char* level_19[] = {
    "#################",
    "#################",
    "#################",
    "#################",
    "#################",
    "#################",
    "#################",
    ";....g.....!hc###",
    "#####$#######.###",
    ";....gb....?#.###",
    "#############p###"
};

const char* level_20[] = {
    "########.########",
    "...hh23..########",
    "#..###$##########",
    "#..?2%&!.########",
    "#....#$..########",
    "######c..########",
    "#################",
    ";...............p",
    "#################",
    ";...............p",
    "#################"
};

const char* level_21[] = {
    "#################",
    "####........#####",
    "####........#####",
    ";.##........#####",
    "#.##....##.*#####",
    "#.......##..#####",
    "####.ee.##..#####",
    "##########......p",
    "##########..#####",
    "##########......p",
    "#################"
};

const char* level_22[] = {
    "#################",
    "##.............##",
    "#####.#####.#####",
    ";...............p",
    "#################",
    "##.....?d......##",
    "#######5%3#######",
    "..k.k.d.d1..d....",
    "#####.#.#!#.#####",
    "##......#......##",
    "#################"
};

const char* level_23[] = {
    "############@####",
    "############.####",
    "###...#...#...###",
    "###.!2%2?.#.....p",
    "###...#...#...###",
    "####.###.########",
    "###...#...d.d.###",
    "......#.k.d.d....",
    "###...#...d.d.###",
    "#################",
    "#################"
};

const char* level_24[] = {
    "############@####",
    "############.####",
    "############.####",
    "############.####",
    "############.####",
    "############.####",
    "############.####",
    "############.####",
    "############.####",
    "############.####",
    "############p####"
};

const char* level_25[] = {
    "########@########",
    "########.########",
    "########.########",
    "########.########",
    "########.########",
    "########.########",
    "#######...#...###",
    "#######.?.#.!e###",
    "#######...#...###",
    "############.####",
    "############p####"
};

// Array of level pointers
const char** levels[] = {
    level_1,
    level_2,
    level_3,
    level_4,
    level_5,
    level_6,
    level_7,
    level_8,
    level_9,
    level_10,
    level_11,
    level_12,
    level_13,
    level_14,
    level_15,
    level_16,
    level_17,
    level_18,
    level_19,
    level_20,
    level_21,
    level_22,
    level_23,
    level_24,
    level_25
};

#define NUM_LEVELS 25

// Graphics setup function for 16x16 mode
void setup_duplicator_graphics(void) {
    word charset_addr = (word)CHARSET_MEM;
    word dlist_addr = (word)DLIST_MEM;
    word screen_addr = (word)SCREEN_MEM;
    byte i;

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

    // Set character set pointer (CHBAS at 756) - BEFORE copying graphics
    POKE(756, (byte)(charset_addr >> 8));

    // Copy ROM font to RAM
    memcpy(CHARSET_MEM, (byte*)ROM_CHARSET_ADDRESS, 1024);

    // Install pre-scaled 16x16 graphics
    // Copy directly to charset position 0x00
    // Screen code 0x00 maps to charset 0x00 (not 0x20!)
    // We have 21 tiles * 32 bytes = 672 bytes total
    memcpy(CHARSET_MEM, duplicator_graphics_16x16, sizeof(duplicator_graphics_16x16));

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

