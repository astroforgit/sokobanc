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

// Custom character set data (optional - can use ROM charset)
// In Mode 6, characters are 8x8 pixels, 1 bit per pixel (2 colors per char)
static unsigned char custom_graphics_mode6[] = {
    // Custom wall character (0x40) - Brick pattern
    0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0x3C, 0x3C,
    
    // Custom box character (0x41) - Crate
    0xFF, 0x81, 0xBD, 0xBD, 0xBD, 0xBD, 0x81, 0xFF,
    
    // Custom goal character (0x42) - Circle
    0x00, 0x3C, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x00,
    
    // Custom box on goal (0x43) - Box + circle
    0xFF, 0xC3, 0xBD, 0xBD, 0xBD, 0xBD, 0xC3, 0xFF,
    
    // Custom player character (0x40 in upper 128) - Stick figure
    0x18, 0x3C, 0x18, 0x7E, 0x18, 0x24, 0x42, 0x81,
    
    // Custom player on goal (0x41 in upper 128)
    0x18, 0x3C, 0x18, 0x7E, 0x18, 0x24, 0x42, 0x81
};

void setup_colors_mode6(void) {
    // Set color registers using OS shadow registers
    OS.color0 = MODE6_COLOR_BLACK;      // Background: Black
    OS.color1 = MODE6_COLOR_BROWN;      // PF0: Brown (walls, boxes)
    OS.color2 = MODE6_COLOR_GREEN;      // PF1: Green (player)
    OS.color3 = MODE6_COLOR_YELLOW;     // PF2: Yellow (goals)
    OS.color4 = MODE6_COLOR_RED;        // PF3: Red (boxes on goals)
}

void setup_graphics_mode6(void) {
    // Install display list
    memcpy((void*)DLIST_MEM_MODE6, display_list_mode6, sizeof(display_list_mode6));

    // Point display list pointer to our custom display list
    OS.sdlstl = (unsigned char)(DLIST_MEM_MODE6 & 0xFF);
    OS.sdlsth = (unsigned char)(DLIST_MEM_MODE6 >> 8);

    // Setup colors
    setup_colors_mode6();

    // Optional: Install custom character set
    // Copy ROM character set first
    memcpy((void*)CHARSET_MEM_MODE6, (void*)0xE000, 1024);

    // Install custom graphics for characters 0x40-0x43
    memcpy((void*)(CHARSET_MEM_MODE6 + 0x40 * 8), custom_graphics_mode6, 4 * 8);

    // Install custom graphics for characters 0xC0-0xC1 (128 + 0x40, 128 + 0x41)
    // In Mode 6, upper 128 characters use same shapes but different color
    memcpy((void*)(CHARSET_MEM_MODE6 + 0xC0 * 8), custom_graphics_mode6 + 4 * 8, 2 * 8);

    // Point character base to our custom character set
    OS.chbas = (unsigned char)(CHARSET_MEM_MODE6 >> 8);

    // Clear screen
    memset((void*)SCREEN_MEM_MODE6, 0, CHAR_COLS_MODE6 * CHAR_ROWS_MODE6);
}

void animate_player_mode6(void) {
    // Simple animation: just update based on state
    // In Mode 6, we can't easily modify character graphics on the fly
    // So animation is limited or requires character swapping
    
    // For now, just track animation state
    if (anim_state_mode6 == 1) {
        // Walking animation - could swap character codes
        if (frame_counter_mode6 > 30) {
            anim_state_mode6 = 0;
        }
    } else if (frame_counter_mode6 > 120) {
        // Blinking animation every 2 seconds
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

