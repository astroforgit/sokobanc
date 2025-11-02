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

static Position flood_queue[32];  // Reduced from 64 to 32
static byte queue_start;
static byte queue_end;

void load_level(const char* level_data[], byte num_rows) {
    byte x, y;
    const char* row;
    char tile;

    // Clear the level map
    memset(level_map, ' ', sizeof(level_map));

    // Reset game state
    game_state.num_players = 0;
    game_state.num_objects = 0;
    game_state.level_width = 0;
    game_state.level_height = num_rows;
    game_state.moves = 0;
    game_state.level_complete = 0;

    // Load level data and find player position and objects
    for (y = 0; y < num_rows; y++) {
        row = level_data[y];
        x = 0;
        while (row[x] != '\0' && x < MAX_LEVEL_WIDTH) {
            tile = row[x];
            level_map[y][x] = tile;

            // Find player starting position (can be 'p' or 'P')
            if (tile == 'p' || tile == 'P') {
                game_state.players[0].x = x;
                game_state.players[0].y = y;
                game_state.players[0].under = TILE_FLOOR;
                game_state.num_players = 1;
                level_map[y][x] = TILE_PLAYER;
            }
            // Check for 'z' = player on holeA
            else if (tile == 'z') {
                game_state.players[0].x = x;
                game_state.players[0].y = y;
                game_state.players[0].under = TILE_HOLE_A;
                game_state.num_players = 1;
                level_map[y][x] = TILE_PLAYER;
            }
            // Track pushable objects (keys, crates, enemies)
            else if ((tile == 'k' || tile == '*' || tile == 'e') && game_state.num_objects < MAX_OBJECTS) {
                game_state.objects[game_state.num_objects].x = x;
                game_state.objects[game_state.num_objects].y = y;
                game_state.objects[game_state.num_objects].type = tile;
                game_state.objects[game_state.num_objects].under = TILE_FLOOR;
                game_state.num_objects++;
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

// is_exit and is_pushable are now macros in the header file

void door_flood_fill(byte x, byte y) {
    byte cx, cy, nx, ny;
    char tile;
    char dx, dy, i;

    queue_start = 0;
    queue_end = 0;
    flood_queue[queue_end].x = x;
    flood_queue[queue_end].y = y;
    queue_end++;
    set_tile(x, y, TILE_DOOR_OPEN);

    while (queue_start != queue_end) {
        cx = flood_queue[queue_start].x;
        cy = flood_queue[queue_start].y;
        queue_start++;

        // Check 4 directions: up, down, left, right
        for (i = 0; i < 4; i++) {
            dx = (i == 2) ? -1 : (i == 3) ? 1 : 0;
            dy = (i == 0) ? -1 : (i == 1) ? 1 : 0;
            nx = cx + dx;
            ny = cy + dy;

            if (nx < MAX_LEVEL_WIDTH && ny < MAX_LEVEL_HEIGHT) {
                tile = get_tile(nx, ny);
                if (tile == TILE_DOOR) {
                    set_tile(nx, ny, TILE_DOOR_OPEN);
                    flood_queue[queue_end].x = nx;
                    flood_queue[queue_end].y = ny;
                    queue_end++;
                }
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
        if (game_state.players[i].under == TILE_PLATE_A) {
            plateA_has_object = 1;
        }
        if (game_state.players[i].under == TILE_PLATE_B) {
            plateB_has_object = 1;
        }
    }

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

// Optimized duplication handler
void handle_duplication(void) {
    byte i, j, x, y;
    byte player_holeA = 0, player_holeB = 0;
    byte key_holeA = 0, key_holeB = 0;

    // Count players in each hole type
    for (i = 0; i < game_state.num_players; i++) {
        if (game_state.players[i].under == TILE_HOLE_A) player_holeA++;
        if (game_state.players[i].under == TILE_HOLE_B) player_holeB++;
    }

    // Count keys in each hole type
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].type == 'k') {
            if (game_state.objects[i].under == TILE_HOLE_A) key_holeA++;
            if (game_state.objects[i].under == TILE_HOLE_B) key_holeB++;
        }
    }

    // If both holes have players, they disappear (win condition)
    if (player_holeA > 0 && player_holeB > 0) {
        j = 0;
        for (i = 0; i < game_state.num_players; i++) {
            if (game_state.players[i].under != TILE_HOLE_A &&
                game_state.players[i].under != TILE_HOLE_B) {
                game_state.players[j++] = game_state.players[i];
            } else {
                set_tile(game_state.players[i].x, game_state.players[i].y, game_state.players[i].under);
                my_cputcxy(game_state.players[i].x, game_state.players[i].y + 2, game_state.players[i].under);
            }
        }
        game_state.num_players = j;
        if (game_state.num_players == 0) {
            game_state.level_complete = 1;
        }
        return;
    }

    // If both holes have keys, they disappear
    if (key_holeA > 0 && key_holeB > 0) {
        j = 0;
        for (i = 0; i < game_state.num_objects; i++) {
            if (game_state.objects[i].type != 'k' ||
                (game_state.objects[i].under != TILE_HOLE_A &&
                 game_state.objects[i].under != TILE_HOLE_B)) {
                game_state.objects[j++] = game_state.objects[i];
            } else {
                set_tile(game_state.objects[i].x, game_state.objects[i].y, game_state.objects[i].under);
                my_cputcxy(game_state.objects[i].x, game_state.objects[i].y + 2, game_state.objects[i].under);
            }
        }
        game_state.num_objects = j;
    }

    // Duplicate players
    if (game_state.num_players < MAX_PLAYERS) {
        for (y = 0; y < game_state.level_height; y++) {
            for (x = 0; x < game_state.level_width; x++) {
                char tile = get_tile(x, y);
                if (tile == TILE_HOLE_A && player_holeB > 0 && game_state.num_players < MAX_PLAYERS) {
                    game_state.players[game_state.num_players].x = x;
                    game_state.players[game_state.num_players].y = y;
                    game_state.players[game_state.num_players].under = TILE_HOLE_A;
                    set_tile(x, y, TILE_PLAYER);
                    my_cputcxy(x, y + 2, TILE_PLAYER);
                    game_state.num_players++;
                    player_holeA++;
                }
                else if (tile == TILE_HOLE_B && player_holeA > 0 && game_state.num_players < MAX_PLAYERS) {
                    game_state.players[game_state.num_players].x = x;
                    game_state.players[game_state.num_players].y = y;
                    game_state.players[game_state.num_players].under = TILE_HOLE_B;
                    set_tile(x, y, TILE_PLAYER);
                    my_cputcxy(x, y + 2, TILE_PLAYER);
                    game_state.num_players++;
                    player_holeB++;
                }
            }
        }
    }

    // Duplicate keys
    if (game_state.num_objects < MAX_OBJECTS) {
        for (y = 0; y < game_state.level_height; y++) {
            for (x = 0; x < game_state.level_width; x++) {
                char tile = get_tile(x, y);
                if (tile == TILE_HOLE_A && key_holeB > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = 'k';
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_A;
                    set_tile(x, y, 'k');
                    my_cputcxy(x, y + 2, 'k');
                    game_state.num_objects++;
                    key_holeA++;
                }
                else if (tile == TILE_HOLE_B && key_holeA > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = 'k';
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_B;
                    set_tile(x, y, 'k');
                    my_cputcxy(x, y + 2, 'k');
                    game_state.num_objects++;
                    key_holeB++;
                }
            }
        }
    }
}

byte try_push(byte x, byte y, char dx, char dy) {
    char object_tile;
    char behind_tile;
    byte behind_x, behind_y;
    byte i;
    char old_under = TILE_FLOOR;

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
        // Find this object in the objects array and remove it
        for (i = 0; i < game_state.num_objects; i++) {
            if (game_state.objects[i].x == x && game_state.objects[i].y == y) {
                old_under = game_state.objects[i].under;
                // Remove this object by shifting array
                for (; i < game_state.num_objects - 1; i++) {
                    game_state.objects[i] = game_state.objects[i + 1];
                }
                game_state.num_objects--;
                break;
            }
        }
        // Restore what was under the key
        set_tile(x, y, old_under);
        my_cputcxy(x, y + 2, old_under);
        // Handle key-door interaction
        handle_key_door(x, y, behind_x, behind_y);
        return 1;  // Push successful (key and doors removed)
    }

    // Check if space behind is passable (empty or floor-like)
    if (!is_passable(behind_tile)) {
        return 0;  // Can't push into blocking tile
    }

    // Find this object in the objects array and update it
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].x == x && game_state.objects[i].y == y) {
            old_under = game_state.objects[i].under;
            // Update object position and what's underneath
            game_state.objects[i].x = behind_x;
            game_state.objects[i].y = behind_y;
            game_state.objects[i].under = behind_tile;
            break;
        }
    }

    // Perform the push
    // Restore what was under the object
    set_tile(x, y, old_under);
    my_cputcxy(x, y + 2, old_under);

    // Move object to new position
    set_tile(behind_x, behind_y, object_tile);
    my_cputcxy(behind_x, behind_y + 2, object_tile);

    return 1;  // Push successful
}

