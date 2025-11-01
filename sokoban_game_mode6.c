/*
  sokoban_game_mode6.c - Sokoban game logic library for ANTIC Mode 6
  Implementation file
*/

#include "sokoban_game_mode6.h"
#include "atari_conio_mode6.h"
#include "atari_font_mode6.h"

// Game State & Data
static char level[CHAR_ROWS_MODE6][CHAR_COLS_MODE6];
static byte player_x, player_y;
static byte total_boxes = 0;
static byte boxes_on_goals = 0;
static byte moves = 0;

void load_level_mode6(const char** d, byte r_num) {
    byte r, c;
    char t;

    // Reset game state
    total_boxes = 0;
    boxes_on_goals = 0;
    moves = 0;

    // Clear the level array
    for (r = 0; r < CHAR_ROWS_MODE6; ++r) {
        for (c = 0; c < CHAR_COLS_MODE6; ++c) {
            level[r][c] = TILE_EMPTY;
        }
    }

    // Load level data from string array
    for (r = 0; r < r_num && r < CHAR_ROWS_MODE6; ++r) {
        c = 0;
        while (d[r][c] && c < CHAR_COLS_MODE6) {
            t = d[r][c];
            if (t == ' ') {
                level[r][c] = TILE_EMPTY;
            } else if (t == TILE_PLAYER) {
                player_x = c;
                player_y = r;
                level[r][c] = TILE_EMPTY;
            } else if (t == TILE_PLAYER_ON_GOAL) {
                player_x = c;
                player_y = r;
                level[r][c] = TILE_GOAL;
            } else if (t == TILE_BOX) {
                total_boxes++;
                level[r][c] = t;
            } else if (t == TILE_BOX_ON_GOAL) {
                total_boxes++;
                boxes_on_goals++;
                level[r][c] = t;
            } else {
                level[r][c] = t;
            }
            c++;
        }
    }
}

void draw_level_mode6(void) {
    byte r, c;
    char t;
    byte screen_char;

    my_clrscr_mode6();

    // Draw all tiles with appropriate colors
    for (r = 0; r < CHAR_ROWS_MODE6; ++r) {
        for (c = 0; c < CHAR_COLS_MODE6; ++c) {
            t = level[r][c];
            
            // Map game tiles to colored characters
            // PF0 (brown) for walls and boxes - characters 0-127
            // PF1 (green) for player - characters 128-255
            // Use custom characters for better graphics
            
            if (t == TILE_WALL) {
                // Wall: brown color (PF0)
                screen_char = CHAR_WALL_CUSTOM;
                my_cputcxy_mode6(c, r, screen_char);
            } else if (t == TILE_BOX) {
                // Box: brown color (PF0)
                screen_char = CHAR_BOX_CUSTOM;
                my_cputcxy_mode6(c, r, screen_char);
            } else if (t == TILE_GOAL) {
                // Goal: yellow color - use custom char with offset
                // We'll use a special character that shows in PF2 color
                screen_char = CHAR_GOAL_CUSTOM;
                my_cputcxy_mode6(c, r, screen_char);
            } else if (t == TILE_BOX_ON_GOAL) {
                // Box on goal: red color - use custom char
                screen_char = CHAR_BOXGOAL_CUSTOM;
                my_cputcxy_mode6(c, r, screen_char);
            }
            // Empty tiles are already cleared
        }
    }
    
    // Draw the player (green color - PF1, characters 128-255)
    if (level[player_y][player_x] == TILE_GOAL) {
        my_cputcxy_mode6(player_x, player_y, CHAR_PLAYERGOAL_CUSTOM);
    } else {
        my_cputcxy_mode6(player_x, player_y, CHAR_PLAYER_CUSTOM);
    }
}

void update_status_mode6(void) {
    my_cprintf_status_mode6(boxes_on_goals, total_boxes, moves);
}

void try_move_player_mode6(signed char dx, signed char dy) {
    byte tx, ty, bx, by;
    char tt, bt;

    // Calculate target position
    tx = player_x + dx;
    ty = player_y + dy;

    // Bounds checking
    if (tx >= CHAR_COLS_MODE6 || ty >= CHAR_ROWS_MODE6) {
        return;
    }
    
    tt = level[ty][tx];
    
    // Check if target is a wall
    if (tt == TILE_WALL) {
        return;
    }
    
    // Check if target is a box
    if (tt == TILE_BOX || tt == TILE_BOX_ON_GOAL) {
        // Calculate box's target position
        bx = tx + dx;
        by = ty + dy;

        // Bounds checking for box
        if (bx >= CHAR_COLS_MODE6 || by >= CHAR_ROWS_MODE6) {
            return;
        }
        
        bt = level[by][bx];
        
        // Check if box can be pushed
        if (bt == TILE_WALL || bt == TILE_BOX || bt == TILE_BOX_ON_GOAL) {
            return; // Can't push box into wall or another box
        }
        
        // Remove box from current position
        if (tt == TILE_BOX_ON_GOAL) {
            level[ty][tx] = TILE_GOAL;
            boxes_on_goals--;
            my_cputcxy_mode6(tx, ty, CHAR_GOAL_CUSTOM);
        } else {
            level[ty][tx] = TILE_EMPTY;
            my_cputcxy_mode6(tx, ty, 0);  // Empty
        }
        
        // Place box at new position
        if (bt == TILE_GOAL) {
            level[by][bx] = TILE_BOX_ON_GOAL;
            boxes_on_goals++;
            my_cputcxy_mode6(bx, by, CHAR_BOXGOAL_CUSTOM);
        } else {
            level[by][bx] = TILE_BOX;
            my_cputcxy_mode6(bx, by, CHAR_BOX_CUSTOM);
        }
    }
    
    // Clear player's old position
    if (level[player_y][player_x] == TILE_GOAL) {
        my_cputcxy_mode6(player_x, player_y, CHAR_GOAL_CUSTOM);
    } else {
        my_cputcxy_mode6(player_x, player_y, 0);  // Empty
    }
    
    // Move player to new position
    player_x = tx;
    player_y = ty;
    
    // Draw player at new position (green color - PF1)
    if (level[player_y][player_x] == TILE_GOAL) {
        my_cputcxy_mode6(player_x, player_y, CHAR_PLAYERGOAL_CUSTOM);
    } else {
        my_cputcxy_mode6(player_x, player_y, CHAR_PLAYER_CUSTOM);
    }
    
    // Update move counter and status
    moves++;
    update_status_mode6();
    
    // Trigger the walking animation
    set_anim_state_mode6(1);
    set_frame_counter_mode6(0);
}

byte get_player_x_mode6(void) {
    return player_x;
}

byte get_player_y_mode6(void) {
    return player_y;
}

byte get_total_boxes_mode6(void) {
    return total_boxes;
}

byte get_boxes_on_goals_mode6(void) {
    return boxes_on_goals;
}

byte get_moves_mode6(void) {
    return moves;
}

byte is_level_complete_mode6(void) {
    return (boxes_on_goals == total_boxes);
}

