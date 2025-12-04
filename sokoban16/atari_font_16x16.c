/*
  atari_font_16x16.c - 16x16 tile font and graphics library
  Implementation file
*/

#include "atari_font_16x16.h"

// Animation state variables
static byte frame_counter = 0;
static byte anim_state = 0;  // 0 = idle, 1 = walking

// 16x16 Graphics Data
// Each tile is 16x16 pixels = 4 characters of 8x8 pixels each
// Format: [Top-Left 8 bytes][Top-Right 8 bytes][Bottom-Left 8 bytes][Bottom-Right 8 bytes]

static unsigned char custom_graphics_16x16[] = {
    // WALL TILE (0x40-0x43) - Brick pattern
    // Top-Left
    0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,
    // Top-Right
    0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,
    // Bottom-Left
    0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,
    // Bottom-Right
    0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,
    
    // BOX TILE (0x44-0x47) - Wooden crate with cross pattern
    // Top-Left
    0xFF,0x81,0x81,0xBD,0xBD,0xA5,0xA5,0xBD,
    // Top-Right
    0xFF,0x81,0x81,0xBD,0xBD,0xA5,0xA5,0xBD,
    // Bottom-Left
    0xBD,0xA5,0xA5,0xBD,0xBD,0x81,0x81,0xFF,
    // Bottom-Right
    0xBD,0xA5,0xA5,0xBD,0xBD,0x81,0x81,0xFF,
    
    // GOAL TILE (0x48-0x4B) - Target circle
    // Top-Left
    0x00,0x00,0x00,0x07,0x0F,0x1F,0x1F,0x3F,
    // Top-Right
    0x00,0x00,0x00,0xE0,0xF0,0xF8,0xF8,0xFC,
    // Bottom-Left
    0x3F,0x1F,0x1F,0x0F,0x07,0x00,0x00,0x00,
    // Bottom-Right
    0xFC,0xF8,0xF8,0xF0,0xE0,0x00,0x00,0x00,
    
    // BOX ON GOAL TILE (0x4C-0x4F) - Box with target visible
    // Top-Left
    0xFF,0x81,0x87,0xBF,0xBF,0xAF,0xAF,0xBF,
    // Top-Right
    0xFF,0x81,0xE1,0xFD,0xFD,0xF5,0xF5,0xFD,
    // Bottom-Left
    0xBF,0xAF,0xAF,0xBF,0xBF,0x87,0x81,0xFF,
    // Bottom-Right
    0xFD,0xF5,0xF5,0xFD,0xFD,0xE1,0x81,0xFF,
    
    // PLAYER TILE - Standing (0x50-0x53)
    // Top-Left
    0x00,0x00,0x00,0x03,0x07,0x07,0x0F,0x0F,
    // Top-Right
    0x00,0x00,0x00,0xC0,0xE0,0xE0,0xF0,0xF0,
    // Bottom-Left
    0x0F,0x07,0x07,0x03,0x01,0x03,0x06,0x0C,
    // Bottom-Right
    0xF0,0xE0,0xE0,0xC0,0x80,0xC0,0x60,0x30,
    
    // PLAYER ON GOAL - Standing (0x54-0x57)
    // Top-Left
    0x00,0x00,0x00,0x03,0x07,0x07,0x0F,0x0F,
    // Top-Right
    0x00,0x00,0x00,0xC0,0xE0,0xE0,0xF0,0xF0,
    // Bottom-Left
    0x0F,0x07,0x07,0x03,0x01,0x03,0x06,0x0C,
    // Bottom-Right
    0xF0,0xE0,0xE0,0xC0,0x80,0xC0,0x60,0x30,
};

// Animation frames for player (walking and blinking)
static unsigned char player_walking_16x16[] = {
    // PLAYER WALKING - Left foot forward
    // Top-Left
    0x00,0x00,0x00,0x03,0x07,0x07,0x0F,0x0F,
    // Top-Right
    0x00,0x00,0x00,0xC0,0xE0,0xE0,0xF0,0xF0,
    // Bottom-Left
    0x0F,0x07,0x07,0x03,0x01,0x06,0x0C,0x18,
    // Bottom-Right
    0xF0,0xE0,0xE0,0xC0,0x80,0x60,0x30,0x18,
};

static unsigned char player_blinking_16x16[] = {
    // PLAYER BLINKING - Eyes closed
    // Top-Left
    0x00,0x00,0x00,0x03,0x07,0x07,0x0F,0x0F,
    // Top-Right
    0x00,0x00,0x00,0xC0,0xE0,0xE0,0xF0,0xF0,
    // Bottom-Left
    0x0F,0x07,0x07,0x03,0x00,0x03,0x06,0x0C,
    // Bottom-Right
    0xF0,0xE0,0xE0,0xC0,0x00,0xC0,0x60,0x30,
};

