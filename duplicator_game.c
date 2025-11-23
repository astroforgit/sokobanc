/*
  duplicator_game.c - Game logic for Duplicator game
  Implementation file
*/

#include "duplicator_game.h"

/* Only include atari_conio.h if we're not in 16x16 mode */
/* In 16x16 mode, duplicator_conio_16x16.h provides the functions */
#ifndef DUPLICATOR_16X16_MODE
#include "atari_conio.h"
#endif

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

// Forward declaration
void reset_duplication_tracking(void);

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

            // Find player starting positions (support multiple players)
            if (tile == TILE_PLAYER && game_state.num_players < MAX_PLAYERS) {
                game_state.players[game_state.num_players].x = x;
                game_state.players[game_state.num_players].y = y;
                game_state.players[game_state.num_players].under = background_map[y][x];
                game_state.num_players++;
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

void handle_key_door(byte key_x, byte key_y, byte door_x, byte door_y, char tile_under_key) {
    // Remove key and restore the tile that was under it
    set_tile_and_draw(key_x, key_y, tile_under_key);

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

    // Check if any movable object (key, crate, enemy) is on a plate
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].under == TILE_PLATE_A) {
            plateA_has_object = 1;
        }
        if (game_state.objects[i].under == TILE_PLATE_B) {
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

// Track which holes had objects in the previous turn
// This prevents duplication when objects move OUT of holes
static byte prev_holeA_occupied = 0;
static byte prev_holeB_occupied = 0;

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

    // Check if holes are currently occupied
    prev_holeA_occupied = 0;
    prev_holeB_occupied = 0;
    for (i = 0; i < game_state.num_players; i++) {
        if (game_state.players[i].under == TILE_HOLE_A) prev_holeA_occupied = 1;
        if (game_state.players[i].under == TILE_HOLE_B) prev_holeB_occupied = 1;
    }
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].under == TILE_HOLE_A) prev_holeA_occupied = 1;
        if (game_state.objects[i].under == TILE_HOLE_B) prev_holeB_occupied = 1;
    }
}

