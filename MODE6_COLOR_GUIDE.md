# ANTIC Mode 6 Color System Guide

## Overview

The Sokoban game now supports **ANTIC Mode 6** (Graphics 1) for colorful gameplay!

**ANTIC Mode 6 Features:**
- **20 columns × 24 rows** character grid
- **5 colors** (background + 4 playfield colors)
- Each character can use one of two color pairs
- Perfect for colorful retro games!

---

## Color Scheme

### Game Colors

| Element | Color | Register | Value |
|---------|-------|----------|-------|
| **Background** | Black | COLBK | 0x00 |
| **Walls & Boxes** | Brown | PF0 | 0x14 |
| **Player** | Green | PF1 | 0xC4 |
| **Goals** | Yellow | PF2 | 0xE8 |
| **Boxes on Goals** | Red | PF3 | 0x32 |

### How Colors Work in Mode 6

ANTIC Mode 6 uses a clever color system:

- **Characters 0-127**: Use **PF0 color** (brown)
  - Walls: Brown
  - Boxes: Brown
  
- **Characters 128-255**: Use **PF1 color** (green)
  - Player: Green
  - Player on goal: Green

- **Special characters**: Can be designed to use PF2 or PF3
  - Goals: Yellow (PF2)
  - Boxes on goals: Red (PF3)

---

## Screen Layout

### Grid Dimensions

```
┌────────────────────────────────────┐
│  20 columns × 24 rows              │
│                                    │
│  Perfect for Sokoban levels!       │
│                                    │
│  Each character: 8×8 pixels        │
│  Total screen: 160×192 pixels      │
└────────────────────────────────────┘
```

### Comparison with Other Modes

| Mode | Columns | Rows | Colors | Use Case |
|------|---------|------|--------|----------|
| **Mode 2** (8×8) | 40 | 24 | 1+BG | Monochrome text |
| **Mode 6** (Color) | **20** | **24** | **5** | **Colorful game** ✨ |
| **16×16 tiles** | 20 | 12 | 1+BG | Big monochrome tiles |

---

## How to Enable Mode 6

### Step 1: Edit sokoban.c

Find line 19 in `sokoban.c`:

```c
// Uncomment ONE of the following lines:

//#define USE_BIG_TILES      // 16x16 tiles (20x12 grid, monochrome)
//#define USE_MODE6_COLOR    // ANTIC Mode 6 (20x24 grid, 5 colors)
// Default: 8x8 tiles (40x24 grid, monochrome)
```

### Step 2: Uncomment Mode 6

Change to:

```c
#define USE_MODE6_COLOR    // ANTIC Mode 6 (20x24 grid, 5 colors)
```

### Step 3: Save and Reload

The 8bitworkshop IDE will automatically:
1. Compile the Mode 6 libraries
2. Link everything together
3. Run the game with 5 colors!

---

## Technical Details

### Display List

ANTIC Mode 6 uses display list mode **0x06**:

```c
static unsigned char display_list_mode6[] = {
    0x70, 0x70, 0x70,           // 24 blank lines (top border)
    0x46, 0x00, 0x90,           // Mode 6 + LMS, screen at $9000
    0x06, 0x06, 0x06, ...       // 23 more Mode 6 lines
    0x41, 0x00, 0x80            // JVB - Jump and wait for VBlank
};
```

### Color Registers

```c
void setup_colors_mode6(void) {
    POKE(COLOR0, COLOR_BLACK);   // Background: Black
    POKE(COLOR1, COLOR_BROWN);   // PF0: Brown (walls, boxes)
    POKE(COLOR2, COLOR_GREEN);   // PF1: Green (player)
    POKE(COLOR3, COLOR_YELLOW);  // PF2: Yellow (goals)
    POKE(COLOR4, COLOR_RED);     // PF3: Red (boxes on goals)
}
```

### Character Mapping

