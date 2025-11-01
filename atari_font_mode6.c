/*
  atari_font_mode6.c - ANTIC Mode 6 (Graphics 1) color graphics library
  Implementation file
*/

#include "atari_font_mode6.h"

// Animation state
static byte anim_state_mode6 = 0;
static byte frame_counter_mode6 = 0;

// Display list for ANTIC Mode 6 (Graphics 1)
// 20 columns x 24 rows, 5 colors
static unsigned char display_list_mode6[] = {
    0x70, 0x70, 0x70,                    // 24 blank lines (top border)
    0x46, 0x00, 0x90,                    // Mode 6 + LMS, screen at $9000
    0x06, 0x06, 0x06, 0x06, 0x06,        // 5 more Mode 6 lines
    0x06, 0x06, 0x06, 0x06, 0x06,        // 5 more Mode 6 lines
    0x06, 0x06, 0x06, 0x06, 0x06,        // 5 more Mode 6 lines
    0x06, 0x06, 0x06, 0x06, 0x06,        // 5 more Mode 6 lines
    0x06, 0x06, 0x06,                    // 3 more Mode 6 lines (total 24)
    0x41, 0x00, 0x80                     // JVB - Jump and wait for VBlank
};

// Custom character set data
// In Mode 6, characters are 8x8 pixels, 1 bit per pixel
// We'll define custom graphics for game elements
static unsigned char custom_graphics_mode6[] = {
    // Wall character (0x40 = 64, PF0 brown) - Brick pattern
    0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0x3C, 0x3C,

    // Box character (0x41 = 65, PF0 brown) - Crate
    0xFF, 0x81, 0xBD, 0xBD, 0xBD, 0xBD, 0x81, 0xFF,

    // Goal character (0x42 = 66, PF2 yellow) - Circle/target
    0x00, 0x3C, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x00,

    // Box on goal (0x43 = 67, PF3 red) - Box + circle
    0xFF, 0xC3, 0xBD, 0xBD, 0xBD, 0xBD, 0xC3, 0xFF,

    // Player character (0x80 = 128, PF1 green) - Stick figure
    0x18, 0x3C, 0x18, 0x7E, 0x18, 0x24, 0x42, 0x81,

    // Player on goal (0x81 = 129, PF1 green) - Stick figure
    0x18, 0x3C, 0x18, 0x7E, 0x18, 0x24, 0x42, 0x81
};

void setup_graphics_mode6(void) {
    // Install display list
    memcpy((void*)DLIST_MEM_MODE6, display_list_mode6, sizeof(display_list_mode6));

    // Point display list pointer to our custom display list
    OS.sdlstl = (unsigned char)(DLIST_MEM_MODE6 & 0xFF);
    OS.sdlsth = (unsigned char)(DLIST_MEM_MODE6 >> 8);

    // Setup colors for ANTIC Mode 6
    // In Mode 6, characters use different PF colors based on their value:
    // 0-63: PF0, 64-127: PF2, 128-191: PF1, 192-255: PF3
    OS.color0 = MODE6_COLOR_BLACK;   // Background
    OS.color1 = MODE6_COLOR_BROWN;   // PF0 - walls, boxes
    OS.color2 = MODE6_COLOR_GREEN;   // PF1 - player
    OS.color3 = MODE6_COLOR_YELLOW;  // PF2 - goals
    OS.color4 = MODE6_COLOR_RED;     // PF3 - boxes on goals

    // Copy ROM character set to RAM
    memcpy((void*)CHARSET_MEM_MODE6, (void*)0xE000, 1024);

    // Install custom graphics at specific positions:
    // Characters 64-65 (0x40-0x41): Wall and Box (PF0 brown)
    memcpy((void*)(CHARSET_MEM_MODE6 + 0x40 * 8), custom_graphics_mode6, 16);

    // Character 66 (0x42): Goal (PF2 yellow)
    memcpy((void*)(CHARSET_MEM_MODE6 + 0x42 * 8), custom_graphics_mode6 + 16, 8);

    // Character 67 (0x43): Box on goal (PF3 red) - but we'll use char 195 instead
    memcpy((void*)(CHARSET_MEM_MODE6 + (192 + 3) * 8), custom_graphics_mode6 + 24, 8);

    // Characters 128-129 (0x80-0x81): Player (PF1 green)
    memcpy((void*)(CHARSET_MEM_MODE6 + 0x80 * 8), custom_graphics_mode6 + 32, 16);

    // Point character base to our custom character set
    OS.chbas = (unsigned char)(CHARSET_MEM_MODE6 >> 8);

    // Clear screen
    memset((void*)SCREEN_MEM_MODE6, 0, 480);
}

void animate_player_mode6(void) {
    if (anim_state_mode6 == 1 && frame_counter_mode6 > 30) {
        anim_state_mode6 = 0;
    } else if (frame_counter_mode6 > 120) {
        frame_counter_mode6 = 0;
    }
}

void increment_frame_counter_mode6(void) {
    frame_counter_mode6++;
}

void set_anim_state_mode6(byte state) {
    anim_state_mode6 = state;
}

void set_frame_counter_mode6(byte frame) {
    frame_counter_mode6 = frame;
}

