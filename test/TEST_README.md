# Duplicator Game Test Environment

This directory contains a test environment that allows you to test the duplicator game logic on modern computers without Atari hardware.

## Overview

The test environment provides:
- **Pure C implementation** that runs on modern computers (Linux, macOS, Windows with gcc)
- **Mock console I/O library** that simulates Atari screen functions
- **Automated test framework** for programmatic testing of game logic
- **No modifications to game logic** - `duplicator_game.c` and `duplicator_game.h` remain unchanged

## Files

### Test Environment Files
- **test_conio.h** - Mock console I/O library header (replaces atari_conio.h)
- **test_conio.c** - Implementation of mock console I/O functions
- **duplicator_test_runner.c** - Test runner with automated test cases
- **build_test.sh** - Build script for compiling tests with gcc

### Original Game Files (Unchanged)
- **duplicator.c** - Main Atari game file (still works with Atari hardware)
- **duplicator_game.c** - Game logic (used by both Atari and test versions)
- **duplicator_game.h** - Game constants and structures (used by both versions)

## Building and Running Tests

### Prerequisites
- gcc compiler
- Standard C library

### Build and Run
```bash
# Make the build script executable (first time only)
chmod +x build_test.sh

# Build and run tests
./build_test.sh && ./duplicator_test
```

### Build Only
```bash
./build_test.sh
```

### Run Tests Only
```bash
./duplicator_test
```

## How It Works

The test environment uses gcc's `-include` flag to force `test_conio.h` to be included before `atari_conio.h`:

```bash
gcc -include test_conio.h -o duplicator_test \
    test_conio.c \
    duplicator_game.c \
    duplicator_test_runner.c
```

This allows the game logic to use the mock console I/O functions instead of the real Atari hardware functions, without modifying any game code.

## Writing Tests

### Test Structure

Each test follows this pattern:

```c
void test_example(void) {
    // 1. Load a test level
    load_level(test_level_data, num_rows);
    draw_level();
    
    // 2. Get game state
    GameState* state = get_game_state();
    
    // 3. Execute moves
    execute_moves("r u l d");  // right, up, left, down
    
    // 4. Verify results
    assert(state->players[0].x == expected_x);
    assert(state->players[0].y == expected_y);
    
    printf("✓ TEST PASSED\n");
}
```

### Available Helper Functions

#### `execute_moves(const char* moves)`
Executes a sequence of moves:
- `'r'` - Move right
- `'l'` - Move left
- `'u'` - Move up
- `'d'` - Move down
- `' '` - No move (process game logic)

Example:
```c
execute_moves("r r u l");  // right, right, up, left
```

#### `print_game_state()`
Displays current game state:
- Number of players and their positions
- Number of objects and their positions
- Level complete status

#### `print_level()`
Displays the visual level representation (what's on screen)

#### `get_game_state()`
Returns pointer to the current game state structure

#### `get_tile(byte x, byte y)`
Returns the tile character at position (x, y)

### Test Level Format

Test levels are defined as string arrays:

```c
const char* test_level[] = {
    "########:########",  // Row 0 (exit at top)
    "########.########",  // Row 1
    "#######...#######",  // Row 2
    "#######.p.#######",  // Row 3 (player in center)
    "#######...#######",  // Row 4
    "#################"   // Row 5
};
```

Each string represents one row. The `:` separator is used to split the level data.

## Current Test Cases

### 1. Simple Movement
Tests basic player movement in all 4 directions.

### 2. Key and Door Interaction
Tests pushing keys and door mechanics.

### 3. Hole Duplication
Tests the duplication mechanic:
- Player enters hole A
- Duplicate appears in hole B
- Both players move simultaneously

### 4. Gate and Pressure Plate
Tests gate/plate interaction:
- Player steps on pressure plate
- Gate opens
- Player steps off plate
- Gate closes

## Adding New Tests

1. Define your test level data:
```c
const char* my_test_level[] = {
    "########:########",
    "#######.p.#######",
    "#################"
};
```

2. Write your test function:
```c
void test_my_feature(void) {
    printf("\n\n========================================\n");
    printf("TEST: My Feature\n");
    printf("========================================\n");
    
    load_level(my_test_level, 3);
    draw_level();
    
    GameState* state = get_game_state();
    
    // Your test logic here
    execute_moves("r r u");
    assert(state->players[0].x == expected_x);
    
    printf("\n✓ TEST PASSED: My Feature\n");
}
```

3. Add your test to `main()`:
```c
int main(void) {
    test_simple_movement();
    test_key_door();
    test_hole_duplication();
    test_gate_plate();
    test_my_feature();  // Add your test here
    
    printf("\n========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("========================================\n");
    
    return 0;
}
```

4. Rebuild and run:
```bash
./build_test.sh && ./duplicator_test
```

## Debugging

### View Screen Buffer
The test environment maintains a screen buffer that you can inspect:

```c
print_level();  // Shows the current screen state
```

### View Game State
```c
print_game_state();  // Shows players, objects, level status
```

### Get Specific Tile
```c
char tile = get_tile(x, y);
printf("Tile at (%d, %d) is '%c'\n", x, y, tile);
```

## Limitations

- No graphics - text-only output
- No joystick input - tests use programmatic moves
- No sound
- No timing/animation - instant execution
- Screen buffer is 40x24 (same as Atari)

## Benefits

✅ **Fast iteration** - No need to compile for Atari and run in emulator  
✅ **Automated testing** - Verify game logic programmatically  
✅ **Easy debugging** - Use standard C debuggers (gdb, lldb)  
✅ **No hardware needed** - Test on any modern computer  
✅ **Regression testing** - Catch bugs before deploying to Atari  
✅ **Clean separation** - Game logic unchanged, works on both platforms  

## Atari Version

The original Atari version (`duplicator.c`) remains completely unchanged and continues to work with Atari hardware. The test environment is a parallel implementation that shares the same game logic.

To build for Atari, use the original build process (8bitworkshop or cc65 toolchain).