// Optimized duplication handler
// Only triggers if something ENTERED a hole (moved from non-hole to hole)
// AND the hole was empty in the previous turn
void handle_duplication(void) {
    byte i, j, x, y;
    byte player_holeA = 0, player_holeB = 0;
    byte key_holeA = 0, key_holeB = 0;
    byte crate_holeA = 0, crate_holeB = 0;
    byte enemy_holeA = 0, enemy_holeB = 0;
    byte total_player_holeA = 0, total_player_holeB = 0;
    byte total_key_holeA = 0, total_key_holeB = 0;
    byte total_crate_holeA = 0, total_crate_holeB = 0;
    byte total_enemy_holeA = 0, total_enemy_holeB = 0;
    byte curr_holeA_occupied = 0, curr_holeB_occupied = 0;
    char current_under, previous_under;

    // Count players that JUST ENTERED each hole type (not already on it)
    // Only count if the hole was EMPTY in the previous turn
    for (i = 0; i < game_state.num_players; i++) {
        current_under = game_state.players[i].under;
        previous_under = prev_player_under[i];

        // Only count if player just moved ONTO a hole (wasn't on a hole before)
        // AND the hole was empty in the previous turn
        if (current_under == TILE_HOLE_A && !is_hole(previous_under) && !prev_holeA_occupied) {
            player_holeA++;
        }
        if (current_under == TILE_HOLE_B && !is_hole(previous_under) && !prev_holeB_occupied) {
            player_holeB++;
        }

        // Update previous state for next turn
        prev_player_under[i] = current_under;
    }

    // Count keys, crates, and enemies that JUST ENTERED each hole type (not already on it)
    // Only count if the hole was EMPTY in the previous turn
    for (i = 0; i < game_state.num_objects; i++) {
        current_under = game_state.objects[i].under;
        previous_under = prev_object_under[i];

        if (game_state.objects[i].type == TILE_KEY) {
            // Only count if key just moved ONTO a hole (wasn't on a hole before)
            // AND the hole was empty in the previous turn
            if (current_under == TILE_HOLE_A && !is_hole(previous_under) && !prev_holeA_occupied) {
                key_holeA++;
            }
            if (current_under == TILE_HOLE_B && !is_hole(previous_under) && !prev_holeB_occupied) {
                key_holeB++;
            }
        } else if (game_state.objects[i].type == TILE_CRATE) {
            // Only count if crate just moved ONTO a hole (wasn't on a hole before)
            // AND the hole was empty in the previous turn
            if (current_under == TILE_HOLE_A && !is_hole(previous_under) && !prev_holeA_occupied) {
                crate_holeA++;
            }
            if (current_under == TILE_HOLE_B && !is_hole(previous_under) && !prev_holeB_occupied) {
                crate_holeB++;
            }
        } else if (game_state.objects[i].type == TILE_ENEMY) {
            // Only count if enemy just moved ONTO a hole (wasn't on a hole before)
            // AND the hole was empty in the previous turn
            if (current_under == TILE_HOLE_A && !is_hole(previous_under) && !prev_holeA_occupied) {
                enemy_holeA++;
            }
            if (current_under == TILE_HOLE_B && !is_hole(previous_under) && !prev_holeB_occupied) {
                enemy_holeB++;
            }
        }

        // Update previous state for next turn
        prev_object_under[i] = current_under;
    }

    // Count total objects on holes (for disappearing check)
    for (i = 0; i < game_state.num_players; i++) {
        if (game_state.players[i].under == TILE_HOLE_A) total_player_holeA++;
        if (game_state.players[i].under == TILE_HOLE_B) total_player_holeB++;
    }

    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].type == TILE_KEY) {
            if (game_state.objects[i].under == TILE_HOLE_A) total_key_holeA++;
            if (game_state.objects[i].under == TILE_HOLE_B) total_key_holeB++;
        } else if (game_state.objects[i].type == TILE_CRATE) {
            if (game_state.objects[i].under == TILE_HOLE_A) total_crate_holeA++;
            if (game_state.objects[i].under == TILE_HOLE_B) total_crate_holeB++;
        } else if (game_state.objects[i].type == TILE_ENEMY) {
            if (game_state.objects[i].under == TILE_HOLE_A) total_enemy_holeA++;
            if (game_state.objects[i].under == TILE_HOLE_B) total_enemy_holeB++;
        }
    }

    // If a player just entered a hole AND both holes now have players, they disappear
    if ((player_holeA > 0 || player_holeB > 0) && total_player_holeA > 0 && total_player_holeB > 0) {
        j = 0;
        for (i = 0; i < game_state.num_players; i++) {
            if (!is_hole(game_state.players[i].under)) {
                game_state.players[j] = game_state.players[i];
                prev_player_under[j] = prev_player_under[i];
                j++;
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

    // If a key just entered a hole AND both holes now have keys, they disappear
    if ((key_holeA > 0 || key_holeB > 0) && total_key_holeA > 0 && total_key_holeB > 0) {
        j = 0;
        for (i = 0; i < game_state.num_objects; i++) {
            if (game_state.objects[i].type != TILE_KEY || !is_hole(game_state.objects[i].under)) {
                game_state.objects[j] = game_state.objects[i];
                prev_object_under[j] = prev_object_under[i];
                j++;
            } else {
                set_tile_and_draw(game_state.objects[i].x, game_state.objects[i].y, game_state.objects[i].under);
            }
        }
        game_state.num_objects = j;
        return;
    }

    // If a crate just entered a hole AND both holes now have crates, they disappear
    if ((crate_holeA > 0 || crate_holeB > 0) && total_crate_holeA > 0 && total_crate_holeB > 0) {
        j = 0;
        for (i = 0; i < game_state.num_objects; i++) {
            if (game_state.objects[i].type != TILE_CRATE || !is_hole(game_state.objects[i].under)) {
                game_state.objects[j] = game_state.objects[i];
                prev_object_under[j] = prev_object_under[i];
                j++;
            } else {
                set_tile_and_draw(game_state.objects[i].x, game_state.objects[i].y, game_state.objects[i].under);
            }
        }
        game_state.num_objects = j;
        return;
    }

    // If an enemy just entered a hole AND both holes now have enemies, they disappear
    if ((enemy_holeA > 0 || enemy_holeB > 0) && total_enemy_holeA > 0 && total_enemy_holeB > 0) {
        j = 0;
        for (i = 0; i < game_state.num_objects; i++) {
            if (game_state.objects[i].type != TILE_ENEMY || !is_hole(game_state.objects[i].under)) {
                game_state.objects[j] = game_state.objects[i];
                prev_object_under[j] = prev_object_under[i];
                j++;
            } else {
                set_tile_and_draw(game_state.objects[i].x, game_state.objects[i].y, game_state.objects[i].under);
            }
        }
        game_state.num_objects = j;
        return;
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
                    prev_player_under[game_state.num_players] = TILE_HOLE_A;
                    game_state.num_players++;
                    player_holeA++;
                }
                else if (tile == TILE_HOLE_B && player_holeA > 0 && game_state.num_players < MAX_PLAYERS) {
                    game_state.players[game_state.num_players].x = x;
                    game_state.players[game_state.num_players].y = y;
                    game_state.players[game_state.num_players].under = TILE_HOLE_B;
                    set_tile_and_draw(x, y, TILE_PLAYER);
                    prev_player_under[game_state.num_players] = TILE_HOLE_B;
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
                    prev_object_under[game_state.num_objects] = TILE_HOLE_A;
                    game_state.num_objects++;
                    key_holeA++;
                }
                else if (tile == TILE_HOLE_B && key_holeA > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = TILE_KEY;
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_B;
                    set_tile_and_draw(x, y, TILE_KEY);
                    prev_object_under[game_state.num_objects] = TILE_HOLE_B;
                    game_state.num_objects++;
                    key_holeB++;
                }
            }
        }
    }

    // Duplicate crates
    if (game_state.num_objects < MAX_OBJECTS) {
        for (y = 0; y < game_state.level_height; y++) {
            for (x = 0; x < game_state.level_width; x++) {
                char tile = get_tile(x, y);
                if (tile == TILE_HOLE_A && crate_holeB > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = TILE_CRATE;
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_A;
                    set_tile_and_draw(x, y, TILE_CRATE);
                    prev_object_under[game_state.num_objects] = TILE_HOLE_A;
                    game_state.num_objects++;
                    crate_holeA++;
                }
                else if (tile == TILE_HOLE_B && crate_holeA > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = TILE_CRATE;
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_B;
                    set_tile_and_draw(x, y, TILE_CRATE);
                    prev_object_under[game_state.num_objects] = TILE_HOLE_B;
                    game_state.num_objects++;
                    crate_holeB++;
                }
            }
        }
    }

    // Duplicate enemies
    if (game_state.num_objects < MAX_OBJECTS) {
        for (y = 0; y < game_state.level_height; y++) {
            for (x = 0; x < game_state.level_width; x++) {
                char tile = get_tile(x, y);
                if (tile == TILE_HOLE_A && enemy_holeB > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = TILE_ENEMY;
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_A;
                    set_tile_and_draw(x, y, TILE_ENEMY);
                    prev_object_under[game_state.num_objects] = TILE_HOLE_A;
                    game_state.num_objects++;
                    enemy_holeA++;
                }
                else if (tile == TILE_HOLE_B && enemy_holeA > 0 && game_state.num_objects < MAX_OBJECTS) {
                    game_state.objects[game_state.num_objects].x = x;
                    game_state.objects[game_state.num_objects].y = y;
                    game_state.objects[game_state.num_objects].type = TILE_ENEMY;
                    game_state.objects[game_state.num_objects].under = TILE_HOLE_B;
                    set_tile_and_draw(x, y, TILE_ENEMY);
                    prev_object_under[game_state.num_objects] = TILE_HOLE_B;
                    game_state.num_objects++;
                    enemy_holeB++;
                }
            }
        }
    }

    // Update hole occupation tracking for next turn
    // Check if any holes are currently occupied
    curr_holeA_occupied = 0;
    curr_holeB_occupied = 0;
    for (i = 0; i < game_state.num_players; i++) {
        if (game_state.players[i].under == TILE_HOLE_A) curr_holeA_occupied = 1;
        if (game_state.players[i].under == TILE_HOLE_B) curr_holeB_occupied = 1;
    }
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].under == TILE_HOLE_A) curr_holeA_occupied = 1;
        if (game_state.objects[i].under == TILE_HOLE_B) curr_holeB_occupied = 1;
    }
    prev_holeA_occupied = curr_holeA_occupied;
    prev_holeB_occupied = curr_holeB_occupied;
}

