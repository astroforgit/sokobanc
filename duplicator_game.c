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
byte tile_categories[256];

// Initialize the tile category lookup table
static void init_tile_categories(void) {
    // Initialize all to 0 (no flags)
    memset(tile_categories, 0, sizeof(tile_categories));

    // Set tile categories
    tile_categories[' '] = TILE_CAT_PASSABLE;                                    // TILE_EMPTY
    tile_categories['.'] = TILE_CAT_PASSABLE;                                    // TILE_FLOOR
    tile_categories['#'] = TILE_CAT_BLOCKING;                                    // TILE_WALL
    tile_categories['p'] = TILE_CAT_BLOCKING;                                    // TILE_PLAYER (blocking to other players)
    tile_categories['*'] = TILE_CAT_BLOCKING | TILE_CAT_PUSHABLE;                // TILE_CRATE
    tile_categories['k'] = TILE_CAT_BLOCKING | TILE_CAT_PUSHABLE;                // TILE_KEY
    tile_categories['e'] = TILE_CAT_BLOCKING | TILE_CAT_PUSHABLE;                // TILE_ENEMY
    tile_categories['d'] = TILE_CAT_BLOCKING;                                    // TILE_DOOR (closed)
    tile_categories['D'] = TILE_CAT_PASSABLE;                                    // TILE_DOOR_OPEN
    tile_categories['?'] = TILE_CAT_PASSABLE | TILE_CAT_HOLE;                    // TILE_HOLE_A
    tile_categories['!'] = TILE_CAT_PASSABLE | TILE_CAT_HOLE;                    // TILE_HOLE_B
    tile_categories['['] = TILE_CAT_PASSABLE | TILE_CAT_HOLE;                    // TILE_HOLE_A filled
    tile_categories[']'] = TILE_CAT_PASSABLE | TILE_CAT_HOLE;                    // TILE_HOLE_B filled
    tile_categories['b'] = TILE_CAT_PASSABLE | TILE_CAT_PLATE;                   // TILE_PLATE_A
    tile_categories['c'] = TILE_CAT_PASSABLE | TILE_CAT_PLATE;                   // TILE_PLATE_B
    tile_categories['g'] = TILE_CAT_BLOCKING | TILE_CAT_GATE;                    // TILE_GATE_A (closed)
    tile_categories['h'] = TILE_CAT_BLOCKING | TILE_CAT_GATE;                    // TILE_GATE_B (closed)
    tile_categories['G'] = TILE_CAT_PASSABLE | TILE_CAT_GATE;                    // TILE_GATE_A (open)
    tile_categories['H'] = TILE_CAT_PASSABLE | TILE_CAT_GATE;                    // TILE_GATE_B (open)
    tile_categories['@'] = TILE_CAT_PASSABLE | TILE_CAT_EXIT;                    // TILE_EXIT_A
    tile_categories[':'] = TILE_CAT_PASSABLE | TILE_CAT_EXIT;                    // TILE_EXIT_B
    tile_categories[';'] = TILE_CAT_PASSABLE | TILE_CAT_EXIT;                    // TILE_EXIT_C
    tile_categories['$'] = TILE_CAT_BLOCKING;                                    // Wall line A
    tile_categories['%'] = TILE_CAT_BLOCKING;                                    // Wall line B
    tile_categories['&'] = TILE_CAT_BLOCKING;                                    // Wall line C
    tile_categories['1'] = TILE_CAT_PASSABLE;                                    // Decorative line 1
    tile_categories['2'] = TILE_CAT_PASSABLE;                                    // Decorative line 2
    tile_categories['3'] = TILE_CAT_PASSABLE;                                    // Decorative line 3
    tile_categories['4'] = TILE_CAT_PASSABLE;                                    // Decorative line 4
    tile_categories['5'] = TILE_CAT_PASSABLE;                                    // Decorative line 5
    tile_categories['6'] = TILE_CAT_PASSABLE;                                    // Decorative line 6
    tile_categories['7'] = TILE_CAT_PASSABLE;                                    // Decorative line 7
    tile_categories['8'] = TILE_CAT_PASSABLE;                                    // Decorative line 8
}

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
    static byte categories_initialized = 0;

    // Initialize tile categories on first call
    if (!categories_initialized) {
        init_tile_categories();
        categories_initialized = 1;
    }

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

    // Reset duplication tracking so objects already on holes don't trigger duplication
    reset_duplication_tracking();
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

// Static arrays to track previous 'under' state for duplication detection
static char prev_player_under[MAX_PLAYERS];
static char prev_object_under[MAX_OBJECTS];
static byte tracking_initialized = 0;

// Reset duplication tracking (call when loading a new level)
void reset_duplication_tracking(void) {
    byte i;

    // Initialize ALL entries to prevent stale data
    for (i = 0; i < MAX_PLAYERS; i++) {
        prev_player_under[i] = TILE_FLOOR;
    }
    for (i = 0; i < MAX_OBJECTS; i++) {
        prev_object_under[i] = TILE_FLOOR;
    }

    // Then set current state for existing players/objects
    for (i = 0; i < game_state.num_players; i++) {
        prev_player_under[i] = game_state.players[i].under;
    }
    for (i = 0; i < game_state.num_objects; i++) {
        prev_object_under[i] = game_state.objects[i].under;
    }

    tracking_initialized = 1;
}

