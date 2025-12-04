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

// Test level: 3 players in horizontal line (start with 1, we'll add 2 more manually)
const char* test_level_three_players[] = {
    "########:########",
    "########.########",
    "#####.......#####",
    "#####...p...#####",
    "#####.......#####",
    "#################"
};

// Test level: 3 players + 2 keys in horizontal line (all movable objects)
const char* test_level_players_and_keys[] = {
    "########:########",
    "########.########",
    "#####.........###",
    "#####...p.....###",
    "#####.........###",
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

// Test case: 3 players in horizontal line (should expose bug)
void test_three_players_horizontal(void) {
    printf("\n\n========================================\n");
    printf("TEST: Three Players in Horizontal Line\n");
    printf("========================================\n");
    printf("This test should FAIL due to a bug where not all 3 players move together\n");

    load_level(test_level_three_players, 6);

    GameState* state = get_game_state();

    // Manually add 2 more players to create a horizontal line of 3
    // Player 0 is at (8, 3) from level load
    // Add player 1 at (7, 3) - left of player 0
    state->players[1].x = 7;
    state->players[1].y = 3;
    state->players[1].under = TILE_FLOOR;
    state->num_players = 2;

    // Add player 2 at (9, 3) - right of player 0
    state->players[2].x = 9;
    state->players[2].y = 3;
    state->players[2].under = TILE_FLOOR;
    state->num_players = 3;

    // Draw all players on the map
    set_tile(7, 3, TILE_PLAYER);
    set_tile(8, 3, TILE_PLAYER);
    set_tile(9, 3, TILE_PLAYER);

    draw_level();

    printf("\nInitial state (3 players in horizontal line):");
    print_level();
    print_game_state();

    // Verify we have 3 players
    assert(state->num_players == 3);
    printf("✓ Have 3 players as expected\n");

    // Record initial positions
    byte initial_x0 = state->players[0].x;
    byte initial_x1 = state->players[1].x;
    byte initial_x2 = state->players[2].x;
    byte initial_y = state->players[0].y;

    printf("\nInitial positions:\n");
    printf("  Player 0: (%d, %d)\n", initial_x0, initial_y);
    printf("  Player 1: (%d, %d)\n", initial_x1, initial_y);
    printf("  Player 2: (%d, %d)\n", initial_x2, initial_y);

    // Move right - all 3 players should move
    printf("\nMoving RIGHT (all 3 players should move together)...");
    execute_moves("r");

    printf("\nAfter moving right:");
    print_game_state();
    print_level();

    // Check if all 3 players moved
    byte moved_0 = (state->players[0].x == initial_x0 + 1);
    byte moved_1 = (state->players[1].x == initial_x1 + 1);
    byte moved_2 = (state->players[2].x == initial_x2 + 1);

    printf("\nMovement check:\n");
    printf("  Player 0: %s (from %d to %d)\n", moved_0 ? "MOVED" : "STUCK", initial_x0, state->players[0].x);
    printf("  Player 1: %s (from %d to %d)\n", moved_1 ? "MOVED" : "STUCK", initial_x1, state->players[1].x);
    printf("  Player 2: %s (from %d to %d)\n", moved_2 ? "MOVED" : "STUCK", initial_x2, state->players[2].x);

    // This assertion should FAIL if there's a bug
    if (!moved_0 || !moved_1 || !moved_2) {
        printf("\n❌ BUG DETECTED: Not all 3 players moved!\n");
        printf("   Expected: All 3 players move together\n");
        printf("   Actual: Only %d player(s) moved\n", moved_0 + moved_1 + moved_2);
        assert(0 && "BUG: Not all 3 players moved together!");
    }

    // Update positions for next test
    initial_x0 = state->players[0].x;
    initial_x1 = state->players[1].x;
    initial_x2 = state->players[2].x;

    // Move left - all 3 players should move
    printf("\nMoving LEFT (all 3 players should move together)...");
    execute_moves("l");

    printf("\nAfter moving left:");
    print_game_state();
    print_level();

    // Check if all 3 players moved
    moved_0 = (state->players[0].x == initial_x0 - 1);
    moved_1 = (state->players[1].x == initial_x1 - 1);
    moved_2 = (state->players[2].x == initial_x2 - 1);

    printf("\nMovement check:\n");
    printf("  Player 0: %s (from %d to %d)\n", moved_0 ? "MOVED" : "STUCK", initial_x0, state->players[0].x);
    printf("  Player 1: %s (from %d to %d)\n", moved_1 ? "MOVED" : "STUCK", initial_x1, state->players[1].x);
    printf("  Player 2: %s (from %d to %d)\n", moved_2 ? "MOVED" : "STUCK", initial_x2, state->players[2].x);

    // This assertion should FAIL if there's a bug
    if (!moved_0 || !moved_1 || !moved_2) {
        printf("\n❌ BUG DETECTED: Not all 3 players moved!\n");
        printf("   Expected: All 3 players move together\n");
        printf("   Actual: Only %d player(s) moved\n", moved_0 + moved_1 + moved_2);
        assert(0 && "BUG: Not all 3 players moved together!");
    }

    printf("\n✓ TEST PASSED: Three Players in Horizontal Line\n");
}

// Test case: 3 players + 2 keys in horizontal line (all should move together)
void test_players_and_keys_line(void) {
    printf("\n\n========================================\n");
    printf("TEST: Players and Keys in Horizontal Line\n");
    printf("========================================\n");
    printf("Testing: p k p k p (all movable objects should move together)\n");

    load_level(test_level_players_and_keys, 6);

    GameState* state = get_game_state();

    // Manually create the line: p k p k p
    // Player 0 at (8, 3) from level load
    // Key 0 at (9, 3)
    state->objects[0].x = 9;
    state->objects[0].y = 3;
    state->objects[0].type = TILE_KEY;
    state->objects[0].under = TILE_FLOOR;
    state->num_objects = 1;

    // Player 1 at (10, 3)
    state->players[1].x = 10;
    state->players[1].y = 3;
    state->players[1].under = TILE_FLOOR;
    state->num_players = 2;

    // Key 1 at (11, 3)
    state->objects[1].x = 11;
    state->objects[1].y = 3;
    state->objects[1].type = TILE_KEY;
    state->objects[1].under = TILE_FLOOR;
    state->num_objects = 2;

    // Player 2 at (12, 3)
    state->players[2].x = 12;
    state->players[2].y = 3;
    state->players[2].under = TILE_FLOOR;
    state->num_players = 3;

    // Draw all objects on the map
    set_tile(8, 3, TILE_PLAYER);   // p
    set_tile(9, 3, TILE_KEY);      // k
    set_tile(10, 3, TILE_PLAYER);  // p
    set_tile(11, 3, TILE_KEY);     // k
    set_tile(12, 3, TILE_PLAYER);  // p

    draw_level();

    printf("\nInitial state (p k p k p in horizontal line):");
    print_level();
    print_game_state();

    // Verify we have 3 players and 2 keys
    assert(state->num_players == 3);
    assert(state->num_objects == 2);
    printf("✓ Have 3 players and 2 keys as expected\n");

    // Record initial positions
    byte initial_p0_x = state->players[0].x;
    byte initial_p1_x = state->players[1].x;
    byte initial_p2_x = state->players[2].x;
    byte initial_k0_x = state->objects[0].x;
    byte initial_k1_x = state->objects[1].x;

    printf("\nInitial positions:\n");
    printf("  Player 0: (%d, 3)\n", initial_p0_x);
    printf("  Key 0:    (%d, 3)\n", initial_k0_x);
    printf("  Player 1: (%d, 3)\n", initial_p1_x);
    printf("  Key 1:    (%d, 3)\n", initial_k1_x);
    printf("  Player 2: (%d, 3)\n", initial_p2_x);

    // Move right - all 5 objects should move
    printf("\nMoving RIGHT (all 5 objects should move together)...");
    execute_moves("r");

    printf("\nAfter moving right:");
    print_game_state();
    print_level();

    // Check if all objects moved
    byte p0_moved = (state->players[0].x == initial_p0_x + 1);
    byte p1_moved = (state->players[1].x == initial_p1_x + 1);
    byte p2_moved = (state->players[2].x == initial_p2_x + 1);
    byte k0_moved = (state->objects[0].x == initial_k0_x + 1);
    byte k1_moved = (state->objects[1].x == initial_k1_x + 1);

    printf("\nMovement check:\n");
    printf("  Player 0: %s (from %d to %d)\n", p0_moved ? "MOVED" : "STUCK", initial_p0_x, state->players[0].x);
    printf("  Key 0:    %s (from %d to %d)\n", k0_moved ? "MOVED" : "STUCK", initial_k0_x, state->objects[0].x);
    printf("  Player 1: %s (from %d to %d)\n", p1_moved ? "MOVED" : "STUCK", initial_p1_x, state->players[1].x);
    printf("  Key 1:    %s (from %d to %d)\n", k1_moved ? "MOVED" : "STUCK", initial_k1_x, state->objects[1].x);
    printf("  Player 2: %s (from %d to %d)\n", p2_moved ? "MOVED" : "STUCK", initial_p2_x, state->players[2].x);

    // This assertion should pass if the fix works
    if (!p0_moved || !p1_moved || !p2_moved || !k0_moved || !k1_moved) {
        printf("\n❌ BUG DETECTED: Not all objects moved!\n");
        printf("   Expected: All 5 objects move together\n");
        printf("   Actual: Only %d object(s) moved\n", p0_moved + p1_moved + p2_moved + k0_moved + k1_moved);
        assert(0 && "BUG: Not all objects moved together!");
    }

    // Update positions for next test
    initial_p0_x = state->players[0].x;
    initial_p1_x = state->players[1].x;
    initial_p2_x = state->players[2].x;
    initial_k0_x = state->objects[0].x;
    initial_k1_x = state->objects[1].x;

    // Move left - all 5 objects should move
    printf("\nMoving LEFT (all 5 objects should move together)...");
    execute_moves("l");

    printf("\nAfter moving left:");
    print_game_state();
    print_level();

    // Check if all objects moved
    p0_moved = (state->players[0].x == initial_p0_x - 1);
    p1_moved = (state->players[1].x == initial_p1_x - 1);
    p2_moved = (state->players[2].x == initial_p2_x - 1);
    k0_moved = (state->objects[0].x == initial_k0_x - 1);
    k1_moved = (state->objects[1].x == initial_k1_x - 1);

    printf("\nMovement check:\n");
    printf("  Player 0: %s (from %d to %d)\n", p0_moved ? "MOVED" : "STUCK", initial_p0_x, state->players[0].x);
    printf("  Key 0:    %s (from %d to %d)\n", k0_moved ? "MOVED" : "STUCK", initial_k0_x, state->objects[0].x);
    printf("  Player 1: %s (from %d to %d)\n", p1_moved ? "MOVED" : "STUCK", initial_p1_x, state->players[1].x);
    printf("  Key 1:    %s (from %d to %d)\n", k1_moved ? "MOVED" : "STUCK", initial_k1_x, state->objects[1].x);
    printf("  Player 2: %s (from %d to %d)\n", p2_moved ? "MOVED" : "STUCK", initial_p2_x, state->players[2].x);

    // This assertion should pass if the fix works
    if (!p0_moved || !p1_moved || !p2_moved || !k0_moved || !k1_moved) {
        printf("\n❌ BUG DETECTED: Not all objects moved!\n");
        printf("   Expected: All 5 objects move together\n");
        printf("   Actual: Only %d object(s) moved\n", p0_moved + p1_moved + p2_moved + k0_moved + k1_moved);
        assert(0 && "BUG: Not all objects moved together!");
    }

    printf("\n✓ TEST PASSED: Players and Keys in Horizontal Line\n");
}

void test_key_pushed_off_hole(void) {
    GameState* state;
    byte i;

    printf("========================================\n");
    printf("TEST: Key Pushed OFF Hole (No Duplication)\n");
    printf("========================================\n");
    printf("Bug: Key on hole A should NOT duplicate when player pushes key off hole B\n\n");

    // Level: Simple level with two holes and space
    const char* test_level[] = {
        "##########",
        "#.?...!..#",
        "#........#",
        "#........#",
        "##########"
    };

    load_level(test_level, 5);
    state = get_game_state();

    // Manually place keys on the holes and player below hole B
    state->objects[0].x = 2;
    state->objects[0].y = 1;
    state->objects[0].type = TILE_KEY;
    state->objects[0].under = TILE_HOLE_A;
    set_tile(2, 1, TILE_KEY);

    state->objects[1].x = 6;
    state->objects[1].y = 1;
    state->objects[1].type = TILE_KEY;
    state->objects[1].under = TILE_HOLE_B;
    set_tile(6, 1, TILE_KEY);

    state->num_objects = 2;

    // Place player directly below hole B
    state->players[0].x = 6;
    state->players[0].y = 2;
    state->players[0].under = TILE_FLOOR;
    set_tile(6, 2, TILE_PLAYER);

    // Reset duplication tracking so keys already on holes don't trigger duplication
    reset_duplication_tracking();

    printf("Initial state:\n");
    printf("  Player at (%d, %d)\n", state->players[0].x, state->players[0].y);
    printf("  Key 0 at (%d, %d) under='%c' (on hole A)\n",
           state->objects[0].x, state->objects[0].y, state->objects[0].under);
    printf("  Key 1 at (%d, %d) under='%c' (on hole B)\n",
           state->objects[1].x, state->objects[1].y, state->objects[1].under);
    printf("  Hole A at (2, 1), Hole B at (6, 1)\n");
    printf("  Number of keys: %d\n\n", state->num_objects);

    // Move 1: Player moves UP (pushes key OFF hole B, player moves onto hole B)
    printf("Move 1: Player UP (pushes key 1 OFF hole B)\n");
    execute_moves("U");
    printf("  Player at (%d, %d) under='%c'\n",
           state->players[0].x, state->players[0].y, state->players[0].under);

    // Print all keys
    printf("  Keys:\n");
    for (i = 0; i < state->num_objects; i++) {
        if (state->objects[i].type == TILE_KEY) {
            printf("    Key %d at (%d, %d) under='%c'\n",
                   i, state->objects[i].x, state->objects[i].y, state->objects[i].under);
        }
    }
    printf("  Number of keys: %d\n\n", state->num_objects);

    // Move 2: Player moves DOWN (moves OFF hole B)
    printf("Move 2: Player DOWN (moves OFF hole B)\n");
    printf("  Before move: %d keys\n", state->num_objects);
    execute_moves("D");
    printf("  After move:\n");
    printf("    Player at (%d, %d) under='%c'\n",
           state->players[0].x, state->players[0].y, state->players[0].under);

    // Print all keys
    printf("    Keys:\n");
    for (i = 0; i < state->num_objects; i++) {
        if (state->objects[i].type == TILE_KEY) {
            printf("      Key %d at (%d, %d) under='%c'\n",
                   i, state->objects[i].x, state->objects[i].y, state->objects[i].under);
        }
    }
    printf("    Number of keys: %d\n", state->num_objects);

    // Check: Key should NOT be duplicated
    // Key 0 is still on hole A (didn't move)
    // Key 1 is on floor (was pushed off hole B)
    // Player left hole B, so hole B is now empty
    // But duplication should only happen when object ENTERS a hole, not when it stays on it
    if (state->num_objects != 2) {
        printf("\n❌ BUG DETECTED: Key duplicated even though no key entered a hole!\n");
        printf("   Expected: 2 keys (key 0 stayed on hole A, key 1 on floor)\n");
        printf("   Actual: %d keys\n", state->num_objects);
        printf("   Key 0 was already on hole A and didn't move.\n");
        printf("   Duplication should only happen when object ENTERS a hole!\n");
        assert(0 && "BUG: Key duplicated when staying on hole!");
    }

    printf("  ✓ Correct: Keys were NOT duplicated (no key entered a hole)\n\n");

    printf("✓ TEST PASSED: Key Pushed OFF Hole\n\n");
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
    test_three_players_horizontal();
    test_players_and_keys_line();  // Test mixed players and keys
    test_key_pushed_off_hole();  // Test duplication only on entry

    printf("\n\n========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("========================================\n");

    return 0;
}

