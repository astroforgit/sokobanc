// Link the 8x8 mode libraries
//#link "atari_conio.c"
//#link "atari_font.c"
//#link "sokoban_game.c"

/*
  Sokoban - 8x8 Standard Mode
  40x24 character grid, monochrome

  This is the main file for the standard 8x8 tile mode.
  Open THIS file in 8bitworkshop to play in standard mode.
*/

#include "atari_conio.h"
#include "atari_font.h"
#include "sokoban_game.h"

#include <stdlib.h>
#include <joystick.h>
#include <conio.h>

// Data types for clarity
typedef unsigned char byte;

// Level data for 8x8 mode (40x24 grid)
const char* level_1[] = {
    "    #####",
    "    #   #",
    "    #$  #",
    "  ###  $##",
    "  #  $ $ #",
    "### # ## #   ######",
    "#   # ## #####  ..#",
    "# $  $          ..#",
    "##### ### #@##  ..#",
    "    #     #########",
    "    #######"
};

// Number of rows in the level
#define LEVEL_ROWS 11

// Main game function
void main(void) {
    byte joy, last_joy = 0;

    // Initialize joystick
    joy_install(joy_static_stddrv);

    // Initialize graphics and load level
    setup_graphics();
    load_level(level_1, LEVEL_ROWS);
    draw_level();
    update_status();
    
    // Display title
    my_cputsxy(0, 0, "SOKOBAN - 8x8 MODE");
    
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
        
        // Animate player
        animate_player();
        increment_frame_counter();
        
        // Check win condition
        if (is_level_complete()) {
            my_cputsxy(10, 12, "YOU WIN!");
            wait_vblank();
            wait_vblank();
            wait_vblank();
            // Wait for key press
            while (!kbhit()) {
                wait_vblank();
            }
            break;
        }
        
        wait_vblank();
    }
}

