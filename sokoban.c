/*
  A simple Sokoban game with a custom character set.
  REFACTORED VERSION - Separated into modular libraries:
  - atari_conio: Console I/O functions
  - atari_font: Custom graphics and character set
  - sokoban_game: Game logic

  DISPLAY MODE SWITCHES:
  - Uncomment USE_BIG_TILES to use 16x16 pixel tiles (2x2 character cells)
  - Uncomment USE_MODE6_COLOR to use ANTIC Mode 6 with 5 colors (20x24 grid)
  - Comment both out to use standard 8x8 monochrome mode (40x24 grid)

  NOTE: Only enable ONE mode at a time!
*/

// ============================================================
// DISPLAY MODE CONFIGURATION
// ============================================================
// Uncomment ONE of the following lines:

//#define USE_BIG_TILES      // 16x16 tiles (20x12 grid, monochrome)
//#define USE_MODE6_COLOR    // ANTIC Mode 6 (20x24 grid, 5 colors)
// Default: 8x8 tiles (40x24 grid, monochrome)

// ============================================================
// CONDITIONAL LINK DIRECTIVES - CHOOSE ONE MODE
// ============================================================
// NOTE: Uncomment ONLY the set of #link directives for the mode you want!
// The 8bitworkshop IDE processes ALL //#link comments, so we must
// manually comment/uncomment the correct set.

// MODE 6 COLOR - Uncomment these 3 lines for 5-color mode:
//#link "atari_conio_mode6.c"
//#link "atari_font_mode6.c"
//#link "sokoban_game_mode6.c"

// 16x16 TILES - Uncomment these 3 lines for big tile mode:
//#link "atari_conio_16x16.c"
//#link "atari_font_16x16.c"
//#link "sokoban_game_16x16.c"

// 8x8 DEFAULT - Uncomment these 3 lines for standard mode:
#link "atari_conio.c"
#link "atari_font.c"
#link "sokoban_game.c"

// ============================================================
// CONDITIONAL INCLUDES BASED ON DISPLAY MODE
// ============================================================
#ifdef USE_MODE6_COLOR
    #include "atari_conio_mode6.h"
    #include "atari_font_mode6.h"
    #include "sokoban_game_mode6.h"

    // Create aliases for easier code
    #define setup_graphics      setup_graphics_mode6
    #define load_level          load_level_mode6
    #define draw_level          draw_level_mode6
    #define update_status       update_status_mode6
    #define try_move_player     try_move_player_mode6
    #define is_level_complete   is_level_complete_mode6
    #define animate_player      animate_player_mode6
    #define wait_vblank         wait_vblank_mode6
    #define increment_frame_counter increment_frame_counter_mode6
    #define my_cputsxy          my_cputsxy_mode6

#elif defined(USE_BIG_TILES)
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
// Different levels for different display modes
#ifdef USE_MODE6_COLOR
// Level for ANTIC Mode 6 (20x24 grid with colors)
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

#elif defined(USE_BIG_TILES)
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