// Optimized duplication handler
// NOTE: This is called AFTER movement, so we need to track which objects
// JUST ENTERED a hole this turn (not objects that were already on holes)
void handle_duplication(void) {
    byte i, j, x, y;
    byte player_holeA = 0, player_holeB = 0;
    byte key_holeA = 0, key_holeB = 0;
    byte total_player_holeA = 0, total_player_holeB = 0;
    byte total_key_holeA = 0, total_key_holeB = 0;
    char current_under, previous_under;

    // Safety check: if tracking not initialized, initialize it now
    if (!tracking_initialized) {
        reset_duplication_tracking();
        return;  // Don't trigger duplication on first call
    }

    // Count players that JUST ENTERED each hole type (not already on it)
    for (i = 0; i < game_state.num_players; i++) {
        current_under = game_state.players[i].under;
        previous_under = prev_player_under[i];

        // Only count if player just moved ONTO a hole (wasn't on a hole before)
        if (current_under == TILE_HOLE_A && !is_hole(previous_under)) {
            player_holeA++;
        }
        if (current_under == TILE_HOLE_B && !is_hole(previous_under)) {
            player_holeB++;
        }

        // Update previous state for next turn
        prev_player_under[i] = current_under;
    }

    // Count keys that JUST ENTERED each hole type (not already on it)
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].type == TILE_KEY) {
            current_under = game_state.objects[i].under;
            previous_under = prev_object_under[i];

            // Only count if key just moved ONTO a hole (wasn't on a hole before)
            if (current_under == TILE_HOLE_A && !is_hole(previous_under)) {
                key_holeA++;
            }
            if (current_under == TILE_HOLE_B && !is_hole(previous_under)) {
                key_holeB++;
            }

            // Update previous state for next turn
            prev_object_under[i] = current_under;
        }
    }

    // Check if BOTH holes have players (for disappearing, check ALL players on holes)

    for (i = 0; i < game_state.num_players; i++) {
        if (game_state.players[i].under == TILE_HOLE_A) total_player_holeA++;
        if (game_state.players[i].under == TILE_HOLE_B) total_player_holeB++;
    }

    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].type == TILE_KEY) {
            if (game_state.objects[i].under == TILE_HOLE_A) total_key_holeA++;
            if (game_state.objects[i].under == TILE_HOLE_B) total_key_holeB++;
        }
    }

    // If both holes have players, they disappear (win condition)
    if (total_player_holeA > 0 && total_player_holeB > 0) {
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
    if (total_key_holeA > 0 && total_key_holeB > 0) {
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
byte try_push(byte x, byte y, signed char dx, signed char dy) {
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
  New algorithm: Process players from back to front in movement direction
  This ensures that when multiple players are in a line, they all move together
*/
byte try_move_player(signed char dx, signed char dy) {
    byte i, j, new_x, new_y;
    char target_tile;
    byte moved = 0;
    byte player_order[MAX_PLAYERS];
    byte temp_idx;

    /* Step 1: Create an array of player indices */
    for (i = 0; i < game_state.num_players; i++) {
        player_order[i] = i;
    }

    /* Step 2: Sort players so we process them from BACK to FRONT
       in the movement direction. This means:
       - If moving RIGHT (dx=1), process rightmost players first
       - If moving LEFT (dx=-1), process leftmost players first
       - If moving DOWN (dy=1), process bottom players first
       - If moving UP (dy=-1), process top players first
    */
    for (i = 0; i < game_state.num_players - 1; i++) {
        for (j = i + 1; j < game_state.num_players; j++) {
            byte should_swap = 0;

            /* Determine if we should swap based on movement direction */
            if (dx == 1) {
                /* Moving right: process rightmost first */
                if (game_state.players[player_order[i]].x < game_state.players[player_order[j]].x) {
                    should_swap = 1;
                }
            } else if (dx == -1) {
                /* Moving left: process leftmost first */
                if (game_state.players[player_order[i]].x > game_state.players[player_order[j]].x) {
                    should_swap = 1;
                }
            } else if (dy == 1) {
                /* Moving down: process bottom first */
                if (game_state.players[player_order[i]].y < game_state.players[player_order[j]].y) {
                    should_swap = 1;
                }
            } else if (dy == -1) {
                /* Moving up: process top first */
                if (game_state.players[player_order[i]].y > game_state.players[player_order[j]].y) {
                    should_swap = 1;
                }
            }

            if (should_swap) {
                temp_idx = player_order[i];
                player_order[i] = player_order[j];
                player_order[j] = temp_idx;
            }
        }
    }

    /* Step 3: Process players in sorted order (back to front) */
    for (i = 0; i < game_state.num_players; i++) {
        byte player_idx = player_order[i];

        new_x = game_state.players[player_idx].x + dx;
        new_y = game_state.players[player_idx].y + dy;

        /* Check bounds */
        if (new_x >= MAX_LEVEL_WIDTH || new_y >= MAX_LEVEL_HEIGHT) {
            continue;
        }

        target_tile = get_tile(new_x, new_y);

        /* Check if target is passable */
        if (is_passable(target_tile)) {
            /* Restore tile under old position */
            set_tile_and_draw(game_state.players[player_idx].x, game_state.players[player_idx].y, game_state.players[player_idx].under);

            /* Move player */
            game_state.players[player_idx].x = new_x;
            game_state.players[player_idx].y = new_y;
            game_state.players[player_idx].under = target_tile;

            /* Check if reached exit */
            if (is_exit(target_tile)) {
                game_state.level_complete = 1;
            }

            /* Set player at new position */
            set_tile_and_draw(new_x, new_y, TILE_PLAYER);
            moved = 1;
        }
        /* Check if target is pushable */
        else if (is_pushable(target_tile)) {
            if (try_push(new_x, new_y, dx, dy)) {
                /* Restore tile under old position */
                set_tile_and_draw(game_state.players[player_idx].x, game_state.players[player_idx].y, game_state.players[player_idx].under);

                /* Move player */
                game_state.players[player_idx].x = new_x;
                game_state.players[player_idx].y = new_y;

                /* Re-read the tile to correctly update the player's 'under' memory */
                game_state.players[player_idx].under = get_tile(new_x, new_y);

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

