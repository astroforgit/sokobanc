/*
  Sokoban - 16x16 Big Tile Mode
  20x12 tile grid, monochrome, detailed graphics
  
  This is the main file for the 16x16 tile mode.
  Open THIS file in 8bitworkshop to play with big tiles.
*/

// Link the 16x16 mode libraries
//#link "atari_conio_16x16.c"
//#link "atari_font_16x16.c"
//#link "sokoban_game_16x16.c"

#include "atari_conio_16x16.h"
#include "atari_font_16x16.h"
#include "sokoban_game_16x16.h"

#include <stdlib.h>
#include <joystick.h>
#include <conio.h>

// Data types for clarity
typedef unsigned char byte;

// Level data for 16x16 mode (20x12 grid)
const char* level_1[] = {
    "  ######  ",
    "  #    #  ",
    "  # $  #  ",
    " ## $  ## ",
    " # $ $  # ",
    " #  ##  # ",
    " # ##  .# ",
    " #  $ ..# ",
    " ## #@..# ",
    "  #    ## ",
    "  ###### "
};

// Number of rows in the level
#define LEVEL_ROWS 11

// Main game function
void main(void) {
    byte joy;
    
    // Initialize graphics and load level
    setup_graphics_16x16();
    load_level_16x16(level_1, LEVEL_ROWS);
    draw_level_16x16();
    update_status_16x16();
    
    // Display title
    my_cputsxy_16x16(0, 0, "SOKOBAN 16x16");
    
    // Main game loop
    while (1) {
        // Read joystick
        joy = joy_read(0);
        
        // Handle input
        if (JOY_UP(joy)) {
            try_move_player_16x16(0, -1);
            wait_vblank_16x16();
        } else if (JOY_DOWN(joy)) {
            try_move_player_16x16(0, 1);
            wait_vblank_16x16();
        } else if (JOY_LEFT(joy)) {
            try_move_player_16x16(-1, 0);
            wait_vblank_16x16();
        } else if (JOY_RIGHT(joy)) {
            try_move_player_16x16(1, 0);
            wait_vblank_16x16();
        }
        
        // Also check keyboard
        if (kbhit()) {
            byte key = cgetc();
            if (key == CH_CURS_UP || key == 'w' || key == 'W') {
                try_move_player_16x16(0, -1);
            } else if (key == CH_CURS_DOWN || key == 's' || key == 'S') {
                try_move_player_16x16(0, 1);
            } else if (key == CH_CURS_LEFT || key == 'a' || key == 'A') {
                try_move_player_16x16(-1, 0);
            } else if (key == CH_CURS_RIGHT || key == 'd' || key == 'D') {
                try_move_player_16x16(1, 0);
            } else if (key == CH_ESC) {
                break;  // Exit game
            }
        }
        
        // Animate player
        animate_player_16x16();
        increment_frame_counter_16x16();
        
        // Check win condition
        if (is_level_complete_16x16()) {
            my_cputsxy_16x16(5, 6, "YOU WIN!");
            wait_vblank_16x16();
            wait_vblank_16x16();
            wait_vblank_16x16();
            // Wait for key press
            while (!kbhit()) {
                wait_vblank_16x16();
            }
            break;
        }
        
        wait_vblank_16x16();
    }
}