/*
  Check if enemy can see player in a straight line (line-of-sight)
  Returns 1 if line-of-sight exists, 0 otherwise
  Sets dx/dy to movement direction if line-of-sight exists
*/
byte has_line_of_sight(byte enemy_x, byte enemy_y, byte player_x, byte player_y, signed char* dx, signed char* dy) {
    byte x, y;
    char tile;

    *dx = 0;
    *dy = 0;

    // Check vertical line-of-sight
    if (enemy_x == player_x) {
        if (enemy_y < player_y) {
            // Check path downward
            for (y = enemy_y + 1; y < player_y; y++) {
                tile = get_tile(enemy_x, y);
                // enemySeen = enemy or walls or door or gateA_closed or gateB_closed
                if (!is_passable(tile) || tile == TILE_ENEMY || tile == TILE_DOOR ||
                    tile == TILE_GATE_A || tile == TILE_GATE_B) {
                    return 0;  // Path blocked
                }
            }
            *dy = 1;  // Move down
            return 1;
        } else if (enemy_y > player_y) {
            // Check path upward
            for (y = player_y + 1; y < enemy_y; y++) {
                tile = get_tile(enemy_x, y);
                if (!is_passable(tile) || tile == TILE_ENEMY || tile == TILE_DOOR ||
                    tile == TILE_GATE_A || tile == TILE_GATE_B) {
                    return 0;  // Path blocked
                }
            }
            *dy = -1;  // Move up
            return 1;
        }
    }

    // Check horizontal line-of-sight
    if (enemy_y == player_y) {
        if (enemy_x < player_x) {
            // Check path rightward
            for (x = enemy_x + 1; x < player_x; x++) {
                tile = get_tile(x, enemy_y);
                if (!is_passable(tile) || tile == TILE_ENEMY || tile == TILE_DOOR ||
                    tile == TILE_GATE_A || tile == TILE_GATE_B) {
                    return 0;  // Path blocked
                }
            }
            *dx = 1;  // Move right
            return 1;
        } else if (enemy_x > player_x) {
            // Check path leftward
            for (x = player_x + 1; x < enemy_x; x++) {
                tile = get_tile(x, enemy_y);
                if (!is_passable(tile) || tile == TILE_ENEMY || tile == TILE_DOOR ||
                    tile == TILE_GATE_A || tile == TILE_GATE_B) {
                    return 0;  // Path blocked
                }
            }
            *dx = -1;  // Move left
            return 1;
        }
    }

    return 0;  // No line-of-sight
}

