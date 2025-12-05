// Use RAM-based configuration
#define CFGFILE atari-xex.cfg

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <peekpoke.h>
#include <conio.h>
#include <joystick.h>

typedef unsigned char byte;
typedef unsigned short word;

// Memory definitions
#define CHARSET_MEM ((byte*)0x7000)
#define DLIST_MEM   ((byte*)0x8000)
#define SCREEN_MEM  ((byte*)0x9000)

#include "satanic_game.h"
#include "satanic_conio_16x16.h"
#include "satanic_graphics_16x16.h"
#include "satanic_levels_16x16.h"

// Graphics setup
void setup_graphics(void) {
    word charset_addr = (word)CHARSET_MEM;
    word dlist_addr = (word)DLIST_MEM;
    word screen_addr = (word)SCREEN_MEM;
    byte i;

    // Create custom display list
    DLIST_MEM[0] = 0x70; DLIST_MEM[1] = 0x70; DLIST_MEM[2] = 0x70;
    DLIST_MEM[3] = 0x42;
    DLIST_MEM[4] = (byte)screen_addr;
    DLIST_MEM[5] = (byte)(screen_addr >> 8);
    for (i = 0; i < 23; ++i) DLIST_MEM[6 + i] = 0x02;
    DLIST_MEM[29] = 0x41;
    DLIST_MEM[30] = (byte)dlist_addr;
    DLIST_MEM[31] = (byte)(dlist_addr >> 8);

    POKE(756, (byte)(charset_addr >> 8));
    memcpy(CHARSET_MEM, satanic_graphics_16x16, sizeof(satanic_graphics_16x16));
    POKE(709, 0); POKE(710, 14);
    POKEW(560, dlist_addr);
    POKE(752, 1);
}

void main(void) {
    byte joy, last_joy = 0;
    GameState* state;

    joy_install(joy_static_stddrv);
    setup_graphics();
    init_game_state();

    // Get game state pointer
    state = get_game_state();

    // Load first level
    load_level(state->current_level);

    while (1) {
        joy = joy_read(0);
        if (joy && !last_joy) {
            if (JOY_UP(joy)) try_move(0, -1);
            else if (JOY_DOWN(joy)) try_move(0, 1);
            else if (JOY_LEFT(joy)) try_move(-1, 0);
            else if (JOY_RIGHT(joy)) try_move(1, 0);
        }
        last_joy = joy;
        wait_vblank_16x16();
    }
}
