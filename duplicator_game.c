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

// Simple queue for flood fill (max 256 positions)
typedef struct {
    byte x;
    byte y;
} Position;

static Position flood_queue[256];
static byte queue_start;
static byte queue_end;

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
            
            // Find player starting position (can be 'p' or 'P')
            if (row[x] == 'p' || row[x] == 'P') {
                game_state.player_x = x;
                game_state.player_y = y;
                // Normalize to lowercase 'p'
                level_map[y][x] = TILE_PLAYER;
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

byte is_blocking(char tile) {
    // Walls and wall lines
    if (tile == '#' || tile == '$' || tile == '%' || tile == '&') {
        return 1;
    }

    // Closed doors and gates
    if (tile == 'd' || tile == 'g' || tile == 'h') {
        return 1;
    }

    // Pushable objects (blocking unless pushed)
    if (tile == '*' || tile == 'k' || tile == 'e') {
        return 1;
    }

    return 0;
}

byte is_passable(char tile) {
    // Floor and empty
    if (tile == '.' || tile == ' ') {
        return 1;
    }

    // Holes (both empty and filled)
    if (tile == '?' || tile == '!' || tile == '[' || tile == ']') {
        return 1;
    }

    // Exits
    if (tile == '@' || tile == ':' || tile == ';' || tile == '<') {
        return 1;
    }

    // Plates
    if (tile == 'b' || tile == 'c') {
        return 1;
    }

    // Decorative lines (1-8)
    if (tile >= '1' && tile <= '8') {
        return 1;
    }

    // Open gates (uppercase G, H)
    if (tile == 'G' || tile == 'H') {
        return 1;
    }

    // Open door
    if (tile == 'D') {
        return 1;
    }

    return 0;
}

byte is_exit(char tile) {
    return (tile == '@' || tile == ':' || tile == ';' || tile == '<');
}

byte is_pushable(char tile) {
    return (tile == '*' || tile == 'k' || tile == 'e');
}

void door_flood_fill(byte x, byte y) {
    byte current_x, current_y;
    char tile;

    // Initialize queue
    queue_start = 0;
    queue_end = 0;

    // Add starting position to queue
    flood_queue[queue_end].x = x;
    flood_queue[queue_end].y = y;
    queue_end++;

    // Mark starting door as open
    set_tile(x, y, TILE_DOOR_OPEN);

    // Process queue (BFS flood fill)
    while (queue_start != queue_end) {
        // Dequeue position
        current_x = flood_queue[queue_start].x;
        current_y = flood_queue[queue_start].y;
        queue_start++;

        // Check all 4 neighbors (up, down, left, right)
        // Up
        if (current_y > 0) {
            tile = get_tile(current_x, current_y - 1);
            if (tile == TILE_DOOR) {
                set_tile(current_x, current_y - 1, TILE_DOOR_OPEN);
                flood_queue[queue_end].x = current_x;
                flood_queue[queue_end].y = current_y - 1;
                queue_end++;
            }
        }

        // Down
        if (current_y < MAX_LEVEL_HEIGHT - 1) {
            tile = get_tile(current_x, current_y + 1);
            if (tile == TILE_DOOR) {
                set_tile(current_x, current_y + 1, TILE_DOOR_OPEN);
                flood_queue[queue_end].x = current_x;
                flood_queue[queue_end].y = current_y + 1;
                queue_end++;
            }
        }

        // Left
        if (current_x > 0) {
            tile = get_tile(current_x - 1, current_y);
            if (tile == TILE_DOOR) {
                set_tile(current_x - 1, current_y, TILE_DOOR_OPEN);
                flood_queue[queue_end].x = current_x - 1;
                flood_queue[queue_end].y = current_y;
                queue_end++;
            }
        }

        // Right
        if (current_x < MAX_LEVEL_WIDTH - 1) {
            tile = get_tile(current_x + 1, current_y);
            if (tile == TILE_DOOR) {
                set_tile(current_x + 1, current_y, TILE_DOOR_OPEN);
                flood_queue[queue_end].x = current_x + 1;
                flood_queue[queue_end].y = current_y;
                queue_end++;
            }
        }
    }
}

void remove_open_doors(void) {
    byte x, y;

    // Scan entire level and remove all door_open tiles
    for (y = 0; y < game_state.level_height; y++) {
        for (x = 0; x < game_state.level_width; x++) {
            if (get_tile(x, y) == TILE_DOOR_OPEN) {
                set_tile(x, y, TILE_FLOOR);
                // Redraw the tile
                my_cputcxy(x, y + 2, TILE_FLOOR);
            }
        }
    }
}

void handle_key_door(byte key_x, byte key_y, byte door_x, byte door_y) {
    // Remove key
    set_tile(key_x, key_y, TILE_FLOOR);
    my_cputcxy(key_x, key_y + 2, TILE_FLOOR);

    // Start flood fill from the door
    door_flood_fill(door_x, door_y);

    // Remove all open doors
    remove_open_doors();
}

byte try_push(byte x, byte y, char dx, char dy) {
    char object_tile;
    char behind_tile;
    byte behind_x, behind_y;

    // Get the object we're trying to push
    object_tile = get_tile(x, y);

    // Check if it's actually pushable
    if (!is_pushable(object_tile)) {
        return 0;
    }

    // Calculate position behind the object
    behind_x = x + dx;
    behind_y = y + dy;

    // Check bounds
    if (behind_x >= MAX_LEVEL_WIDTH || behind_y >= MAX_LEVEL_HEIGHT) {
        return 0;  // Can't push out of bounds
    }

    // Get what's behind the object
    behind_tile = get_tile(behind_x, behind_y);

    // Special case: Key pushing into door
    if (object_tile == TILE_KEY && behind_tile == TILE_DOOR) {
        // Handle key-door interaction
        handle_key_door(x, y, behind_x, behind_y);
        return 1;  // Push successful (key and doors removed)
    }

    // Check if space behind is passable (empty or floor-like)
    if (!is_passable(behind_tile)) {
        return 0;  // Can't push into blocking tile
    }

    // Perform the push
    // Clear current position (restore floor)
    set_tile(x, y, TILE_FLOOR);

    // Move object to new position
    set_tile(behind_x, behind_y, object_tile);

    // Redraw the affected tiles
    my_cputcxy(x, y + 2, TILE_FLOOR);
    my_cputcxy(behind_x, behind_y + 2, object_tile);

    return 1;  // Push successful
}

byte try_move_player(char dx, char dy) {
    byte new_x = game_state.player_x + dx;
    byte new_y = game_state.player_y + dy;
    char target_tile;
    char old_tile;

    // Check bounds
    if (new_x >= MAX_LEVEL_WIDTH || new_y >= MAX_LEVEL_HEIGHT) {
        return 0;  // Can't move out of bounds
    }

    target_tile = get_tile(new_x, new_y);

    // Check if target is passable
    if (is_passable(target_tile)) {
        // Remember what was at old position (might be on a plate, hole, etc.)
        old_tile = get_tile(game_state.player_x, game_state.player_y);

        // Clear old player position (restore floor or keep special tile)
        if (old_tile == TILE_PLAYER) {
            set_tile(game_state.player_x, game_state.player_y, TILE_FLOOR);
        }

        // Move player to new position
        game_state.player_x = new_x;
        game_state.player_y = new_y;

        // Check if player reached exit
        if (is_exit(target_tile)) {
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

    // Check if target is pushable
    if (is_pushable(target_tile)) {
        // Try to push the object
        if (try_push(new_x, new_y, dx, dy)) {
            // Push successful, now move player into the vacated space
            old_tile = get_tile(game_state.player_x, game_state.player_y);

            // Clear old player position
            if (old_tile == TILE_PLAYER) {
                set_tile(game_state.player_x, game_state.player_y, TILE_FLOOR);
            }

            // Move player to where the object was
            game_state.player_x = new_x;
            game_state.player_y = new_y;
            set_tile(new_x, new_y, TILE_PLAYER);

            // Increment move counter
            game_state.moves++;

            // Redraw the affected tiles
            my_cputcxy(game_state.player_x - dx, game_state.player_y - dy + 2, TILE_FLOOR);
            my_cputcxy(game_state.player_x, game_state.player_y + 2, TILE_PLAYER);

            return 1;  // Move and push successful
        } else {
            return 0;  // Can't push (blocked)
        }
    }

    // Check if target is blocking (walls, doors, gates)
    if (is_blocking(target_tile)) {
        return 0;  // Can't move into blocking tile
    }

    return 0;  // Can't move to this tile
}

byte is_level_complete(void) {
    return game_state.level_complete;
}

GameState* get_game_state(void) {
    return &game_state;
}