/*
  Move all enemies toward players using line-of-sight
  Called after player movement. Matches PuzzleScript behavior.
  Enemies move ALL THE WAY to the player or until blocked (simulates "again" rule).
  After killing a player, enemy checks again for more players to kill (chain kills).
*/
void move_enemies(void) {
    byte i, j, k;
    byte enemy_x, enemy_y;
    signed char dx, dy;
    byte new_x, new_y;
    char new_tile;
    byte can_see;
    byte player_caught;
    byte keep_checking;
    char tile_under_player;

    // Process each enemy
    for (i = 0; i < game_state.num_objects; i++) {
        if (game_state.objects[i].type != TILE_ENEMY) {
            continue;  // Skip non-enemy objects
        }

        enemy_x = game_state.objects[i].x;
        enemy_y = game_state.objects[i].y;

        // Keep checking for players until no more are visible
        keep_checking = 1;
        while (keep_checking) {
            keep_checking = 0;  // Assume no more players visible

            // Check line-of-sight to any player
            can_see = 0;
            for (j = 0; j < game_state.num_players; j++) {
                if (has_line_of_sight(enemy_x, enemy_y,
                                      game_state.players[j].x, game_state.players[j].y,
                                      &dx, &dy)) {
                    can_see = 1;
                    break;  // Found a player in line-of-sight
                }
            }

            // If enemy can see a player, move ALL THE WAY toward them (simulates "again")
            if (can_see && (dx != 0 || dy != 0)) {
                // Keep moving until blocked or reach player
                while (1) {
                    new_x = enemy_x + dx;
                    new_y = enemy_y + dy;

                    // Check bounds
                    if (new_x >= MAX_LEVEL_WIDTH || new_y >= MAX_LEVEL_HEIGHT) {
                        break;
                    }

                    new_tile = get_tile(new_x, new_y);

                    // Check if there's a player at this position
                    player_caught = 0;
                    for (k = 0; k < game_state.num_players; k++) {
                        if (game_state.players[k].x == new_x && game_state.players[k].y == new_y) {
                            player_caught = 1;

                            // Save what was under the player (not the player itself!)
                            tile_under_player = game_state.players[k].under;

                            // Remove the caught player (like disappearing in duplication)
                            // Shift remaining players down
                            for (j = k; j < game_state.num_players - 1; j++) {
                                game_state.players[j] = game_state.players[j + 1];
                                prev_player_under[j] = prev_player_under[j + 1];
                            }
                            game_state.num_players--;

                            // Move enemy to player's position
                            set_tile_and_draw(enemy_x, enemy_y, game_state.objects[i].under);
                            enemy_x = new_x;
                            enemy_y = new_y;
                            game_state.objects[i].x = new_x;
                            game_state.objects[i].y = new_y;
                            game_state.objects[i].under = tile_under_player;  // Store what was under player
                            set_tile_and_draw(new_x, new_y, TILE_ENEMY);

                            // Check if all players are dead
                            if (game_state.num_players == 0) {
                                game_state.level_complete = 2;  // Level failed
                                return;
                            }

                            // After killing a player, check again for more players
                            keep_checking = 1;

                            break;  // Stop moving this direction
                        }
                    }

                    if (player_caught) {
                        break;  // Enemy reached player, check for more players
                    }

                    // enemystopper = crate or key or enemy or walls or door or gateA_closed or gateB_closed
                    // Check if blocked by enemystopper
                    if (!is_passable(new_tile) || new_tile == TILE_CRATE || new_tile == TILE_KEY ||
                        new_tile == TILE_ENEMY || new_tile == TILE_DOOR ||
                        new_tile == TILE_GATE_A || new_tile == TILE_GATE_B) {
                        break;  // Blocked, stop moving
                    }

                    // Move enemy one step
                    set_tile_and_draw(enemy_x, enemy_y, game_state.objects[i].under);
                    enemy_x = new_x;
                    enemy_y = new_y;
                    game_state.objects[i].x = new_x;
                    game_state.objects[i].y = new_y;
                    game_state.objects[i].under = new_tile;
                    set_tile_and_draw(new_x, new_y, TILE_ENEMY);
                }
            }
        }
        // If no line-of-sight, enemy doesn't move
    }
}

