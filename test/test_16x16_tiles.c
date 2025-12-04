// Test 16x16 tiles with pre-scaled graphics
#define CFGFILE atari-xex.cfg

#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>

typedef unsigned char byte;
typedef unsigned short word;

// Memory definitions
#define CHARSET_MEM ((byte*)0x7000)
#define SCREEN_MEM  ((byte*)0x9000)
#define DLIST_MEM   ((byte*)0x8000)
#define ROM_CHARSET_ADDRESS 0xE000

// Include the pre-scaled 16x16 graphics
#include "duplicator_graphics_16x16.h"
#include "duplicator_font_16x16.h"

// Draw a 16x16 tile at tile coordinates (tx, ty)
void draw_tile_16x16(byte tx, byte ty, byte tile_code) {
    word offset = (ty * 2 * 40) + (tx * 2);
    SCREEN_MEM[offset] = tile_code;          // Top-left
    SCREEN_MEM[offset + 1] = tile_code + 1;  // Top-right
    SCREEN_MEM[offset + 40] = tile_code + 2; // Bottom-left
    SCREEN_MEM[offset + 41] = tile_code + 3; // Bottom-right
}

void main(void) {
    word charset_addr = (word)CHARSET_MEM;
    word screen_addr = (word)SCREEN_MEM;
    word dlist_addr = (word)DLIST_MEM;
    byte i;
    
    // Create custom display list
    DLIST_MEM[0] = 0x70;  // 8 blank lines
    DLIST_MEM[1] = 0x70;  // 8 blank lines
    DLIST_MEM[2] = 0x70;  // 8 blank lines
    
    // First line with LMS (Load Memory Scan)
    DLIST_MEM[3] = 0x42;  // Mode 2 + LMS
    DLIST_MEM[4] = (byte)screen_addr;
    DLIST_MEM[5] = (byte)(screen_addr >> 8);
    
    // 23 more lines of mode 2
    for (i = 0; i < 23; i++) {
        DLIST_MEM[6 + i] = 0x02;
    }
    
    // Jump back to start
    DLIST_MEM[29] = 0x41;
    DLIST_MEM[30] = (byte)dlist_addr;
    DLIST_MEM[31] = (byte)(dlist_addr >> 8);
    
    // Set character set pointer (CHBAS at 756)
    POKE(756, (byte)(charset_addr >> 8));
    
    // Copy ROM font to RAM
    memcpy(CHARSET_MEM, (byte*)ROM_CHARSET_ADDRESS, 1024);

    // Install pre-scaled 16x16 graphics
    // Copy directly to charset position 0x00 (not 0x20)
    memcpy(CHARSET_MEM, duplicator_graphics_16x16, sizeof(duplicator_graphics_16x16));
    
    // Set colors
    POKE(709, 0);   // Background color (black)
    POKE(710, 14);  // Foreground color (light blue)
    
    // Set display list pointer
    POKE(560, (byte)dlist_addr);
    POKE(561, (byte)(dlist_addr >> 8));
    
    // Clear screen
    memset(SCREEN_MEM, 0, 40 * 24);

    // Turn off cursor
    POKE(752, 1);

    // Draw ALL game tiles in a grid (5 columns x 5 rows)
    // Row 0: Wall, Player, Crate, Key, Door
    draw_tile_16x16(0, 0, TILE_WALL_TL);
    draw_tile_16x16(2, 0, TILE_PLAYER_TL);
    draw_tile_16x16(4, 0, TILE_CRATE_TL);
    draw_tile_16x16(6, 0, TILE_KEY_TL);
    draw_tile_16x16(8, 0, TILE_DOOR_TL);

    // Row 2: Enemy, Hole A, Hole B, Plate A, Plate B
    draw_tile_16x16(0, 2, TILE_ENEMY_TL);
    draw_tile_16x16(2, 2, TILE_HOLE_A_TL);
    draw_tile_16x16(4, 2, TILE_HOLE_B_TL);
    draw_tile_16x16(6, 2, TILE_PLATE_A_TL);
    draw_tile_16x16(8, 2, TILE_PLATE_B_TL);

    // Row 4: Gate A, Gate B, Exit A, Exit B, Exit C
    draw_tile_16x16(0, 4, TILE_GATE_A_TL);
    draw_tile_16x16(2, 4, TILE_GATE_B_TL);
    draw_tile_16x16(4, 4, TILE_EXIT_A_TL);
    draw_tile_16x16(6, 4, TILE_EXIT_B_TL);
    draw_tile_16x16(8, 4, TILE_EXIT_C_TL);

    // Row 6: Floor, Gate A Open, Gate B Open, Door Open, Hole A Fill
    draw_tile_16x16(0, 6, TILE_FLOOR_TL);
    draw_tile_16x16(2, 6, TILE_GATE_A_OPEN_TL);
    draw_tile_16x16(4, 6, TILE_GATE_B_OPEN_TL);
    draw_tile_16x16(6, 6, TILE_DOOR_OPEN_TL);
    draw_tile_16x16(8, 6, TILE_HOLE_A_FILL_TL);

    // Row 8: Hole B Fill
    draw_tile_16x16(0, 8, TILE_HOLE_B_FILL_TL);

    // Infinite loop
    while (1) {}
}

