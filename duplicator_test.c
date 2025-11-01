// Link the libraries
//#link "atari_conio.c"

/*
  Duplicator Font Test
  Displays all converted fonts from the Duplicator game
*/

#include "duplicator_font.h"
#include "atari_conio.h"

#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <conio.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Memory definitions
#define CHARSET_MEM ((byte*)0x7000)
#define DLIST_MEM   ((byte*)0x8000)
#define SCREEN_MEM  ((byte*)0x9000)
#define ROM_CHARSET_ADDRESS 0xE000

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
    // Player '@' = 0x40
    memcpy(CHARSET_MEM + (CHAR_PLAYER * 8), &duplicator_graphics[0], 8);
    
    // Wall '#' = 0x23
    memcpy(CHARSET_MEM + (CHAR_WALL * 8), &duplicator_graphics[8], 8);
    
    // Crate '*' = 0x2A
    memcpy(CHARSET_MEM + (CHAR_CRATE * 8), &duplicator_graphics[16], 8);
    
    // Key 'k' = 0x6B
    memcpy(CHARSET_MEM + (CHAR_KEY * 8), &duplicator_graphics[24], 8);
    
    // Door 'd' = 0x64
    memcpy(CHARSET_MEM + (CHAR_DOOR * 8), &duplicator_graphics[32], 8);
    
    // Enemy 'e' = 0x65
    memcpy(CHARSET_MEM + (CHAR_ENEMY * 8), &duplicator_graphics[40], 8);
    
    // Hole A '?' = 0x3F
    memcpy(CHARSET_MEM + (CHAR_HOLE_A * 8), &duplicator_graphics[48], 8);
    
    // Hole B '!' = 0x21
    memcpy(CHARSET_MEM + (CHAR_HOLE_B * 8), &duplicator_graphics[56], 8);
    
    // Plate A 'b' = 0x62
    memcpy(CHARSET_MEM + (CHAR_PLATE_A * 8), &duplicator_graphics[64], 8);
    
    // Plate B 'c' = 0x63
    memcpy(CHARSET_MEM + (CHAR_PLATE_B * 8), &duplicator_graphics[72], 8);
    
    // Gate A 'g' = 0x67
    memcpy(CHARSET_MEM + (CHAR_GATE_A * 8), &duplicator_graphics[80], 8);
    
    // Gate B 'h' = 0x68
    memcpy(CHARSET_MEM + (CHAR_GATE_B * 8), &duplicator_graphics[88], 8);
    
    // Exit A ':' = 0x3A
    memcpy(CHARSET_MEM + (CHAR_EXIT_A * 8), &duplicator_graphics[96], 8);
    
    // Exit B ';' = 0x3B
    memcpy(CHARSET_MEM + (CHAR_EXIT_B * 8), &duplicator_graphics[104], 8);
    
    // Floor '.' = 0x2E
    memcpy(CHARSET_MEM + (CHAR_FLOOR * 8), &duplicator_graphics[112], 8);
    
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
    byte i;
    
    // Initialize graphics
    setup_duplicator_graphics();

    // Clear screen
    my_clrscr();
    
    // Display title
    my_cputsxy(5, 0, "DUPLICATOR FONT TEST");
    my_cputsxy(5, 1, "====================");
    
    // Display all characters with labels
    my_cputsxy(2, 3, "@ PLAYER");
    my_cputsxy(2, 4, "# WALL");
    my_cputsxy(2, 5, "* CRATE");
    my_cputsxy(2, 6, "k KEY");
    my_cputsxy(2, 7, "d DOOR");
    my_cputsxy(2, 8, "e ENEMY");
    
    my_cputsxy(2, 10, "? HOLE A");
    my_cputsxy(2, 11, "! HOLE B");
    my_cputsxy(2, 12, "b PLATE A");
    my_cputsxy(2, 13, "c PLATE B");
    
    my_cputsxy(2, 15, "g GATE A");
    my_cputsxy(2, 16, "h GATE B");
    my_cputsxy(2, 17, ": EXIT A");
    my_cputsxy(2, 18, "; EXIT B");
    my_cputsxy(2, 19, ". FLOOR");
    
    // Display sample level from duplicator
    my_cputsxy(20, 3, "SAMPLE LEVEL:");
    my_cputsxy(20, 5, "########:########");
    my_cputsxy(20, 6, "########.########");
    my_cputsxy(20, 7, "#######...#...###");
    my_cputsxy(20, 8, "#######.@.#.e.###");
    my_cputsxy(20, 9, "#######...#...###");
    my_cputsxy(20, 10, "############.####");
    
    my_cputsxy(2, 21, "PRESS ANY KEY TO EXIT");
    
    // Wait for key press
    while (!kbhit()) {
        wait_vblank();
    }
    cgetc();
}

