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
// Background layer (for tiles under objects)
static char background_map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];

// Tile category lookup table (256 bytes - one for each ASCII character)
// Each byte contains bit flags for tile properties
const byte tile_categories[256] = {
    [' '] = TILE_CAT_PASSABLE,                                    // TILE_EMPTY
    ['.'] = TILE_CAT_PASSABLE,                                    // TILE_FLOOR
    ['#'] = TILE_CAT_BLOCKING,                                    // TILE_WALL
    ['p'] = TILE_CAT_BLOCKING,                                    // TILE_PLAYER (blocking to other players)
    ['*'] = TILE_CAT_BLOCKING | TILE_CAT_PUSHABLE,                // TILE_CRATE
    ['k'] = TILE_CAT_BLOCKING | TILE_CAT_PUSHABLE,                // TILE_KEY
    ['e'] = TILE_CAT_BLOCKING | TILE_CAT_PUSHABLE,                // TILE_ENEMY
    ['d'] = TILE_CAT_BLOCKING,                                    // TILE_DOOR (closed)
    ['D'] = TILE_CAT_PASSABLE,                                    // TILE_DOOR_OPEN
    ['?'] = TILE_CAT_PASSABLE | TILE_CAT_HOLE,                    // TILE_HOLE_A
    ['!'] = TILE_CAT_PASSABLE | TILE_CAT_HOLE,                    // TILE_HOLE_B
    ['['] = TILE_CAT_PASSABLE | TILE_CAT_HOLE,                    // TILE_HOLE_A filled
    [']'] = TILE_CAT_PASSABLE | TILE_CAT_HOLE,                    // TILE_HOLE_B filled
    ['b'] = TILE_CAT_PASSABLE | TILE_CAT_PLATE,                   // TILE_PLATE_A
    ['c'] = TILE_CAT_PASSABLE | TILE_CAT_PLATE,                   // TILE_PLATE_B
    ['g'] = TILE_CAT_BLOCKING | TILE_CAT_GATE,                    // TILE_GATE_A (closed)
    ['h'] = TILE_CAT_BLOCKING | TILE_CAT_GATE,                    // TILE_GATE_B (closed)
    ['G'] = TILE_CAT_PASSABLE | TILE_CAT_GATE,                    // TILE_GATE_A (open)
    ['H'] = TILE_CAT_PASSABLE | TILE_CAT_GATE,                    // TILE_GATE_B (open)
    ['@'] = TILE_CAT_PASSABLE | TILE_CAT_EXIT,                    // TILE_EXIT_A
    [':'] = TILE_CAT_PASSABLE | TILE_CAT_EXIT,                    // TILE_EXIT_B
    [';'] = TILE_CAT_PASSABLE | TILE_CAT_EXIT,                    // TILE_EXIT_C
    ['$'] = TILE_CAT_BLOCKING,                                    // Wall line A
    ['%'] = TILE_CAT_BLOCKING,                                    // Wall line B
    ['&'] = TILE_CAT_BLOCKING,                                    // Wall line C
    ['1'] = TILE_CAT_PASSABLE,                                    // Decorative line 1
    ['2'] = TILE_CAT_PASSABLE,                                    // Decorative line 2
    ['3'] = TILE_CAT_PASSABLE,                                    // Decorative line 3
    ['4'] = TILE_CAT_PASSABLE,                                    // Decorative line 4
    ['5'] = TILE_CAT_PASSABLE,                                    // Decorative line 5
    ['6'] = TILE_CAT_PASSABLE,                                    // Decorative line 6
    ['7'] = TILE_CAT_PASSABLE,                                    // Decorative line 7
    ['8'] = TILE_CAT_PASSABLE,                                    // Decorative line 8
};

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

    // Clear the maps
    memset(level_map, ' ', sizeof(level_map));
    memset(background_map, '.', sizeof(background_map));

    // Reset game state
    game_state.num_players = 0;
    game_state.num_objects = 0;
    game_state.level_width = 0;
    game_state.level_height = num_rows;
    game_state.level_complete = 0;

    // First pass: Load all tiles and separate objects from background
    for (y = 0; y < num_rows; y++) {
        row = level_data[y];
        x = 0;
        while (row[x] != '\0' && x < MAX_LEVEL_WIDTH) {
            tile = row[x];

            // Determine if this is an object or background
            if (tile == TILE_PLAYER || is_pushable(tile)) {
                // Object - store floor as background
                background_map[y][x] = TILE_FLOOR;
                level_map[y][x] = tile;
            } else if (tile == 'z') {
                // Player on holeA
                background_map[y][x] = TILE_HOLE_A;
                level_map[y][x] = TILE_PLAYER;
            } else if (tile == 'y') {
                // Enemy on holeB
                background_map[y][x] = TILE_HOLE_B;
                level_map[y][x] = TILE_ENEMY;
            } else {
                // Background tile
                background_map[y][x] = tile;
                level_map[y][x] = tile;
            }

            x++;
        }

        // Track the maximum width
        if (x > game_state.level_width) {
            game_state.level_width = x;
        }
    }

    // Second pass: Extract objects into arrays
    for (y = 0; y < num_rows; y++) {
        for (x = 0; x < game_state.level_width; x++) {
            tile = level_map[y][x];

            // Find player starting position
            if (tile == TILE_PLAYER) {
                game_state.players[0].x = x;
                game_state.players[0].y = y;
                game_state.players[0].under = background_map[y][x];
                game_state.num_players = 1;
                level_map[y][x] = TILE_PLAYER;
            }
            // Track pushable objects (keys, crates, enemies)
            else if (is_pushable(tile) && game_state.num_objects < MAX_OBJECTS) {
                game_state.objects[game_state.num_objects].x = x;
                game_state.objects[game_state.num_objects].y = y;
                game_state.objects[game_state.num_objects].type = tile;
                game_state.objects[game_state.num_objects].under = background_map[y][x];
                game_state.num_objects++;
            }
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
            if (tile != TILE_EMPTY) {
                my_cputcxy(x, y + SCREEN_TOP_MARGIN, tile);
            } else {
                my_cputcxy(x, y + SCREEN_TOP_MARGIN, TILE_EMPTY);
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

void set_tile_and_draw(byte x, byte y, char tile) {
    set_tile(x, y, tile);
    my_cputcxy(x, y + SCREEN_TOP_MARGIN, tile);
}

byte is_blocking(char tile) {
    return (tile_categories[(byte)tile] & TILE_CAT_BLOCKING) != 0;
}

byte is_passable(char tile) {
    return (tile_categories[(byte)tile] & TILE_CAT_PASSABLE) != 0;
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
                set_tile_and_draw(x, y, TILE_FLOOR);
            }
        }
    }
}

