// Test custom character set
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

    // TEST: Create solid blocks for characters 0x20-0x23
    // Screen code 0x00 should map to charset 0x20
    for (i = 0; i < 8; i++) {
        CHARSET_MEM[0x20 * 8 + i] = 0xFF;  // Solid block
    }
    for (i = 0; i < 8; i++) {
        CHARSET_MEM[0x21 * 8 + i] = 0xFF;  // Solid block
    }
    for (i = 0; i < 8; i++) {
        CHARSET_MEM[0x22 * 8 + i] = 0xFF;  // Solid block
    }
    for (i = 0; i < 8; i++) {
        CHARSET_MEM[0x23 * 8 + i] = 0xFF;  // Solid block
    }
    
    // Set colors
    POKE(709, 0);   // Background color (black)
    POKE(710, 14);  // Foreground color (light blue)

    // Set display list pointer
    POKE(560, (byte)dlist_addr);
    POKE(561, (byte)(dlist_addr >> 8));

    // Clear screen
    memset(SCREEN_MEM, 0, 40 * 24);
    
    // Write screen code 0x00 which should map to charset 0x20 (solid block)
    SCREEN_MEM[0] = 0x00;
    SCREEN_MEM[1] = 0x01;
    SCREEN_MEM[2] = 0x02;
    SCREEN_MEM[3] = 0x03;

    // These should all be solid blocks if the mapping is correct
    
    // Turn off cursor
    POKE(752, 1);
    
    // Infinite loop
    while (1) {}
}

