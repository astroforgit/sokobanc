/* satanic_game.c - Game state implementation */

#include "satanic_game.h"
#include "satanic_conio_16x16.h"
#include "satanic_levels_16x16.h"

// Static game state
static GameState game_state;

GameState* get_game_state(void) {
    return &game_state;
}

void init_game_state(void) {
    memset(&game_state, 0, sizeof(GameState));
    game_state.current_level = 0;
}

void load_level(byte level_num) {
    byte x, y;
    const char** level_data = levels[level_num];
    my_clrscr_16x16();

    game_state.current_level = level_num;
    game_state.level_width = 17;  // Adjust based on your levels
    game_state.level_height = 11; // Adjust based on your levels

    for (y = 0; y < game_state.level_height; y++) {
        for (x = 0; x < game_state.level_width; x++) {
            char tile = level_data[y][x];
            if (tile == 'p') {
                game_state.player_x = x;
                game_state.player_y = y;
                game_state.level_map[y][x] = ' ';
            } else {
                game_state.level_map[y][x] = tile;
                my_cputcxy_16x16(x, y, map_tile_to_16x16(tile));
            }
        }
    }
    my_cputcxy_16x16(game_state.player_x, game_state.player_y, map_tile_to_16x16('p'));
}

void try_move(signed char dx, signed char dy) {
    byte new_x = game_state.player_x + dx;
    byte new_y = game_state.player_y + dy;
    char tile = game_state.level_map[new_y][new_x];

    if (tile != '#') {
        my_cputcxy_16x16(game_state.player_x, game_state.player_y,
                         map_tile_to_16x16(game_state.level_map[game_state.player_y][game_state.player_x]));
        game_state.player_x = new_x;
        game_state.player_y = new_y;
        my_cputcxy_16x16(game_state.player_x, game_state.player_y, map_tile_to_16x16('p'));
        if (tile == '@') {
            game_state.current_level++;
            if (game_state.current_level >= NUM_LEVELS) game_state.current_level = 0;
            load_level(game_state.current_level);
        }
    }
}

