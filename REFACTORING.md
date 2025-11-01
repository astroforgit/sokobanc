# Sokoban Refactoring Documentation

## Overview

The Sokoban game has been refactored from a single monolithic file into a modular structure with separate libraries for different concerns. This improves code organization, reusability, and maintainability.

## File Structure

```
presets/atari8-800/sokobanc/
├── sokoban.c           # Main game file (86 lines)
├── atari_conio.h       # Console I/O library header
├── atari_conio.c       # Console I/O library implementation
├── atari_font.h        # Font/graphics library header
├── atari_font.c        # Font/graphics library implementation
├── sokoban_game.h      # Game logic library header
├── sokoban_game.c      # Game logic library implementation
└── REFACTORING.md      # This file
```

## Libraries

### 1. atari_conio - Console I/O Library

**Purpose**: Provides custom screen manipulation functions that work directly with screen memory for better performance and control.

**Files**: `atari_conio.h`, `atari_conio.c`

**Functions**:
- `void my_clrscr(void)` - Clear the screen
- `void my_cputcxy(byte x, byte y, byte character)` - Put character at position
- `void my_cputsxy(byte x, byte y, const char* str)` - Put string at position
- `void my_cprintf_status(byte b, byte t, byte m)` - Print formatted status line
- `void wait_vblank(void)` - Wait for vertical blank (screen refresh)

**Memory Layout**:
- `SCREEN_MEM` at `0x9000` - Screen memory (40x24 characters)
- Direct memory access for fast rendering

**Key Features**:
- ASCII to ATASCII conversion
- Direct screen memory manipulation
- VBlank synchronization for smooth animation

---

### 2. atari_font - Font and Graphics Library

**Purpose**: Handles character set redefinition and custom graphics for game sprites and tiles.

**Files**: `atari_font.h`, `atari_font.c`

**Functions**:
- `void setup_graphics(void)` - Initialize custom graphics mode
- `void animate_player(void)` - Animate player character
- `byte get_anim_state(void)` - Get current animation state
- `void set_anim_state(byte state)` - Set animation state
- `byte get_frame_counter(void)` - Get frame counter
- `void set_frame_counter(byte counter)` - Set frame counter
- `void increment_frame_counter(void)` - Increment frame counter

**Memory Layout**:
- `CHARSET_MEM` at `0x7000` - Custom character set (1024 bytes)
- `DLIST_MEM` at `0x8000` - Display list
- `ROM_CHARSET_ADDRESS` at `0xE000` - ROM character set (source)

**Custom Graphics**:
- Wall (`#`) - Brick pattern
- Box (`$`) - Box with border
- Goal (`.`) - Circle target
- Box on Goal (`*`) - Box with target inside
- Player (`@`) - Stick figure with 3 animation frames:
  - Standing (default)
  - Blinking (idle animation)
  - Walking (movement animation)

**Animation System**:
- Frame-based animation using character set redefinition
- Walking animation: 8 frames
- Blinking animation: Triggers every ~50 frames
- Updates both `@` and `+` characters (player on empty/goal)

---

### 3. sokoban_game - Game Logic Library

**Purpose**: Contains all game logic including level loading, drawing, player movement, and win condition checking.

**Files**: `sokoban_game.h`, `sokoban_game.c`

**Functions**:
- `void load_level(const char** d, byte r_num)` - Load level from string array
- `void draw_level(void)` - Draw entire level to screen
- `void update_status(void)` - Update status line
- `void try_move_player(signed char dx, signed char dy)` - Move player
- `byte get_player_x(void)` - Get player X position
- `byte get_player_y(void)` - Get player Y position
- `byte get_total_boxes(void)` - Get total boxes count
- `byte get_boxes_on_goals(void)` - Get boxes on goals count
- `byte get_moves(void)` - Get move count
- `byte is_level_complete(void)` - Check if level is complete

**Game State**:
- `level[ROWS][COLS]` - Level tile data
- `player_x`, `player_y` - Player position
- `total_boxes` - Total number of boxes in level
- `boxes_on_goals` - Number of boxes currently on goals
- `moves` - Number of moves made

**Tile Definitions**:
```c
#define TILE_EMPTY       0
#define TILE_WALL       '#'
#define TILE_GOAL       '.'
#define TILE_BOX        '$'
#define TILE_BOX_ON_GOAL '*'
#define TILE_PLAYER     '@'
#define TILE_PLAYER_ON_GOAL '+'
```