```c
// Brown elements (PF0) - characters 0-127
#define CHAR_WALL       '#'
#define CHAR_BOX        '$'

// Green elements (PF1) - characters 128-255
#define CHAR_PLAYER     (128 + '@')

// Custom characters for special colors
#define CHAR_GOAL_CUSTOM       0x42  // Yellow (PF2)
#define CHAR_BOXGOAL_CUSTOM    0x43  // Red (PF3)
```

---

## File Structure

### New Mode 6 Files

```
sokobanc/
├── atari_font_mode6.h/c       ← Graphics & colors
├── atari_conio_mode6.h/c      ← Console I/O
├── sokoban_game_mode6.h/c     ← Game logic
└── MODE6_COLOR_GUIDE.md       ← This file
```

### Conditional Compilation

The main file automatically selects the correct libraries:

```c
#ifdef USE_MODE6_COLOR
    //#link "atari_conio_mode6.c"
    //#link "atari_font_mode6.c"
    //#link "sokoban_game_mode6.c"
    
    #include "atari_conio_mode6.h"
    #include "atari_font_mode6.h"
    #include "sokoban_game_mode6.h"
#endif
```

---

## API Reference

### Graphics Functions

```c
// Setup ANTIC Mode 6 and colors
void setup_graphics_mode6(void);

// Setup color palette
void setup_colors_mode6(void);

// Animate player
void animate_player_mode6(void);
```

### Console I/O Functions

```c
// Clear screen
void my_clrscr_mode6(void);

// Put character at position
void my_cputcxy_mode6(byte x, byte y, byte character);

// Put string (PF0 color - brown)
void my_cputsxy_mode6(byte x, byte y, const char* str);

// Put colored string
void my_cputsxy_color_mode6(byte x, byte y, const char* str, byte use_pf1);

// Print status line
void my_cprintf_status_mode6(byte b, byte t, byte m);

// Wait for vertical blank
void wait_vblank_mode6(void);
```

### Game Logic Functions

```c
// Load level
void load_level_mode6(const char** d, byte r_num);

// Draw level with colors
void draw_level_mode6(void);

// Update status
void update_status_mode6(void);

// Move player
void try_move_player_mode6(signed char dx, signed char dy);

// Check win condition
byte is_level_complete_mode6(void);
```

---

## Color Customization

### Changing Colors

Edit `atari_font_mode6.h` to change the color scheme:

```c
// Current colors
#define COLOR_BLACK     0x00
#define COLOR_BROWN     0x14    // Walls/boxes
#define COLOR_GREEN     0xC4    // Player
#define COLOR_YELLOW    0xE8    // Goals
#define COLOR_RED       0x32    // Boxes on goals

// Alternative color schemes:

// Scheme 1: Blue theme
#define COLOR_BLUE      0x96    // Walls
#define COLOR_CYAN      0xA4    // Player
#define COLOR_YELLOW    0xE8    // Goals
#define COLOR_ORANGE    0x28    // Boxes on goals

// Scheme 2: Purple theme
#define COLOR_PURPLE    0x62    // Walls
#define COLOR_PINK      0x42    // Player
#define COLOR_WHITE     0x0E    // Goals
#define COLOR_RED       0x32    // Boxes on goals
```

### Atari Color Values

Format: `0xHL` where:
- `H` = Hue (0-F)
- `L` = Luminance (0, 2, 4, 6, 8, A, C, E)

**Common colors:**
```c
0x00 = Black
0x0E = White
0x14 = Brown
0x28 = Orange
0x32 = Red
0x62 = Purple
0x96 = Blue
0xA4 = Cyan
0xC4 = Green
0xE8 = Yellow
```

---

## Advantages of Mode 6

### ✅ Benefits

1. **Colorful Graphics**
   - 5 distinct colors make game elements easy to identify
   - Brown walls, green player, yellow goals, red boxes on goals

2. **Perfect Grid Size**
   - 20×24 grid is ideal for Sokoban levels
   - Same height as standard mode (24 rows)
   - Narrower width encourages compact level design

3. **Easy to Use**
   - Just change character codes to change colors
   - No complex graphics programming needed
   - Simple color register setup

