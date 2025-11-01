/*
  atari_font.c - Custom font and graphics library for Atari 8-bit
  Implementation file
*/

#include "atari_font.h"

// Animation state variables
static byte frame_counter = 0;
static byte anim_state = 0;

// Graphics Data with Animation Frames
// Each character is 8 bytes (8x8 pixels, 1 bit per pixel)
static unsigned char custom_graphics[] = {
    /*  0 */ 0xFF,0x81,0xBD,0xA5,0xA5,0xBD,0x81,0xFF, // # Wall
    /*  8 */ 0x7E,0x42,0x5A,0x5A,0x5A,0x5A,0x42,0x7E, // $ Box
    /* 16 */ 0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x00, // . Goal
    /* 24 */ 0x7E,0x42,0x42,0x5A,0x5A,0x42,0x42,0x7E, // * Box on Goal
    
    // Player Animation Frames
    /* 32: Player Standing */
    0x00,0x18,0x18,0x7E,0x18,0x18,0x24,0x66,
    /* 40: Player Blinking */
    0x00,0x18,0x18,0x7E,0x18,0x00,0x24,0x66,
    /* 48: Player Walking */
    0x00,0x18,0x18,0x7E,0x18,0x18,0x66,0x24
};

void setup_graphics(void) {
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
    
    // Install custom graphics for game tiles
    memcpy(CHARSET_MEM + ('#' * 8), &custom_graphics[0],  8);  // Wall
    memcpy(CHARSET_MEM + ('$' * 8), &custom_graphics[8],  8);  // Box
    memcpy(CHARSET_MEM + ('.' * 8), &custom_graphics[16], 8);  // Goal
    memcpy(CHARSET_MEM + ('*' * 8), &custom_graphics[24], 8);  // Box on Goal
    
    // Initially set both player chars to the standing graphic
    memcpy(CHARSET_MEM + ('@' * 8), &custom_graphics[32], 8);  // Player
    memcpy(CHARSET_MEM + ('+' * 8), &custom_graphics[32], 8);  // Player on Goal
    
    // Set colors
    // POKE(709, 0)  - Background color (black)
    // POKE(710, 14) - Foreground color (light blue)
    POKE(709, 0);
    POKE(710, 14);
    
    // Set display list pointer (SDLSTL/SDLSTH at 560/561)
    POKEW(560, dlist_addr);
    
    // Turn off cursor
    POKE(752, 0);
}

void animate_player(void) {
    // Get pointers to BOTH player character locations in the character set
    byte* player_char_data = CHARSET_MEM + ('@' * 8);
    byte* player_goal_char_data = CHARSET_MEM + ('+' * 8);

    if (anim_state == 1) { // Walking animation
        // Alternate between standing and walking frames based on the timer
        if (frame_counter < 4) {
            memcpy(player_char_data, &custom_graphics[32], 8); // Standing
            memcpy(player_goal_char_data, &custom_graphics[32], 8);
        } else {
            memcpy(player_char_data, &custom_graphics[48], 8); // Walking
            memcpy(player_goal_char_data, &custom_graphics[48], 8);
        }
        if (frame_counter > 8) {
            anim_state = 0; // Stop walking anim after 8 frames
        }
    } else { // Idle animation (blinking)
        // Default to the standing frame
        memcpy(player_char_data, &custom_graphics[32], 8);
        memcpy(player_goal_char_data, &custom_graphics[32], 8);
        // But for a short time, show the blinking frame
        if (frame_counter > 50 && frame_counter < 53) {
            memcpy(player_char_data, &custom_graphics[40], 8); // Blinking
            memcpy(player_goal_char_data, &custom_graphics[40], 8);
        }
    }
}

byte get_anim_state(void) {
    return anim_state;
}

void set_anim_state(byte state) {
    anim_state = state;
}

byte get_frame_counter(void) {
    return frame_counter;
}

void set_frame_counter(byte counter) {
    frame_counter = counter;
}

void increment_frame_counter(void) {
    frame_counter++;
}

