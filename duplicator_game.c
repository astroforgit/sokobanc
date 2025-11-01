/*
  duplicator_game.c - Game logic for Duplicator game
  Implementation file
*/

#include "duplicator_game.h"
#include "atari_conio.h"

// Game state
static GameState game_state;

// Level data storage
static char level_map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];

void load_level(const char* level_data[], byte num_rows) {
    byte x, y;
    const char* row;
    
    // Clear the level map
    memset(level_map, ' ', sizeof(level_map));
    
    // Reset game state
    game_state.player_x = 0;
    game_state.player_y = 0;
    game_state.level_width = 0;
    game_state.level_height = num_rows;
    game_state.moves = 0;
    game_state.level_complete = 0;
    
    // Load level data and find player position
    for (y = 0; y < num_rows; y++) {
        row = level_data[y];
        x = 0;
        while (row[x] != '\0' && x < MAX_LEVEL_WIDTH) {
            level_map[y][x] = row[x];
            
            // Find player starting position
            if (row[x] == TILE_PLAYER) {
                game_state.player_x = x;
                game_state.player_y = y;
            }
            
            x++;
        }
        
        // Track the maximum width
        if (x > game_state.level_width) {
            game_state.level_width = x;
        }
    }
}

void draw_level(void) {
    byte x, y;
    char tile;
    
    for (y = 0; y < game_state.level_height; y++) {
        for (x = 0; x < game_state.level_width; x++) {
            tile = level_map[y][x];
            
            // Draw the tile
            if (tile != ' ') {
                my_cputcxy(x, y + 2, tile);  // +2 to leave room for title
            } else {
                my_cputcxy(x, y + 2, ' ');
            }
        }
    }
}

byte get_tile(byte x, byte y) {
    if (x >= MAX_LEVEL_WIDTH || y >= MAX_LEVEL_HEIGHT) {
        return TILE_WALL;  // Out of bounds = wall
    }
    return level_map[y][x];
}

void set_tile(byte x, byte y, byte tile) {
    if (x < MAX_LEVEL_WIDTH && y < MAX_LEVEL_HEIGHT) {
        level_map[y][x] = tile;
    }
}

byte try_move_player(char dx, char dy) {
    byte new_x = game_state.player_x + dx;
    byte new_y = game_state.player_y + dy;
    byte target_tile;
    
    // Check bounds
    if (new_x >= MAX_LEVEL_WIDTH || new_y >= MAX_LEVEL_HEIGHT) {
        return 0;  // Can't move out of bounds
    }
    
    target_tile = get_tile(new_x, new_y);
    
    // Check if target is a wall
    if (target_tile == TILE_WALL) {
        return 0;  // Can't move into wall
    }
    
    // Check if target is passable
    if (target_tile == TILE_EMPTY || 
        target_tile == TILE_FLOOR ||
        target_tile == TILE_EXIT_A ||
        target_tile == TILE_EXIT_B ||
        target_tile == TILE_HOLE_A ||
        target_tile == TILE_HOLE_B ||
        target_tile == TILE_PLATE_A ||
        target_tile == TILE_PLATE_B) {
        
        // Clear old player position
        set_tile(game_state.player_x, game_state.player_y, TILE_FLOOR);
        
        // Move player to new position
        game_state.player_x = new_x;
        game_state.player_y = new_y;
        
        // Check if player reached exit
        if (target_tile == TILE_EXIT_A || target_tile == TILE_EXIT_B) {
            game_state.level_complete = 1;
        }
        
        // Set new player position
        set_tile(new_x, new_y, TILE_PLAYER);
        
        // Increment move counter
        game_state.moves++;
        
        // Redraw the affected tiles
        my_cputcxy(game_state.player_x - dx, game_state.player_y - dy + 2, TILE_FLOOR);
        my_cputcxy(game_state.player_x, game_state.player_y + 2, TILE_PLAYER);
        
        return 1;  // Move successful
    }
    
    return 0;  // Can't move to this tile
}

byte is_level_complete(void) {
    return game_state.level_complete;
}

GameState* get_game_state(void) {
    return &game_state;
}