4. **Retro Aesthetic**
   - Authentic Atari 8-bit look
   - Vibrant colors on CRT displays
   - Nostalgic gaming experience

### ⚠️ Limitations

1. **Narrower Screen**
   - Only 20 columns (vs 40 in standard mode)
   - Levels must be designed for smaller width

2. **Limited Color Control**
   - Each character limited to 2 colors (foreground + background)
   - Can't have multi-colored characters like Mode 4

3. **Character Set Constraints**
   - Characters 0-127 always use PF0
   - Characters 128-255 always use PF1
   - Need custom characters for PF2/PF3 colors

---

## Comparison Table

| Feature | Mode 2 (8×8) | Mode 6 (Color) | 16×16 Tiles |
|---------|--------------|----------------|-------------|
| **Grid** | 40×24 | **20×24** | 20×12 |
| **Colors** | 1+BG | **5** | 1+BG |
| **Tile Size** | 8×8 px | 8×8 px | 16×16 px |
| **Detail** | Simple | Simple | Detailed |
| **Best For** | Text | **Colorful games** | Big graphics |

---

## Example: Colorful Sokoban

### Visual Representation

```
Mode 2 (Monochrome):          Mode 6 (Color):
┌──────────────────┐          ┌──────────────────┐
│ ####             │          │ #### (brown)     │
│ #  #             │          │ #  #             │
│ # $#             │          │ # $ (brown)      │
│ ##  ##           │          │ ##  ##           │
│ # $ @#           │          │ # $ @ (green)    │
│ #  ..#           │          │ #  .. (yellow)   │
│ ######           │          │ ######           │
└──────────────────┘          └──────────────────┘
```

### Color Legend

- **Brown (#)**: Walls and boxes - Easy to see structure
- **Green (@)**: Player - Stands out clearly
- **Yellow (.)**: Goals - Bright targets
- **Red (*)**: Boxes on goals - Shows progress

---

## Tips for Level Design

### Mode 6 Level Guidelines

1. **Width**: Max 20 characters
   - Keep levels compact
   - Use vertical space (24 rows available)

2. **Visibility**: Use colors wisely
   - Brown for obstacles (walls, boxes)
   - Green for player (always visible)
   - Yellow for targets (goals)
   - Red for success (boxes on goals)

3. **Layout**: Center smaller levels
   - Pad with spaces on left/right
   - Use all 24 rows if needed

### Example Level (20×24)

```c
const char* level_mode6[] = {
    "    ######          ",  // Row 0
    "    #    #          ",  // Row 1
    "    # $  #          ",  // Row 2
    "  ### $  ##         ",  // Row 3
    "  #  $ $  #         ",  // Row 4
    "  # # ##  #         ",  // Row 5
    "  # # ##  #         ",  // Row 6
    "  #  $  ..#         ",  // Row 7
    "  #### #@.#         ",  // Row 8
    "     #   .#         ",  // Row 9
    "     ######         ",  // Row 10
    // Rows 11-23 can be used for larger levels
};
```

---

## Troubleshooting

### Colors Don't Appear

**Problem**: Game runs but no colors  
**Solution**: Make sure `USE_MODE6_COLOR` is defined and other modes are commented out

### Wrong Colors

**Problem**: Colors are different than expected  
**Solution**: Check color register values in `atari_font_mode6.c`

### Screen Corruption

**Problem**: Graphics look garbled  
**Solution**: Verify display list is correctly installed

### Level Too Wide

**Problem**: Level is cut off on right side  
**Solution**: Mode 6 only supports 20 columns - redesign level

---

## Summary

**ANTIC Mode 6** brings colorful graphics to Sokoban!

**Key Features:**
- ✅ 5 colors (black, brown, green, yellow, red)
- ✅ 20×24 character grid
- ✅ Easy to enable (one line change)
- ✅ Authentic Atari 8-bit experience

**To enable:**
```c
#define USE_MODE6_COLOR
```

**Enjoy colorful retro gaming!** 🎨🎮

