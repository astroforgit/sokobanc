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

#include "satanic_conio_16x16.h"
#include "satanic_graphics_16x16.h"
#include "satanic_levels_16x16.h"

// Game state
byte player_x, player_y;
byte current_level = 0;
char level_map[12][20];

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

void load_level(byte level_num) {
    byte x, y;
    const char** level_data = levels[level_num];
    my_clrscr_16x16();

    for (y = 0; y < 11; y++) {
        for (x = 0; x < 17; x++) {
            char tile = level_data[y][x];
            if (tile == 'p') {
                player_x = x; player_y = y;
                level_map[y][x] = ' ';
            } else {
                level_map[y][x] = tile;
                my_cputcxy_16x16(x, y, map_tile_to_16x16(tile));
            }
        }
    }
    my_cputcxy_16x16(player_x, player_y, map_tile_to_16x16('p'));
}

void try_move(signed char dx, signed char dy) {
    byte new_x = player_x + dx;
    byte new_y = player_y + dy;
    char tile = level_map[new_y][new_x];

    if (tile != '#') {
        my_cputcxy_16x16(player_x, player_y, map_tile_to_16x16(level_map[player_y][player_x]));
        player_x = new_x; player_y = new_y;
        my_cputcxy_16x16(player_x, player_y, map_tile_to_16x16('p'));
        if (tile == '@') {
            current_level++;
            if (current_level >= NUM_LEVELS) current_level = 0;
            load_level(current_level);
        }
    }
}

void main(void) {
    byte joy, last_joy = 0;
    joy_install(joy_static_stddrv);
    setup_graphics();
    load_level(current_level);

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