byte try_move_player(char dx, char dy) {
    byte i, new_x, new_y;
    char target_tile;
    byte moved = 0;

    // Move all players simultaneously
    for (i = 0; i < game_state.num_players; i++) {
        new_x = game_state.players[i].x + dx;
        new_y = game_state.players[i].y + dy;

        // Check bounds
        if (new_x >= MAX_LEVEL_WIDTH || new_y >= MAX_LEVEL_HEIGHT) {
            continue;
        }

        target_tile = get_tile(new_x, new_y);

        // Check if target is passable
        if (is_passable(target_tile)) {
            // Restore tile under old position
            set_tile(game_state.players[i].x, game_state.players[i].y, game_state.players[i].under);
            my_cputcxy(game_state.players[i].x, game_state.players[i].y + 2, game_state.players[i].under);

            // Move player
            game_state.players[i].x = new_x;
            game_state.players[i].y = new_y;
            game_state.players[i].under = target_tile;

            // Check if reached exit
            if (is_exit(target_tile)) {
                game_state.level_complete = 1;
            }

            // Set player at new position
            set_tile(new_x, new_y, TILE_PLAYER);
            my_cputcxy(new_x, new_y + 2, TILE_PLAYER);
            moved = 1;
        }
        // Check if target is pushable
        else if (is_pushable(target_tile)) {
            if (try_push(new_x, new_y, dx, dy)) {
                // Restore tile under old position
                set_tile(game_state.players[i].x, game_state.players[i].y, game_state.players[i].under);
                my_cputcxy(game_state.players[i].x, game_state.players[i].y + 2, game_state.players[i].under);

                // Move player
                game_state.players[i].x = new_x;
                game_state.players[i].y = new_y;
                game_state.players[i].under = TILE_FLOOR;

                set_tile(new_x, new_y, TILE_PLAYER);
                my_cputcxy(new_x, new_y + 2, TILE_PLAYER);
                moved = 1;
            }
        }
    }

    if (moved) {
        game_state.moves++;
        handle_duplication();
        update_gates();
    }

    return moved;
}

byte is_level_complete(void) {
    return game_state.level_complete;
}

GameState* get_game_state(void) {
    return &game_state;
}

