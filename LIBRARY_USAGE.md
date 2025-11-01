# Library Usage Guide

## Quick Reference

This guide shows how to use each library in your own Atari 8-bit projects.

---

## 1. atari_conio - Console I/O Library

### Include in your project

```c
#include "atari_conio.h"
```

### Basic Screen Operations

```c
// Clear the entire screen
my_clrscr();

// Put a single character at position (10, 5)
my_cputcxy(10, 5, '@');

// Put a string at position (0, 0)
my_cputsxy(0, 0, "Hello, Atari!");

// Print formatted status (boxes, total, moves)
my_cprintf_status(3, 5, 42);
// Output: "Boxes: 3/5  Moves: 42"
```

### Timing and Synchronization

```c
// Wait for vertical blank (60Hz NTSC / 50Hz PAL)
// Use this to synchronize your game loop
while (1) {
    // Game logic here
    
    wait_vblank();  // Wait for screen refresh
}
```

### Memory Layout

```c
#define SCREEN_MEM ((byte*)0x9000)  // 40x24 screen memory
#define COLS 40
#define ROWS 24
```

### Example: Simple Text Display

```c
#include "atari_conio.h"

void main(void) {
    my_clrscr();
    my_cputsxy(10, 10, "Press any key");
    
    while (1) {
        wait_vblank();
    }
}
```

---

## 2. atari_font - Font and Graphics Library

### Include in your project

```c
#include "atari_font.h"
```

### Setup Graphics Mode

```c
// Initialize custom graphics mode with character set
// Call this once at the start of your program
setup_graphics();
```

This function:
- Creates a custom display list
- Copies ROM character set to RAM
- Sets up custom character graphics
- Configures colors

### Animation System

```c
// In your game loop:
while (1) {
    // Update animation
    animate_player();
    
    // Synchronize to screen refresh
    wait_vblank();
    
    // Increment frame counter
    increment_frame_counter();
}
```

### Animation Control

```c
// Trigger walking animation (e.g., when player moves)
set_anim_state(1);
set_frame_counter(0);

// Check current animation state
byte state = get_anim_state();  // 0 = idle, 1 = walking

// Get current frame
byte frame = get_frame_counter();
```

### Memory Layout

```c
#define CHARSET_MEM ((byte*)0x7000)  // Custom character set
#define DLIST_MEM   ((byte*)0x8000)  // Display list
#define ROM_CHARSET_ADDRESS 0xE000   // ROM character set
```

### Example: Custom Graphics Setup

```c
#include "atari_font.h"
#include "atari_conio.h"

void main(void) {
    // Setup custom graphics
    setup_graphics();
    
    // Clear screen
    my_clrscr();
    
    // Display custom characters
    my_cputcxy(10, 10, '#');  // Wall
    my_cputcxy(12, 10, '$');  // Box
    my_cputcxy(14, 10, '.');  // Goal
    my_cputcxy(16, 10, '@');  // Player
    
    // Animation loop
    while (1) {
        animate_player();
        wait_vblank();
        increment_frame_counter();
    }
}
```

---

## 3. sokoban_game - Game Logic Library

### Include in your project

```c
#include "sokoban_game.h"
```

### Level Data Format

```c
// Define your level as an array of strings
const char* my_level[] = {
    "  ####  ",
    "  #  #  ",
    "  #$ #  ",
    "###  ###",
    "#  $@  #",
    "#   .  #",
    "########"
};
```

### Level Loading and Display

```c
// Load the level
load_level(my_level, sizeof(my_level) / sizeof(my_level[0]));

// Draw the level to screen
draw_level();

// Update status line
update_status();
```

### Player Movement

```c
// Move player up
try_move_player(0, -1);

// Move player down
try_move_player(0, 1);

// Move player left
try_move_player(-1, 0);

// Move player right
try_move_player(1, 0);
```

### Game State Queries

```c
// Get player position
byte x = get_player_x();
byte y = get_player_y();

// Get game statistics
byte total = get_total_boxes();
byte on_goals = get_boxes_on_goals();
byte moves = get_moves();

// Check win condition
if (is_level_complete()) {
    // Player won!
}
```

### Tile Definitions

```c
#define TILE_EMPTY       0    // Empty space
#define TILE_WALL       '#'   // Wall
#define TILE_GOAL       '.'   // Goal marker
#define TILE_BOX        '$'   // Box
#define TILE_BOX_ON_GOAL '*'  // Box on goal
#define TILE_PLAYER     '@'   // Player
#define TILE_PLAYER_ON_GOAL '+' // Player on goal
```

### Example: Complete Game

```c
#include <joystick.h>
#include "atari_conio.h"
#include "atari_font.h"
#include "sokoban_game.h"

const char* level[] = {
    "########",
    "#  .   #",
    "# $@$  #",
    "#  .   #",
    "########"
};

void main(void) {
    byte joy, last_joy = 0;
    
    // Initialize
    joy_install(joy_static_stddrv);
    setup_graphics();
    load_level(level, sizeof(level) / sizeof(level[0]));
    draw_level();
    update_status();
    
    // Game loop
    while (!is_level_complete()) {
        joy = joy_read(0);
        
        if (joy != last_joy) {
            if (JOY_UP(joy))    try_move_player(0, -1);
            if (JOY_DOWN(joy))  try_move_player(0, 1);
            if (JOY_LEFT(joy))  try_move_player(-1, 0);
            if (JOY_RIGHT(joy)) try_move_player(1, 0);
        }
        last_joy = joy;
        
        animate_player();
        wait_vblank();
        increment_frame_counter();
    }
    
    // Win!
    my_cputsxy(10, 10, "YOU WIN!");
    while (1);
}
```

