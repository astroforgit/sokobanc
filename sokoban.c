/*
  A simple Sokoban game with a custom character set.
  REFACTORED VERSION - Separated into modular libraries:
  - atari_conio: Console I/O functions
  - atari_font: Custom graphics and character set
  - sokoban_game: Game logic
*/

//#link "atari_conio.c"
//#link "atari_font.c"
//#link "sokoban_game.c"

#include <stdlib.h>
#include <joystick.h>
#include <conio.h>

#include "atari_conio.h"
#include "atari_font.h"
#include "sokoban_game.h"

// Data types for clarity
typedef unsigned char byte;

// Level data
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