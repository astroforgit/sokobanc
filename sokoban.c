/*
  A simple Sokoban game with a custom character set.
  REFACTORED VERSION - Separated into modular libraries:
  - atari_conio: Console I/O functions
  - atari_font: Custom graphics and character set
  - sokoban_game: Game logic

  TILE SIZE SWITCH:
  - Uncomment USE_BIG_TILES to use 16x16 pixel tiles (2x2 character cells)
  - Comment it out to use standard 8x8 pixel tiles (1 character cell)
*/

// ============================================================
// TILE SIZE CONFIGURATION
// ============================================================
// Uncomment the next line to use 16x16 tiles instead of 8x8
//#define USE_BIG_TILES

// ============================================================
// CONDITIONAL INCLUDES BASED ON TILE SIZE
// ============================================================
#ifdef USE_BIG_TILES
    // 16x16 tile mode
    //#link "atari_conio_16x16.c"
    //#link "atari_font_16x16.c"
    //#link "sokoban_game_16x16.c"

    #include "atari_conio_16x16.h"
    #include "atari_font_16x16.h"
    #include "sokoban_game_16x16.h"

    // Create aliases for easier code
    #define setup_graphics      setup_graphics_16x16
    #define load_level          load_level_16x16
    #define draw_level          draw_level_16x16
    #define update_status       update_status_16x16
    #define try_move_player     try_move_player_16x16
    #define is_level_complete   is_level_complete_16x16
    #define animate_player      animate_player_16x16
    #define wait_vblank         wait_vblank_16x16
    #define increment_frame_counter increment_frame_counter_16x16
    #define my_cputsxy          my_cputsxy_16x16
#else
    // 8x8 tile mode (standard)
    //#link "atari_conio.c"
    //#link "atari_font.c"
    //#link "sokoban_game.c"

    #include "atari_conio.h"
    #include "atari_font.h"
    #include "sokoban_game.h"
#endif

#include <stdlib.h>
#include <joystick.h>
#include <conio.h>

// Data types for clarity
typedef unsigned char byte;

// Level data
// This level works for both 8x8 (40x24) and 16x16 (20x12) tile modes
#ifdef USE_BIG_TILES
// Smaller level for 16x16 tiles (20x12 grid)
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
#else
// Original level for 8x8 tiles (40x24 grid)
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
#endif

void main(void) {
    byte joy, last_joy = 0;

    // Initialize joystick
    joy_install(joy_static_stddrv);

    // Setup custom graphics and character set
    setup_graphics();

    // Load and display the level
    load_level(level_1, sizeof(level_1) / sizeof(level_1[0]));
    draw_level();
    update_status();

    // Main game loop
    while (1) {
        // Read joystick input
        joy = joy_read(0);

        // Process joystick movement (only on state change to prevent auto-repeat)
        if (joy != last_joy) {
            if (JOY_UP(joy))    try_move_player(0, -1);
            if (JOY_DOWN(joy))  try_move_player(0, 1);
            if (JOY_LEFT(joy))  try_move_player(-1, 0);
            if (JOY_RIGHT(joy)) try_move_player(1, 0);
        }
        last_joy = joy;

        // Check win condition
        if (is_level_complete()) {
            break;
        }

        // Animate the player character
        animate_player();

        // Wait for vertical blank (synchronize to screen refresh)
        wait_vblank();

        // Increment frame counter
        increment_frame_counter();
    }

    // Display win message
    my_cputsxy(15, 12, "YOU WIN!");

    // Wait for keypress
    cgetc();

    // Infinite loop to prevent exit
    while (1);
}