void handle_key_door(byte key_x, byte key_y, byte door_x, byte door_y) {
    // Remove key
    set_tile_and_draw(key_x, key_y, TILE_FLOOR);

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
                        set_tile_and_draw(x, y, 'G');
                    }
                } else {
                    // Close gate
                    if (tile != TILE_GATE_A) {
                        set_tile_and_draw(x, y, TILE_GATE_A);
                    }
                }
            }

            // Update gateB
            if (tile == TILE_GATE_B || tile == 'H') {
                if (plateB_has_object) {
                    // Open gate
                    if (tile != 'H') {
                        set_tile_and_draw(x, y, 'H');
                    }
                } else {
                    // Close gate
                    if (tile != TILE_GATE_B) {
                        set_tile_and_draw(x, y, TILE_GATE_B);
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
        if (game_state.objects[i].type == TILE_KEY) {
            if (game_state.objects[i].under == TILE_HOLE_A) key_holeA++;
            if (game_state.objects[i].under == TILE_HOLE_B) key_holeB++;
        }
    }

    // If both holes have players, they disappear (win condition)
    if (player_holeA > 0 && player_holeB > 0) {
        j = 0;
        for (i = 0; i < game_state.num_players; i++) {
            if (!is_hole(game_state.players[i].under)) {
                game_state.players[j++] = game_state.players[i];
            } else {
                set_tile_and_draw(game_state.players[i].x, game_state.players[i].y, game_state.players[i].under);
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
            if (game_state.objects[i].type != TILE_KEY || !is_hole(game_state.objects[i].under)) {
                game_state.objects[j++] = game_state.objects[i];
            } else {
                set_tile_and_draw(game_state.objects[i].x, game_state.objects[i].y, game_state.objects[i].under);
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
                    set_tile_and_draw(x, y, TILE_PLAYER);
                    game_state.num_players++;
                    player_holeA++;
                }
                else if (tile == TILE_HOLE_B && player_holeA > 0 && game_state.num_players < MAX_PLAYERS) {
                    game_state.players[game_state.num_players].x = x;
                    game_state.players[game_state.num_players].y = y;
                    game_state.players[game_state.num_players].under = TILE_HOLE_B;
                    set_tile_and_draw(x, y, TILE_PLAYER);
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
                    game_state.objects[game_state.num_objects].type = TILE_KEY;
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_A;
                    set_tile_and_draw(x, y, TILE_KEY);
                    game_state.num_objects++;
                    key_holeA++;
                }
                else if (tile == TILE_HOLE_B && key_holeA > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = TILE_KEY;
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_B;
                    set_tile_and_draw(x, y, TILE_KEY);
                    game_state.num_objects++;
                    key_holeB++;
                }
            }
        }
    }
}
/*
  Try to push an object at a position in a direction
  (Corrected version to handle background tiles properly)
*/
byte try_push(byte x, byte y, char dx, char dy) {
    byte i;
    char object_tile = get_tile(x, y);
    byte behind_x = x + dx;
    byte behind_y = y + dy;
    char behind_tile;

    // Check bounds
    if (behind_x >= MAX_LEVEL_WIDTH || behind_y >= MAX_LEVEL_HEIGHT) {
        return 0;  // Can't push out of bounds
    }

    behind_tile = get_tile(behind_x, behind_y);

    // Special case: Key pushing into door
    if (object_tile == TILE_KEY && behind_tile == TILE_DOOR) {
        for (i = 0; i < game_state.num_objects; i++) {
            if (game_state.objects[i].x == x && game_state.objects[i].y == y) {
                // Restore the tile the key was on before it disappears
                set_tile_and_draw(x, y, game_state.objects[i].under);

                // Remove this object by shifting array
                for (; i < game_state.num_objects - 1; i++) {
                    game_state.objects[i] = game_state.objects[i + 1];
                }
                game_state.num_objects--;
                break;
            }
        }
        handle_key_door(x, y, behind_x, behind_y);
        return 1;
    }

    // Check if space behind is passable
    if (!is_passable(behind_tile)) {
        return 0;  // Can't push into blocking tile
    }

    // Find the object being pushed to update it
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].x == x && game_state.objects[i].y == y) {
            // *** THE FIX IS HERE ***

            // 1. Get the tile to restore from the object's OWN memory.
            char tile_to_restore = game_state.objects[i].under;

            // 2. Update the object's internal position and memory for its NEW tile.
            game_state.objects[i].x = behind_x;
            game_state.objects[i].y = behind_y;
            game_state.objects[i].under = behind_tile; // Remembers the dynamic tile it moves onto.

            // 3. Restore the tile at the object's OLD position using its saved memory.
            set_tile_and_draw(x, y, tile_to_restore);

            // 4. Move the object to its new position on the map.
            set_tile_and_draw(behind_x, behind_y, object_tile);

            return 1; // Push successful
        }
    }

    return 0; // Should not be reached, but as a fallback
}

 /*
  Try to move the player in the given direction
  (Corrected version to handle pushing objects correctly)
*/
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
            set_tile_and_draw(game_state.players[i].x, game_state.players[i].y, game_state.players[i].under);

            // Move player
            game_state.players[i].x = new_x;
            game_state.players[i].y = new_y;
            game_state.players[i].under = target_tile;

            // Check if reached exit
            if (is_exit(target_tile)) {
                game_state.level_complete = 1;
            }

            // Set player at new position
            set_tile_and_draw(new_x, new_y, TILE_PLAYER);
            moved = 1;
        }
        // Check if target is pushable
        else if (is_pushable(target_tile)) {
            // The corrected try_push now handles restoring tiles correctly.
            if (try_push(new_x, new_y, dx, dy)) {
                // Restore tile under old position
                set_tile_and_draw(game_state.players[i].x, game_state.players[i].y, game_state.players[i].under);

                // Move player
                game_state.players[i].x = new_x;
                game_state.players[i].y = new_y;

                // *** THE FIX IS HERE ***
                // The player moves into the tile the object just left. We need to
                // re-read that tile to correctly update the player's 'under' memory.
                game_state.players[i].under = get_tile(new_x, new_y);

                set_tile_and_draw(new_x, new_y, TILE_PLAYER);
                moved = 1;
            }
        }
    }

    if (moved) {
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