---

## Combining All Libraries

### Full Example with All Features

```c
#include <stdlib.h>
#include <joystick.h>
#include <conio.h>

#include "atari_conio.h"
#include "atari_font.h"
#include "sokoban_game.h"

// Level data
const char* level_1[] = {
    "    #####",
    "    #   #",
    "    #$  #",
    "  ###  $##",
    "  #  $ $ #",
    "### # ## #   ######",
    "#   # ## #####  ..#",
    "# $  $          ..#",
    "##### ### #@##  ..#",
    "    #     #########",
    "    #######"
};

void main(void) {
    byte joy, last_joy = 0;
    
    // 1. Initialize joystick
    joy_install(joy_static_stddrv);
    
    // 2. Setup custom graphics
    setup_graphics();
    
    // 3. Load and display level
    load_level(level_1, sizeof(level_1) / sizeof(level_1[0]));
    draw_level();
    update_status();
    
    // 4. Main game loop
    while (1) {
        // Read input
        joy = joy_read(0);
        
        // Process movement
        if (joy != last_joy) {
            if (JOY_UP(joy))    try_move_player(0, -1);
            if (JOY_DOWN(joy))  try_move_player(0, 1);
            if (JOY_LEFT(joy))  try_move_player(-1, 0);
            if (JOY_RIGHT(joy)) try_move_player(1, 0);
        }
        last_joy = joy;
        
        // Check win condition
        if (is_level_complete()) {
            break;
        }
        
        // Update animation
        animate_player();
        
        // Synchronize to screen
        wait_vblank();
        
        // Update frame counter
        increment_frame_counter();
    }
    
    // 5. Display win message
    my_cputsxy(15, 12, "YOU WIN!");
    cgetc();
    
    while (1);
}
```

---

## Tips and Best Practices

### Performance

1. **Use `wait_vblank()` in your main loop** to synchronize to screen refresh
   - Prevents screen tearing
   - Provides consistent frame rate
   - Reduces CPU usage

2. **Call `animate_player()` every frame** for smooth animation
   - Updates character graphics in real-time
   - Handles both walking and idle animations

3. **Use direct screen memory access** for fast rendering
   - `my_cputcxy()` is faster than standard `conio.h` functions
   - No overhead from cursor positioning

### Memory Management

1. **Character set is at 0x7000** - Don't use this area for other data
2. **Display list is at 0x8000** - Reserved for graphics system
3. **Screen memory is at 0x9000** - 960 bytes (40x24)

### Animation

1. **Walking animation**: Triggered by `set_anim_state(1)`
   - Lasts 8 frames
   - Automatically returns to idle

2. **Idle animation**: Blinking effect
   - Triggers every ~50 frames
   - Lasts 3 frames

### Game Logic

1. **Level format**: Use strings with these characters:
   - `' '` = Empty space
   - `'#'` = Wall
   - `'.'` = Goal
   - `'$'` = Box
   - `'@'` = Player starting position

2. **Win condition**: Check `is_level_complete()`
   - Returns 1 when all boxes are on goals

3. **Movement**: Use `try_move_player(dx, dy)`
   - Handles collision detection
   - Handles box pushing
   - Updates move counter
   - Triggers walking animation

---

## Porting to Other Projects

### Using atari_conio in other projects

The console I/O library can be used in any Atari 8-bit project:

```c
#include "atari_conio.h"

void main(void) {
    my_clrscr();
    my_cputsxy(0, 0, "My Atari Program");
    
    // Your code here
}
```

### Using atari_font for custom graphics

Adapt the graphics data for your own sprites:

```c
// In atari_font.c, modify custom_graphics array:
static unsigned char custom_graphics[] = {
    // Your custom 8x8 character graphics here
    0x00,0x18,0x3C,0x7E,0x7E,0x3C,0x18,0x00, // Example: Diamond
    // ... more graphics
};
```

### Creating your own game logic

Use `sokoban_game.c` as a template for other puzzle games:
- Modify tile definitions
- Adapt movement logic
- Change win conditions

---

## Troubleshooting

### Screen is blank
- Make sure you called `setup_graphics()` first
- Check that you called `draw_level()` after loading

### Animation not working
- Ensure `animate_player()` is called every frame
- Check that `increment_frame_counter()` is called
- Verify `wait_vblank()` is in your main loop

### Player won't move
- Check joystick initialization: `joy_install(joy_static_stddrv)`
- Verify level data has valid tiles
- Make sure player starting position (`@`) is in level data

### Compilation errors
- Include all necessary headers
- Link all `.c` files during compilation
- Check that header guards are present

---

## License

These libraries are provided as-is for use in Atari 8-bit projects.

