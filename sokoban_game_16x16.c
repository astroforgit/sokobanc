/*
  sokoban_game_16x16.c - Sokoban game logic library for 16x16 tiles
  Implementation file
*/

#include "sokoban_game_16x16.h"
#include "atari_conio_16x16.h"
#include "atari_font_16x16.h"

// Game State & Data
static char level[TILE_ROWS][TILE_COLS];
static byte player_x, player_y;
static byte total_boxes = 0;
static byte boxes_on_goals = 0;
static byte moves = 0;

void load_level_16x16(const char** d, byte r_num) {
    byte r, c;
    char t;
    
    // Reset game state
    total_boxes = 0;
    boxes_on_goals = 0;
    moves = 0;
    
    // Clear the level array
    for (r = 0; r < TILE_ROWS; ++r) {
        for (c = 0; c < TILE_COLS; ++c) {
            level[r][c] = TILE_EMPTY;
        }
    }
    
    // Load level data from string array
    for (r = 0; r < r_num && r < TILE_ROWS; ++r) {
        c = 0;
        while (d[r][c] && c < TILE_COLS) {
            t = d[r][c];
            if (t == ' ') {
                level[r][c] = TILE_EMPTY;
            } else if (t == TILE_PLAYER) {
                player_x = c;
                player_y = r;
                level[r][c] = TILE_EMPTY;
            } else if (t == TILE_BOX) {
                total_boxes++;
                level[r][c] = t;
            } else {
                level[r][c] = t;
            }
            c++;
        }
    }
}

void draw_level_16x16(void) {
    byte r, c;
    char t;
    
    my_clrscr_16x16();
    
    // Draw all tiles
    for (r = 0; r < TILE_ROWS; ++r) {
        for (c = 0; c < TILE_COLS; ++c) {
            t = level[r][c];
            
            // Map game tiles to 16x16 character tiles
            if (t == TILE_WALL) {
                my_put_tile_16x16(c, r, TILE_WALL_TL);
            } else if (t == TILE_BOX) {
                my_put_tile_16x16(c, r, TILE_BOX_TL);
            } else if (t == TILE_GOAL) {
                my_put_tile_16x16(c, r, TILE_GOAL_TL);
            } else if (t == TILE_BOX_ON_GOAL) {
                my_put_tile_16x16(c, r, TILE_BOXGOAL_TL);
            }
            // Empty tiles are already cleared
        }
    }
    
    // Draw the player
    if (level[player_y][player_x] == TILE_GOAL) {
        my_put_tile_16x16(player_x, player_y, TILE_PLAYERGOAL_TL);
    } else {
        my_put_tile_16x16(player_x, player_y, TILE_PLAYER_TL);
    }
}

void update_status_16x16(void) {
    // Clear the status line first
    my_cputsxy_16x16(0, 23, "                                        ");
    // Print the status
    my_cprintf_status_16x16(boxes_on_goals, total_boxes, moves);
}

void try_move_player_16x16(signed char dx, signed char dy) {
    byte tx, ty, bx, by;
    char tt, bt;
    
    // Calculate target position
    tx = player_x + dx;
    ty = player_y + dy;
    
    // Bounds checking
    if (tx >= TILE_COLS || ty >= TILE_ROWS) {
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
        if (bx >= TILE_COLS || by >= TILE_ROWS) {
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
            my_put_tile_16x16(tx, ty, TILE_GOAL_TL);
        } else {
            level[ty][tx] = TILE_EMPTY;
            my_put_tile_16x16(tx, ty, 0);  // Empty tile
        }
        
        // Place box at new position
        if (bt == TILE_GOAL) {
            level[by][bx] = TILE_BOX_ON_GOAL;
            boxes_on_goals++;
            my_put_tile_16x16(bx, by, TILE_BOXGOAL_TL);
        } else {
            level[by][bx] = TILE_BOX;
            my_put_tile_16x16(bx, by, TILE_BOX_TL);
        }
    }
    
    // Clear player's old position
    if (level[player_y][player_x] == TILE_GOAL) {
        my_put_tile_16x16(player_x, player_y, TILE_GOAL_TL);
    } else {
        my_put_tile_16x16(player_x, player_y, 0);  // Empty tile
    }
    
    // Move player to new position
    player_x = tx;
    player_y = ty;
    
    // Draw player at new position
    if (level[player_y][player_x] == TILE_GOAL) {
        my_put_tile_16x16(player_x, player_y, TILE_PLAYERGOAL_TL);
    } else {
        my_put_tile_16x16(player_x, player_y, TILE_PLAYER_TL);
    }
    
    // Update move counter and status
    moves++;
    update_status_16x16();
    
    // Trigger the walking animation
    set_anim_state_16x16(1);
    set_frame_counter_16x16(0);
}

byte get_player_x_16x16(void) {
    return player_x;
}

byte get_player_y_16x16(void) {
    return player_y;
}

byte get_total_boxes_16x16(void) {
    return total_boxes;
}

byte get_boxes_on_goals_16x16(void) {
    return boxes_on_goals;
}

byte get_moves_16x16(void) {
    return moves;
}

byte is_level_complete_16x16(void) {
    return (boxes_on_goals == total_boxes);
}

