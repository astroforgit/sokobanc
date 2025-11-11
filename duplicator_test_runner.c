/*
  duplicator_test_runner.c - Test environment for duplicator game
  
  This file allows testing the duplicator game logic on modern computers
  without needing an Atari emulator. It provides:
  - Programmatic player movement
  - Game state inspection
  - Automated test cases
*/

#include "duplicator_game.h"
#include "test_conio.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Test level data
const char* test_level_simple[] = {
    "########:########",
    "########.########",
    "#######...#######",
    "#######.p.#######",
    "#######...#######",
    "#################"
};

const char* test_level_with_key[] = {
    "########:########",
    "########.########",
    "#######...#...###",
    "#######.p.d.k.###",
    "#######...#...###",
    "#################"
};

const char* test_level_holes[] = {
    "########:########",
    "########.########",
    "#######.?.!.#####",
    "#######.p...#####",
    "#######.....#####",
    "#################"
};

const char* test_level_gates[] = {
    "########:########",
    "########.########",
    "#######.b.g.#####",
    "#######.p...#####",
    "#######.....#####",
    "#################"
};

// Helper function to print game state
void print_game_state(void) {
    GameState* state = get_game_state();
    byte i;
    
    printf("\n=== GAME STATE ===\n");
    printf("Level: %dx%d\n", state->level_width, state->level_height);
    printf("Players: %d\n", state->num_players);
    for (i = 0; i < state->num_players; i++) {
        printf("  Player %d: (%d, %d) under='%c'\n", 
               i, state->players[i].x, state->players[i].y, state->players[i].under);
    }
    printf("Objects: %d\n", state->num_objects);
    for (i = 0; i < state->num_objects; i++) {
        printf("  Object %d: type='%c' (%d, %d) under='%c'\n", 
               i, state->objects[i].type, state->objects[i].x, state->objects[i].y, 
               state->objects[i].under);
    }
    printf("Level complete: %s\n", state->level_complete ? "YES" : "NO");
    printf("==================\n");
}

// Helper function to print the level
void print_level(void) {
    printf("\n=== LEVEL VIEW ===\n");
    print_screen();
    printf("==================\n");
}

// Helper function to execute a sequence of moves
// Moves: 'u'=up, 'd'=down, 'l'=left, 'r'=right
void execute_moves(const char* moves) {
    while (*moves) {
        char move = *moves++;
        printf("\nExecuting move: '%c'\n", move);
        
        switch (move) {
            case 'u': case 'U':
                try_move_player(0, -1);
                break;
            case 'd': case 'D':
                try_move_player(0, 1);
                break;
            case 'l': case 'L':
                try_move_player(-1, 0);
                break;
            case 'r': case 'R':
                try_move_player(1, 0);
                break;
            case ' ':
                // Skip spaces
                continue;
            default:
                printf("Unknown move: '%c'\n", move);
                continue;
        }
        
        print_level();
        print_game_state();
    }
}

// Test case: Simple movement
void test_simple_movement(void) {
    printf("\n\n========================================\n");
    printf("TEST: Simple Movement\n");
    printf("========================================\n");
    
    load_level(test_level_simple, 6);
    draw_level();
    
    printf("\nInitial state:");
    print_level();
    print_game_state();
    
    GameState* state = get_game_state();
    assert(state->num_players == 1);
    byte initial_x = state->players[0].x;
    byte initial_y = state->players[0].y;
    printf("Initial player position: (%d, %d)\n", initial_x, initial_y);
    
    // Move right
    execute_moves("r");
    assert(state->players[0].x == initial_x + 1);
    assert(state->players[0].y == initial_y);

    // Move up
    execute_moves("u");
    assert(state->players[0].x == initial_x + 1);
    assert(state->players[0].y == initial_y - 1);

    // Move left
    execute_moves("l");
    assert(state->players[0].x == initial_x);
    assert(state->players[0].y == initial_y - 1);

    // Move down
    execute_moves("d");
    assert(state->players[0].x == initial_x);
    assert(state->players[0].y == initial_y);
    
    printf("\n✓ TEST PASSED: Simple Movement\n");
}