**Game Logic**:
- Wall collision detection
- Box pushing mechanics
- Goal detection and tracking
- Win condition checking (all boxes on goals)
- Move counter

---

## Main File: sokoban.c

The main file has been simplified to just 86 lines (down from 180 lines).

**Responsibilities**:
- Include necessary headers
- Define level data
- Initialize systems (joystick, graphics)
- Main game loop
- Input handling
- Win condition display

**Main Loop Flow**:
```
1. Read joystick input
2. Process movement (if joystick state changed)
3. Check win condition
4. Animate player character
5. Wait for vertical blank
6. Increment frame counter
7. Repeat
```

---

## Benefits of Refactoring

### Code Organization
- **Separation of Concerns**: Each library has a single, well-defined purpose
- **Modularity**: Libraries can be reused in other Atari 8-bit projects
- **Readability**: Main file is now easy to understand at a glance

### Maintainability
- **Easier Debugging**: Issues can be isolated to specific libraries
- **Easier Testing**: Each library can be tested independently
- **Easier Extension**: New features can be added to appropriate libraries

### Reusability
- **atari_conio**: Can be used in any Atari 8-bit project needing screen I/O
- **atari_font**: Can be adapted for other games needing custom graphics
- **sokoban_game**: Game logic is isolated and can be ported to other platforms

### Code Size
- **Main file**: 180 lines → 86 lines (52% reduction)
- **Total code**: Better organized across 7 files
- **Header files**: Provide clear API documentation

---

## Usage Example

```c
#include "atari_conio.h"
#include "atari_font.h"
#include "sokoban_game.h"

void main(void) {
    // Setup
    setup_graphics();
    load_level(level_data, num_rows);
    draw_level();
    update_status();
    
    // Game loop
    while (!is_level_complete()) {
        // Handle input
        // ...
        
        // Update animation
        animate_player();
        wait_vblank();
        increment_frame_counter();
    }
}
```

---

## Compilation

The refactored code should compile with CC65 just like the original:

```bash
# Using 8bitworkshop IDE
# Simply select sokoban.c and it will automatically compile all linked files

# Using CC65 command line
cc65 -I /share/include sokoban.c
cc65 -I /share/include atari_conio.c
cc65 -I /share/include atari_font.c
cc65 -I /share/include sokoban_game.c
ca65 sokoban.s
ca65 atari_conio.s
ca65 atari_font.s
ca65 sokoban_game.s
ld65 -C fastbasic.cfg -o sokoban.xex sokoban.o atari_conio.o atari_font.o sokoban_game.o
```

---

## Future Enhancements

With this modular structure, the following enhancements are easier to implement:

### atari_conio Library
- Add scrolling support
- Add text window functions
- Add color manipulation functions

### atari_font Library
- Support for multiple character sets
- More animation frames
- Sprite multiplexing

### sokoban_game Library
- Multiple levels
- Level editor
- Undo functionality
- Save/load game state
- High score tracking

---

## API Reference

### atari_conio.h

```c
void my_clrscr(void);
void my_cputcxy(byte x, byte y, byte character);
void my_cputsxy(byte x, byte y, const char* str);
void my_cprintf_status(byte b, byte t, byte m);
void wait_vblank(void);
```

### atari_font.h

```c
void setup_graphics(void);
void animate_player(void);
byte get_anim_state(void);
void set_anim_state(byte state);
byte get_frame_counter(void);
void set_frame_counter(byte counter);
void increment_frame_counter(void);
```

### sokoban_game.h

```c
void load_level(const char** d, byte r_num);
void draw_level(void);
void update_status(void);
void try_move_player(signed char dx, signed char dy);
byte get_player_x(void);
byte get_player_y(void);
byte get_total_boxes(void);
byte get_boxes_on_goals(void);
byte get_moves(void);
byte is_level_complete(void);
```

---

## Memory Map

```
Address Range   | Usage
----------------|------------------------------------------
0x0000-0x6FFF   | System and program memory
0x7000-0x73FF   | Custom character set (1024 bytes)
0x8000-0x801F   | Display list (32 bytes)
0x9000-0x93BF   | Screen memory (40x24 = 960 bytes)
0xE000-0xE3FF   | ROM character set (read-only)
```

---

## License

This refactored code maintains the same license as the original Sokoban implementation.

---

## Credits

- **Original Code**: Sokoban game for Atari 8-bit
- **Refactoring**: Separated into modular libraries for better organization
- **Platform**: Atari 800/XL/XE (6502 CPU)
- **Compiler**: CC65

