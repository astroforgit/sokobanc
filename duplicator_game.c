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

// Simple queue for flood fill (reduced size to save memory)
typedef struct {
    byte x;
    byte y;
} Position;

static Position flood_queue[64];  // Reduced from 256 to 64
static byte queue_start;
static byte queue_end;

void load_level(const char* level_data[], byte num_rows) {
    byte x, y;
    const char* row;
    
    // Clear the level map
    memset(level_map, ' ', sizeof(level_map));
    
    // Reset game state
    game_state.num_players = 0;
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
                if (game_state.num_players < MAX_PLAYERS) {
                    game_state.players[game_state.num_players].x = x;
                    game_state.players[game_state.num_players].y = y;
                    game_state.players[game_state.num_players].under_tile = TILE_FLOOR;
                    game_state.num_players++;
                }
                // Normalize to lowercase 'p'
                level_map[y][x] = TILE_PLAYER;
            }
            // Check for 'z' = player on holeA
            else if (row[x] == 'z') {
                if (game_state.num_players < MAX_PLAYERS) {
                    game_state.players[game_state.num_players].x = x;
                    game_state.players[game_state.num_players].y = y;
                    game_state.players[game_state.num_players].under_tile = TILE_HOLE_A;
                    game_state.num_players++;
                }
                level_map[y][x] = TILE_PLAYER;  // Display player, remember hole underneath
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
    if (tile == '@' || tile == ':' || tile == ';') {
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
    return (tile == '@' || tile == ':' || tile == ';');
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

void update_gates(void) {
    byte x, y, i;
    char tile;
    byte plateA_has_object = 0;
    byte plateB_has_object = 0;

    // Check if any player is on a plate
    for (i = 0; i < game_state.num_players; i++) {
        if (game_state.players[i].under_tile == TILE_PLATE_A) {
            plateA_has_object = 1;
        } else if (game_state.players[i].under_tile == TILE_PLATE_B) {
            plateB_has_object = 1;
        }
    }

    // TODO: Check if crate, key, or enemy is on a plate
    // (will implement when we add object tracking)

    // Update gates based on plate states
    for (y = 0; y < game_state.level_height; y++) {
        for (x = 0; x < game_state.level_width; x++) {
            tile = get_tile(x, y);

            // Update gateA
            if (tile == TILE_GATE_A || tile == 'G') {
                if (plateA_has_object) {
                    // Open gate
                    if (tile != 'G') {
                        set_tile(x, y, 'G');
                        my_cputcxy(x, y + 2, 'G');
                    }
                } else {
                    // Close gate
                    if (tile != TILE_GATE_A) {
                        set_tile(x, y, TILE_GATE_A);
                        my_cputcxy(x, y + 2, TILE_GATE_A);
                    }
                }
            }

            // Update gateB
            if (tile == TILE_GATE_B || tile == 'H') {
                if (plateB_has_object) {
                    // Open gate
                    if (tile != 'H') {
                        set_tile(x, y, 'H');
                        my_cputcxy(x, y + 2, 'H');
                    }
                } else {
                    // Close gate
                    if (tile != TILE_GATE_B) {
                        set_tile(x, y, TILE_GATE_B);
                        my_cputcxy(x, y + 2, TILE_GATE_B);
                    }
                }
            }
        }
    }
}

void handle_duplication(void) {
    byte x, y, i;
    char tile;
    byte holeA_x = 0, holeA_y = 0, holeA_found = 0;
    byte holeB_x = 0, holeB_y = 0, holeB_found = 0;
    byte holeA_has_player = 0;
    byte holeB_has_player = 0;
    byte should_duplicate = 0;

    // Find holes in the level
    for (y = 0; y < game_state.level_height; y++) {
        for (x = 0; x < game_state.level_width; x++) {
            tile = get_tile(x, y);

            if (tile == TILE_HOLE_A || tile == '[') {
                holeA_x = x;
                holeA_y = y;
                holeA_found = 1;
            } else if (tile == TILE_HOLE_B || tile == ']') {
                holeB_x = x;
                holeB_y = y;
                holeB_found = 1;
            }
        }
    }

    // Check which players are on holes
    for (i = 0; i < game_state.num_players; i++) {
        if (game_state.players[i].under_tile == TILE_HOLE_A ||
            game_state.players[i].under_tile == '[') {
            holeA_has_player = 1;
        } else if (game_state.players[i].under_tile == TILE_HOLE_B ||
                   game_state.players[i].under_tile == ']') {
            holeB_has_player = 1;
        }
    }

    // Handle disappear if both holes have players
    if (holeA_has_player && holeB_has_player) {
        // Remove all players and mark level complete
        game_state.num_players = 0;
        game_state.level_complete = 1;
        // Mark holes as filled
        set_tile(holeA_x, holeA_y, '[');
        my_cputcxy(holeA_x, holeA_y + 2, '[');
        set_tile(holeB_x, holeB_y, ']');
        my_cputcxy(holeB_x, holeB_y + 2, ']');
        return;
    }

    // Handle duplication: holeA has player, holeB empty
    if (holeA_has_player && holeB_found && !holeB_has_player) {
        if (game_state.num_players < MAX_PLAYERS) {
            game_state.players[game_state.num_players].x = holeB_x;
            game_state.players[game_state.num_players].y = holeB_y;
            game_state.players[game_state.num_players].under_tile = TILE_HOLE_B;
            game_state.num_players++;
            set_tile(holeB_x, holeB_y, TILE_PLAYER);
            my_cputcxy(holeB_x, holeB_y + 2, TILE_PLAYER);
        }
    }

    // Handle duplication: holeB has player, holeA empty
    if (holeB_has_player && holeA_found && !holeA_has_player) {
        if (game_state.num_players < MAX_PLAYERS) {
            game_state.players[game_state.num_players].x = holeA_x;
            game_state.players[game_state.num_players].y = holeA_y;
            game_state.players[game_state.num_players].under_tile = TILE_HOLE_A;
            game_state.num_players++;
            set_tile(holeA_x, holeA_y, TILE_PLAYER);
            my_cputcxy(holeA_x, holeA_y + 2, TILE_PLAYER);
        }
    }

    // Reset filled holes that are now empty
    for (y = 0; y < game_state.level_height; y++) {
        for (x = 0; x < game_state.level_width; x++) {
            tile = get_tile(x, y);

            if (tile == '[') {
                set_tile(x, y, TILE_HOLE_A);
                my_cputcxy(x, y + 2, TILE_HOLE_A);
            } else if (tile == ']') {
                set_tile(x, y, TILE_HOLE_B);
                my_cputcxy(x, y + 2, TILE_HOLE_B);
            }
        }
    }
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
    byte i;
    byte new_x, new_y;
    char target_tile;
    byte can_move;

    // If no players, can't move
    if (game_state.num_players == 0) {
        return 0;
    }

    // Check if ALL players can move in this direction
    for (i = 0; i < game_state.num_players; i++) {
        new_x = game_state.players[i].x + dx;
        new_y = game_state.players[i].y + dy;

        // Check bounds
        if (new_x >= MAX_LEVEL_WIDTH || new_y >= MAX_LEVEL_HEIGHT) {
            return 0;  // Out of bounds
        }

        target_tile = get_tile(new_x, new_y);

        // Check if this player can move to target
        if (!is_passable(target_tile) && !is_pushable(target_tile)) {
            return 0;  // Blocked
        }
    }

    // Try to move all players
    can_move = 1;

    // First check if all moves are valid (passable tiles)
    for (i = 0; i < game_state.num_players; i++) {
        new_x = game_state.players[i].x + dx;
        new_y = game_state.players[i].y + dy;
        target_tile = get_tile(new_x, new_y);

        if (!is_passable(target_tile)) {
            can_move = 0;
            break;
        }
    }

    // If all players can move to passable tiles, move them
    if (can_move) {
        // Move all players
        for (i = 0; i < game_state.num_players; i++) {
            // Restore tile under old position
            set_tile(game_state.players[i].x, game_state.players[i].y,
                     game_state.players[i].under_tile);
            my_cputcxy(game_state.players[i].x, game_state.players[i].y + 2,
                       game_state.players[i].under_tile);

            // Calculate new position
            new_x = game_state.players[i].x + dx;
            new_y = game_state.players[i].y + dy;
            target_tile = get_tile(new_x, new_y);

            // Update player position
            game_state.players[i].x = new_x;
            game_state.players[i].y = new_y;
            game_state.players[i].under_tile = target_tile;

            // Check if player reached exit
            if (is_exit(target_tile)) {
                game_state.level_complete = 1;
            }

            // Set player at new position
            set_tile(new_x, new_y, TILE_PLAYER);
            my_cputcxy(new_x, new_y + 2, TILE_PLAYER);
        }

        // Increment move counter
        game_state.moves++;

        // Update gates based on plate activation
        update_gates();

        // Handle duplication mechanics
        handle_duplication();

        return 1;  // Move successful
    }

    // Check if any player needs to push
    // Try to push all pushable objects and move all players
    for (i = 0; i < game_state.num_players; i++) {
        new_x = game_state.players[i].x + dx;
        new_y = game_state.players[i].y + dy;
        target_tile = get_tile(new_x, new_y);

        if (is_pushable(target_tile)) {
            // Try to push this object
            if (!try_push(new_x, new_y, dx, dy)) {
                return 0;  // Can't push, so can't move
            }
        }
    }

    // All pushes successful (or no pushes needed), now move all players
    for (i = 0; i < game_state.num_players; i++) {
        // Restore tile under old position
        set_tile(game_state.players[i].x, game_state.players[i].y,
                 game_state.players[i].under_tile);
        my_cputcxy(game_state.players[i].x, game_state.players[i].y + 2,
                   game_state.players[i].under_tile);

        // Calculate new position
        new_x = game_state.players[i].x + dx;
        new_y = game_state.players[i].y + dy;

        // Update player position
        game_state.players[i].x = new_x;
        game_state.players[i].y = new_y;
        // Objects were pushed away, so underneath is now floor
        game_state.players[i].under_tile = TILE_FLOOR;

        // Set player at new position
        set_tile(new_x, new_y, TILE_PLAYER);
        my_cputcxy(new_x, new_y + 2, TILE_PLAYER);
    }

    // Increment move counter
    game_state.moves++;

    // Update gates based on plate activation
    update_gates();

    // Handle duplication mechanics
    handle_duplication();

    return 1;  // Move and push successful

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