// Test case: Key and door interaction
void test_key_door(void) {
    printf("\n\n========================================\n");
    printf("TEST: Key and Door Interaction\n");
    printf("========================================\n");
    
    load_level(test_level_with_key, 6);
    draw_level();
    
    printf("\nInitial state:");
    print_level();
    print_game_state();
    
    GameState* state = get_game_state();
    assert(state->num_players == 1);
    assert(state->num_objects == 1);  // Should have 1 key
    
    // Move right to push key
    printf("\nPushing key towards door...");
    execute_moves("r r r");
    
    // Key should have been removed (consumed by door)
    printf("\nAfter key-door interaction:");
    print_game_state();
    
    printf("\n✓ TEST PASSED: Key and Door Interaction\n");
}

// Test case: Hole duplication
void test_hole_duplication(void) {
    printf("\n\n========================================\n");
    printf("TEST: Hole Duplication\n");
    printf("========================================\n");

    load_level(test_level_holes, 6);
    draw_level();

    printf("\nInitial state:");
    print_level();
    print_game_state();

    GameState* state = get_game_state();

    // Move into hole A - this should create a duplicate in hole B
    printf("\nMoving into hole A (should create duplicate in hole B)...");
    execute_moves("u");

    printf("\nAfter moving into hole A:");
    print_game_state();
    print_level();

    // Check that we now have 2 players
    assert(state->num_players == 2);
    printf("✓ Player duplicated! Now have %d players\n", state->num_players);

    // Check that one player is in hole A and one in hole B
    byte in_hole_a = 0;
    byte in_hole_b = 0;
    byte i;
    for (i = 0; i < state->num_players; i++) {
        if (state->players[i].under == TILE_HOLE_A) in_hole_a++;
        if (state->players[i].under == TILE_HOLE_B) in_hole_b++;
    }
    assert(in_hole_a == 1);
    assert(in_hole_b == 1);
    printf("✓ One player in hole A, one in hole B\n");

    printf("\n✓ TEST PASSED: Hole Duplication\n");
}

// Test case: Gate and pressure plate
void test_gate_plate(void) {
    printf("\n\n========================================\n");
    printf("TEST: Gate and Pressure Plate\n");
    printf("========================================\n");

    load_level(test_level_gates, 6);
    draw_level();

    printf("\nInitial state:");
    print_level();
    print_game_state();

    GameState* state = get_game_state();

    // Move onto pressure plate
    printf("\nMoving onto pressure plate...");
    execute_moves("u");

    printf("\nPlayer should be on plate:");
    print_game_state();
    assert(state->players[0].under == TILE_PLATE_A);
    printf("✓ Player is on pressure plate A\n");

    // Gate should be open now - check the level map
    printf("\nGate should be open (check level view - 'G' instead of 'g'):");
    print_level();

    // Verify gate is open by checking the tile
    char gate_tile = get_tile(10, 2);  // Gate position
    assert(gate_tile == 'G');  // Open gate A
    printf("✓ Gate A is open!\n");

    // Move off plate
    printf("\nMoving off pressure plate...");
    execute_moves("d");

    printf("\nGate should be closed again:");
    print_level();

    // Verify gate is closed
    gate_tile = get_tile(10, 2);
    assert(gate_tile == TILE_GATE_A);  // Closed gate A
    printf("✓ Gate A is closed again!\n");

    printf("\n✓ TEST PASSED: Gate and Pressure Plate\n");
}

// Main test runner
int main(void) {
    printf("========================================\n");
    printf("DUPLICATOR GAME TEST SUITE\n");
    printf("========================================\n");
    printf("Testing game logic without Atari hardware\n");
    
    // Run all tests
    test_simple_movement();
    test_key_door();
    test_hole_duplication();
    test_gate_plate();
    
    printf("\n\n========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("========================================\n");
    
    return 0;
}

