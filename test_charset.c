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
    
    // Create a test pattern for character 0x80 (128)
    // Make it a checkerboard pattern
    CHARSET_MEM[0x80 * 8 + 0] = 0xAA;  // 10101010
    CHARSET_MEM[0x80 * 8 + 1] = 0x55;  // 01010101
    CHARSET_MEM[0x80 * 8 + 2] = 0xAA;  // 10101010
    CHARSET_MEM[0x80 * 8 + 3] = 0x55;  // 01010101
    CHARSET_MEM[0x80 * 8 + 4] = 0xAA;  // 10101010
    CHARSET_MEM[0x80 * 8 + 5] = 0x55;  // 01010101
    CHARSET_MEM[0x80 * 8 + 6] = 0xAA;  // 10101010
    CHARSET_MEM[0x80 * 8 + 7] = 0x55;  // 01010101
    
    // Create a solid block for character 0x81
    for (i = 0; i < 8; i++) {
        CHARSET_MEM[0x81 * 8 + i] = 0xFF;  // All pixels on
    }
    
    // Create a border pattern for character 0x82
    CHARSET_MEM[0x82 * 8 + 0] = 0xFF;  // ########
    CHARSET_MEM[0x82 * 8 + 1] = 0x81;  // #......#
    CHARSET_MEM[0x82 * 8 + 2] = 0x81;  // #......#
    CHARSET_MEM[0x82 * 8 + 3] = 0x81;  // #......#
    CHARSET_MEM[0x82 * 8 + 4] = 0x81;  // #......#
    CHARSET_MEM[0x82 * 8 + 5] = 0x81;  // #......#
    CHARSET_MEM[0x82 * 8 + 6] = 0x81;  // #......#
    CHARSET_MEM[0x82 * 8 + 7] = 0xFF;  // ########
    
    // Set colors
    POKE(709, 0);   // Background color (black)
    POKE(710, 14);  // Foreground color (light blue)

    // Set display list pointer
    POKE(560, (byte)dlist_addr);
    POKE(561, (byte)(dlist_addr >> 8));

    // Clear screen
    memset(SCREEN_MEM, 0, 40 * 24);
    
    // Write test message using ROM characters
    SCREEN_MEM[0] = 'T';
    SCREEN_MEM[1] = 'E';
    SCREEN_MEM[2] = 'S';
    SCREEN_MEM[3] = 'T';
    SCREEN_MEM[4] = ':';
    
    // Write custom characters
    SCREEN_MEM[40] = 0x80;  // Checkerboard
    SCREEN_MEM[41] = 0x80;
    SCREEN_MEM[42] = 0x80;
    SCREEN_MEM[43] = 0x80;
    
    SCREEN_MEM[80] = 0x81;  // Solid block
    SCREEN_MEM[81] = 0x81;
    SCREEN_MEM[82] = 0x81;
    SCREEN_MEM[83] = 0x81;
    
    SCREEN_MEM[120] = 0x82;  // Border
    SCREEN_MEM[121] = 0x82;
    SCREEN_MEM[122] = 0x82;
    SCREEN_MEM[123] = 0x82;
    
    // Turn off cursor
    POKE(752, 1);
    
    // Infinite loop
    while (1) {}
}