/*
  Try to push an object at a position in a direction
  Handles chain pushing by checking the entire chain first
*/
byte try_push(byte x, byte y, signed char dx, signed char dy) {
    byte i, j;
    byte check_x, check_y;
    byte chain_length = 0;
    byte end_x, end_y;
    char check_tile;
    char end_tile;
    char tile_under_key;

    // Find the length of the chain
    // Start at the first object and walk forward
    check_x = x;
    check_y = y;

    while (chain_length < 10) {  // Max chain length of 10
        byte next_x = check_x + dx;
        byte next_y = check_y + dy;

        // Check bounds
        if (next_x >= MAX_LEVEL_WIDTH || next_y >= MAX_LEVEL_HEIGHT) {
            return 0;  // Can't push out of bounds
        }

        check_tile = get_tile(next_x, next_y);

        // If the next tile is pushable, it's part of the chain
        if (is_pushable(check_tile)) {
            chain_length++;
            check_x = next_x;
            check_y = next_y;
        }
        // If the next tile is passable or a door, we found the end
        else if (is_passable(check_tile) || check_tile == TILE_DOOR) {
            // Found the end of the chain
            end_x = next_x;
            end_y = next_y;
            end_tile = check_tile;
            break;
        }
        // Otherwise, blocked
        else {
            return 0;
        }
    }

    // Special case: If the END of the chain is hitting a door with a key
    if (end_tile == TILE_DOOR) {
        // Find the last object in the chain (the one that will hit the door)
        byte last_obj_x = x + dx * chain_length;
        byte last_obj_y = y + dy * chain_length;
        char last_obj_tile = get_tile(last_obj_x, last_obj_y);

        if (last_obj_tile == TILE_KEY) {
            // Remove the key that's hitting the door
            // Use background_map to get the correct tile under the key
            tile_under_key = background_map[last_obj_y][last_obj_x];

            for (i = 0; i < game_state.num_objects; i++) {
                if (game_state.objects[i].x == last_obj_x && game_state.objects[i].y == last_obj_y) {
                    // Remove this object by shifting BOTH arrays
                    for (j = i; j < game_state.num_objects - 1; j++) {
                        game_state.objects[j] = game_state.objects[j + 1];
                        prev_object_under[j] = prev_object_under[j + 1];
                    }
                    game_state.num_objects--;
                    break;
                }
            }

            // Open the door and restore the tile that was under the key
            handle_key_door(last_obj_x, last_obj_y, end_x, end_y, tile_under_key);

            // Now push the remaining objects in the chain (if any)
            if (chain_length > 0) {
                for (i = chain_length - 1; i < 255; i--) {  // Count down (byte wraps at 0)
                    byte obj_x = x + dx * i;
                    byte obj_y = y + dy * i;
                    byte new_x = obj_x + dx;
                    byte new_y = obj_y + dy;

                    // Find the object at this position and move it
                    for (j = 0; j < game_state.num_objects; j++) {
                        if (game_state.objects[j].x == obj_x && game_state.objects[j].y == obj_y) {
                            char tile_to_restore = game_state.objects[j].under;
                            char obj_type = game_state.objects[j].type;
                            char new_under = get_tile(new_x, new_y);

                            // Update object position
                            game_state.objects[j].x = new_x;
                            game_state.objects[j].y = new_y;
                            game_state.objects[j].under = new_under;

                            // Update map
                            set_tile_and_draw(obj_x, obj_y, tile_to_restore);
                            set_tile_and_draw(new_x, new_y, obj_type);
                            break;
                        }
                    }

                    if (i == 0) break;  // Prevent underflow
                }
            }

            return 1;
        } else {
            // Non-key object hitting door - can't push
            return 0;
        }
    }

    // Normal push (no door involved) - push all objects from back to front
    for (i = chain_length; i < 255; i--) {  // Count down (byte wraps at 0)
        byte obj_x = x + dx * i;
        byte obj_y = y + dy * i;
        byte new_x = obj_x + dx;
        byte new_y = obj_y + dy;

        // Find the object at this position and move it
        for (j = 0; j < game_state.num_objects; j++) {
            if (game_state.objects[j].x == obj_x && game_state.objects[j].y == obj_y) {
                char tile_to_restore = game_state.objects[j].under;
                char obj_type = game_state.objects[j].type;
                char new_under = get_tile(new_x, new_y);

                // Update object position
                game_state.objects[j].x = new_x;
                game_state.objects[j].y = new_y;
                game_state.objects[j].under = new_under;

                // Update map
                set_tile_and_draw(obj_x, obj_y, tile_to_restore);
                set_tile_and_draw(new_x, new_y, obj_type);
                break;
            }
        }

        if (i == 0) break;  // Prevent underflow
    }

    return 1;  // Push successful
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
        move_enemies();  // Move enemies after player moves
    }

    return moved;
}

byte is_level_complete(void) {
    return game_state.level_complete;
}

GameState* get_game_state(void) {
    return &game_state;
}

