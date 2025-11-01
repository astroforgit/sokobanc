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
    word charset_addr = (word)CHARSET_MEM_MODE6;

    // IMPORTANT: ANTIC Mode 6 only uses 512 bytes for charset (64 chars × 8 bytes)
    // NOT 1024 bytes like text mode!
    // Copy ROM character set to RAM (only first 512 bytes needed for Mode 6)
    memcpy((void*)CHARSET_MEM_MODE6, (void*)0xE000, 512);

    // Install custom graphics at specific positions:
    // In Mode 6, character ranges map to colors:
    // 0-63: PF0 (brown), 64-127: PF2 (yellow), 128-191: PF1 (green), 192-255: PF3 (red)

    // Characters 1-2: Wall and Box (PF0 brown) - using chars 1-2 instead of 64-65
    memcpy((void*)(CHARSET_MEM_MODE6 + 1 * 8), custom_graphics_mode6, 16);

    // Character 3: Goal (PF0 brown for now, we'll use color offset later)
    memcpy((void*)(CHARSET_MEM_MODE6 + 3 * 8), custom_graphics_mode6 + 16, 8);

    // Character 4: Box on goal (PF0 brown for now)
    memcpy((void*)(CHARSET_MEM_MODE6 + 4 * 8), custom_graphics_mode6 + 24, 8);

    // Character 5-6: Player (PF0 brown for now)
    memcpy((void*)(CHARSET_MEM_MODE6 + 5 * 8), custom_graphics_mode6 + 32, 16);

    // Set character set pointer using POKE (location 756 = CHBAS)
    POKE(756, (byte)(charset_addr >> 8));

    // Install display list
    memcpy((void*)DLIST_MEM_MODE6, display_list_mode6, sizeof(display_list_mode6));

    // Point display list pointer to our custom display list
    POKEW(560, DLIST_MEM_MODE6);  // SDLSTL/SDLSTH at 560/561

    // Setup colors for ANTIC Mode 6
    // Color registers for Mode 6:
    POKE(712, MODE6_COLOR_BLACK);   // COLOR0 - Background
    POKE(708, MODE6_COLOR_BROWN);   // COLOR1 - PF0 (chars 0-63)
    POKE(709, MODE6_COLOR_GREEN);   // COLOR2 - PF1 (chars 128-191)
    POKE(710, MODE6_COLOR_YELLOW);  // COLOR3 - PF2 (chars 64-127)
    POKE(711, MODE6_COLOR_RED);     // COLOR4 - PF3 (chars 192-255)

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

