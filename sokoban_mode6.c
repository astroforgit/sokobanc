/*
  Sokoban - ANTIC Mode 6 Color
  20x24 character grid, 5 colors!
  
  This is the main file for the ANTIC Mode 6 color mode.
  Open THIS file in 8bitworkshop to play in glorious color!
  
  Colors:
  - Brown: Walls and boxes
  - Green: Player
  - Yellow: Goals
  - Red: Boxes on goals
*/

// Link the Mode 6 libraries
#link "atari_conio_mode6.c"
#link "atari_font_mode6.c"
#link "sokoban_game_mode6.c"

#include "atari_conio_mode6.h"
#include "atari_font_mode6.h"
#include "sokoban_game_mode6.h"

#include <stdlib.h>
#include <joystick.h>
#include <conio.h>

// Data types for clarity
typedef unsigned char byte;

// Level data for Mode 6 (20x24 grid with colors)
const char* level_1[] = {
    "    ######          ",
    "    #    #          ",
    "    # $  #          ",
    "  ### $  ##         ",
    "  #  $ $  #         ",
    "  # # ##  #         ",
    "  # # ##  #         ",
    "  #  $  ..#         ",
    "  #### #@.#         ",
    "     #   .#         ",
    "     ######         "
};

// Number of rows in the level
#define LEVEL_ROWS 11

// Main game function
void main(void) {
    byte joy;
    
    // Initialize graphics and load level
    setup_graphics_mode6();
    load_level_mode6(level_1, LEVEL_ROWS);
    draw_level_mode6();
    update_status_mode6();
    
    // Display title in color
    my_cputsxy_mode6(0, 0, "SOKOBAN MODE 6");
    
    // Main game loop
    while (1) {
        // Read joystick
        joy = joy_read(0);
        
        // Handle input
        if (JOY_UP(joy)) {
            try_move_player_mode6(0, -1);
            wait_vblank_mode6();
        } else if (JOY_DOWN(joy)) {
            try_move_player_mode6(0, 1);
            wait_vblank_mode6();
        } else if (JOY_LEFT(joy)) {
            try_move_player_mode6(-1, 0);
            wait_vblank_mode6();
        } else if (JOY_RIGHT(joy)) {
            try_move_player_mode6(1, 0);
            wait_vblank_mode6();
        }
        
        // Also check keyboard
        if (kbhit()) {
            byte key = cgetc();
            if (key == CH_CURS_UP || key == 'w' || key == 'W') {
                try_move_player_mode6(0, -1);
            } else if (key == CH_CURS_DOWN || key == 's' || key == 'S') {
                try_move_player_mode6(0, 1);
            } else if (key == CH_CURS_LEFT || key == 'a' || key == 'A') {
                try_move_player_mode6(-1, 0);
            } else if (key == CH_CURS_RIGHT || key == 'd' || key == 'D') {
                try_move_player_mode6(1, 0);
            } else if (key == CH_ESC) {
                break;  // Exit game
            }
        }
        
        // Animate player
        animate_player_mode6();
        increment_frame_counter_mode6();
        
        // Check win condition
        if (is_level_complete_mode6()) {
            my_cputsxy_mode6(5, 12, "YOU WIN!");
            wait_vblank_mode6();
            wait_vblank_mode6();
            wait_vblank_mode6();
            // Wait for key press
            while (!kbhit()) {
                wait_vblank_mode6();
            }
            break;
        }
        
        wait_vblank_mode6();
    }
}

