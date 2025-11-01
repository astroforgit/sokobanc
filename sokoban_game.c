/*
  sokoban_game.c - Sokoban game logic library
  Implementation file
*/

#include "sokoban_game.h"
#include "atari_conio.h"
#include "atari_font.h"

// Game State & Data
static char level[ROWS][COLS];
static byte player_x, player_y;
static byte total_boxes = 0;
static byte boxes_on_goals = 0;
static byte moves = 0;

void load_level(const char** d, byte r_num) {
    byte r, c;
    char t;
    
    // Reset game state
    total_boxes = 0;
    boxes_on_goals = 0;
    moves = 0;
    
    // Clear the level array
    for (r = 0; r < ROWS; ++r) {
        for (c = 0; c < COLS; ++c) {
            level[r][c] = TILE_EMPTY;
        }
    }
    
    // Load level data from string array
    for (r = 0; r < r_num; ++r) {
        c = 0;
        while (d[r][c]) {
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

void draw_level(void) {
    byte r, c;
    char t, put;
    
    my_clrscr();
    
    // Draw all tiles
    for (r = 0; r < ROWS; ++r) {
        for (c = 0; c < COLS; ++c) {
            t = level[r][c];
            
            // Draw goal markers under boxes and player
            if (t == TILE_BOX_ON_GOAL || t == TILE_PLAYER_ON_GOAL) {
                my_cputcxy(c, r, TILE_GOAL);
            }
            
            // Draw the tile itself
            if (t != TILE_EMPTY) {
                my_cputcxy(c, r, t);
            }
        }
    }
    
    // Draw the player
    put = level[player_y][player_x];
    if (put == TILE_GOAL) {
        my_cputcxy(player_x, player_y, TILE_PLAYER_ON_GOAL);
    } else {
        my_cputcxy(player_x, player_y, TILE_PLAYER);
    }
}

void update_status(void) {
    // Clear the status line first
    my_cputsxy(0, 23, "                                        ");
    // Print the status
    my_cprintf_status(boxes_on_goals, total_boxes, moves);
}

void try_move_player(signed char dx, signed char dy) {
    byte tx, ty, bx, by;
    char tt, bt, put;
    
    // Calculate target position
    tx = player_x + dx;
    ty = player_y + dy;
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
        bt = level[by][bx];
        
        // Check if box can be pushed
        if (bt == TILE_WALL || bt == TILE_BOX || bt == TILE_BOX_ON_GOAL) {
            return; // Can't push box into wall or another box
        }
        
        // Remove box from current position
        if (tt == TILE_BOX_ON_GOAL) {
            level[ty][tx] = TILE_GOAL;
            boxes_on_goals--;
        } else {
            level[ty][tx] = TILE_EMPTY;
        }
        my_cputcxy(tx, ty, level[ty][tx]);
        
        // Place box at new position
        if (bt == TILE_GOAL) {
            level[by][bx] = TILE_BOX_ON_GOAL;
            boxes_on_goals++;
        } else {
            level[by][bx] = TILE_BOX;
        }
        my_cputcxy(bx, by, level[by][bx]);
    }
    
    // Clear player's old position
    my_cputcxy(player_x, player_y, level[player_y][player_x]);
    
    // Move player to new position
    player_x = tx;
    player_y = ty;
    
    // Draw player at new position
    put = level[player_y][player_x];
    if (put == TILE_GOAL) {
        my_cputcxy(player_x, player_y, TILE_PLAYER_ON_GOAL);
    } else {
        my_cputcxy(player_x, player_y, TILE_PLAYER);
    }
    
    // Update move counter and status
    moves++;
    update_status();
    
    // Trigger the walking animation
    set_anim_state(1);
    set_frame_counter(0);
}

byte get_player_x(void) {
    return player_x;
}

byte get_player_y(void) {
    return player_y;
}

byte get_total_boxes(void) {
    return total_boxes;
}

byte get_boxes_on_goals(void) {
    return boxes_on_goals;
}

byte get_moves(void) {
    return moves;
}

byte is_level_complete(void) {
    return (boxes_on_goals == total_boxes);
}

