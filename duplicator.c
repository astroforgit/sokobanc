// Link the libraries
//#link "atari_conio.c"
//#link "duplicator_game.c"

/*
  Duplicator Game - Playable Version
  Based on the PuzzleScript game by competor
*/

#include "duplicator_font.h"
#include "duplicator_game.h"
#include "atari_conio.h"

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
    "#######.@.#.e.###",
    "#######...#...###",
    "############.####",
    "############.####"
};

const char* level_2[] = {
    "#################",
    "#################",
    "###...#...#...###",
    "###.!.#.?...b.###",
    "###...#...#...###",
    "####.############",
    "####.##...#######",
    "####........g...:",
    "#######...#######",
    "########.########",
    "########@########"
};

// Level definitions
typedef struct {
    const char** data;
    byte rows;
} LevelDef;

const LevelDef levels[] = {
    { level_1, 11 },
    { level_2, 11 }
};

#define NUM_LEVELS 2

// Setup graphics with duplicator fonts
void setup_duplicator_graphics(void) {
    byte i;
    word screen_addr = (word)SCREEN_MEM;
    word dlist_addr = (word)DLIST_MEM;
    word charset_addr = (word)CHARSET_MEM;
    
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
    
    // Set character set pointer (CHBAS register at location 756)
    POKE(756, (byte)(charset_addr >> 8));
    
    // Copy ROM character set to RAM so we can modify it
    memcpy(CHARSET_MEM, (void*)ROM_CHARSET_ADDRESS, 1024);
    
    // Install custom graphics for duplicator game tiles
    // Use direct character multiplication like sokoban does
    memcpy(CHARSET_MEM + ('@' * 8), &duplicator_graphics[0], 8);   // Player
    memcpy(CHARSET_MEM + ('#' * 8), &duplicator_graphics[8], 8);   // Wall
    memcpy(CHARSET_MEM + ('*' * 8), &duplicator_graphics[16], 8);  // Crate
    memcpy(CHARSET_MEM + ('k' * 8), &duplicator_graphics[24], 8);  // Key
    memcpy(CHARSET_MEM + ('d' * 8), &duplicator_graphics[32], 8);  // Door
    memcpy(CHARSET_MEM + ('e' * 8), &duplicator_graphics[40], 8);  // Enemy
    memcpy(CHARSET_MEM + ('?' * 8), &duplicator_graphics[48], 8);  // Hole A
    memcpy(CHARSET_MEM + ('!' * 8), &duplicator_graphics[56], 8);  // Hole B
    memcpy(CHARSET_MEM + ('b' * 8), &duplicator_graphics[64], 8);  // Plate A
    memcpy(CHARSET_MEM + ('c' * 8), &duplicator_graphics[72], 8);  // Plate B
    memcpy(CHARSET_MEM + ('g' * 8), &duplicator_graphics[80], 8);  // Gate A
    memcpy(CHARSET_MEM + ('h' * 8), &duplicator_graphics[88], 8);  // Gate B
    memcpy(CHARSET_MEM + (':' * 8), &duplicator_graphics[96], 8);  // Exit A
    memcpy(CHARSET_MEM + (';' * 8), &duplicator_graphics[104], 8); // Exit B
    memcpy(CHARSET_MEM + ('.' * 8), &duplicator_graphics[112], 8); // Floor
    
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
    char title_buf[25];

    // Initialize joystick
    joy_install(joy_static_stddrv);

    // Initialize graphics
    setup_duplicator_graphics();

start_level:
    // Clear screen
    my_clrscr();

    // Load and draw current level
    load_level(levels[current_level].data, levels[current_level].rows);
    state = get_game_state();
    state->current_level = current_level;
    draw_level();

    // Display title with level number
    sprintf(title_buf, "DUPLICATOR - LEVEL %d", current_level + 1);
    my_cputsxy(0, 0, title_buf);
    
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
            } else if (key == CH_ESC) {
                break;  // Exit game
            }
        }
        
        // Display move counter
        state = get_game_state();
        my_cputsxy(0, 23, "MOVES: ");
        // Simple number display (only works for < 100)
        if (state->moves < 10) {
            my_cputcxy(7, 23, ' ');
            my_cputcxy(8, 23, '0' + state->moves);
        } else {
            my_cputcxy(7, 23, '0' + (state->moves / 10));
            my_cputcxy(8, 23, '0' + (state->moves % 10));
        }
        
        // Check win condition
        if (is_level_complete()) {
            // Check if there are more levels
            if (current_level < NUM_LEVELS - 1) {
                // Go to next level
                my_cputsxy(8, 12, "LEVEL COMPLETE!");
                my_cputsxy(8, 13, "PRESS ANY KEY");

                // Wait for key press
                while (!kbhit()) {
                    wait_vblank();
                }
                cgetc();  // Clear the key

                current_level++;
                goto start_level;
            } else {
                // All levels complete
                my_cputsxy(7, 12, "ALL LEVELS DONE!");
                my_cputsxy(8, 13, "PRESS ANY KEY");

                // Wait for key press
                while (!kbhit()) {
                    wait_vblank();
                }
                break;
            }
        }

        wait_vblank();
    }
}