void setup_graphics_16x16(void) {
    byte i;
    word screen_addr = (word)0x9000;  // Screen memory
    word dlist_addr = (word)DLIST_MEM;
    word charset_addr = (word)CHARSET_MEM;
    
    // Create display list
    DLIST_MEM[0] = 0x70;  // 8 blank lines
    DLIST_MEM[1] = 0x70;  // 8 blank lines
    DLIST_MEM[2] = 0x70;  // 8 blank lines
    
    // LMS (Load Memory Scan) - point to screen memory
    DLIST_MEM[3] = 0x42;  // Mode 2 with LMS
    DLIST_MEM[4] = (byte)screen_addr;
    DLIST_MEM[5] = (byte)(screen_addr >> 8);
    
    // 23 more mode 2 lines
    for (i = 0; i < 23; ++i) {
        DLIST_MEM[6 + i] = 0x02;
    }
    
    // Jump back to start of display list
    DLIST_MEM[29] = 0x41;
    DLIST_MEM[30] = (byte)dlist_addr;
    DLIST_MEM[31] = (byte)(dlist_addr >> 8);
    
    // Point to custom character set
    POKE(756, (byte)(charset_addr >> 8));
    
    // Copy ROM character set to RAM
    memcpy(CHARSET_MEM, (void*)ROM_CHARSET_ADDRESS, 1024);
    
    // Install 16x16 tile graphics
    // Wall (0x40-0x43)
    memcpy(CHARSET_MEM + (TILE_WALL_TL * 8), &custom_graphics_16x16[0], 32);
    
    // Box (0x44-0x47)
    memcpy(CHARSET_MEM + (TILE_BOX_TL * 8), &custom_graphics_16x16[32], 32);
    
    // Goal (0x48-0x4B)
    memcpy(CHARSET_MEM + (TILE_GOAL_TL * 8), &custom_graphics_16x16[64], 32);
    
    // Box on Goal (0x4C-0x4F)
    memcpy(CHARSET_MEM + (TILE_BOXGOAL_TL * 8), &custom_graphics_16x16[96], 32);
    
    // Player Standing (0x50-0x53)
    memcpy(CHARSET_MEM + (TILE_PLAYER_TL * 8), &custom_graphics_16x16[128], 32);
    
    // Player on Goal Standing (0x54-0x57)
    memcpy(CHARSET_MEM + (TILE_PLAYERGOAL_TL * 8), &custom_graphics_16x16[160], 32);
    
    // Set colors
    POKE(709, 0);   // Background
    POKE(710, 14);  // Foreground
    
    // Activate display list
    POKEW(560, dlist_addr);
    
    // Turn off cursor
    POKE(752, 0);
}

void animate_player_16x16(void) {
    // Get pointers to player character locations in the character set
    byte* player_char_data = CHARSET_MEM + (TILE_PLAYER_TL * 8);
    byte* player_goal_char_data = CHARSET_MEM + (TILE_PLAYERGOAL_TL * 8);
    
    if (anim_state == 1) {  // Walking animation
        if (frame_counter < 4) {
            // Standing frame
            memcpy(player_char_data, &custom_graphics_16x16[128], 32);
            memcpy(player_goal_char_data, &custom_graphics_16x16[160], 32);
        } else {
            // Walking frame
            memcpy(player_char_data, player_walking_16x16, 32);
            memcpy(player_goal_char_data, player_walking_16x16, 32);
        }
        if (frame_counter > 8) {
            anim_state = 0;  // Stop walking animation
        }
    } else {  // Idle animation (blinking)
        // Default to standing frame
        memcpy(player_char_data, &custom_graphics_16x16[128], 32);
        memcpy(player_goal_char_data, &custom_graphics_16x16[160], 32);
        
        // Blink occasionally
        if (frame_counter > 50 && frame_counter < 53) {
            memcpy(player_char_data, player_blinking_16x16, 32);
            memcpy(player_goal_char_data, player_blinking_16x16, 32);
        }
    }
}

byte get_anim_state_16x16(void) {
    return anim_state;
}

void set_anim_state_16x16(byte state) {
    anim_state = state;
}

byte get_frame_counter_16x16(void) {
    return frame_counter;
}

void set_frame_counter_16x16(byte counter) {
    frame_counter = counter;
}

void increment_frame_counter_16x16(void) {
    frame_counter++;
}

