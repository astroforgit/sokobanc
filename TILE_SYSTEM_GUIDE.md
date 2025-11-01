# 16x16 Tile System Guide

## Overview

The Sokoban game now supports **two tile sizes**:

1. **8x8 tiles** (Standard mode) - 1 character cell per tile
2. **16x16 tiles** (Big tile mode) - 4 character cells (2x2 grid) per tile

You can switch between modes by editing a single line in `sokoban.c`.

---

## How to Switch Between Tile Modes

### Using 8x8 Tiles (Default)

In `sokoban.c`, keep this line **commented out**:

```c
//#define USE_BIG_TILES
```

### Using 16x16 Tiles

In `sokoban.c`, **uncomment** this line:

```c
#define USE_BIG_TILES
```

That's it! The code will automatically use the correct libraries and level data.

---

## Technical Details

### 8x8 Tile Mode (Standard)

**Screen Layout:**
- 40 columns × 24 rows of characters
- Each tile = 1 character (8×8 pixels)
- Total playable area: 40×24 tiles

**Files Used:**
- `atari_conio.c` / `atari_conio.h`
- `atari_font.c` / `atari_font.h`
- `sokoban_game.c` / `sokoban_game.h`

**Graphics:**
- Simple 8×8 pixel sprites
- 1 character per game object

---

### 16x16 Tile Mode (Big Tiles)

**Screen Layout:**
- 40 columns × 24 rows of characters
- Each tile = 4 characters in 2×2 grid (16×16 pixels)
- Total playable area: 20×12 tiles

**Files Used:**
- `atari_conio_16x16.c` / `atari_conio_16x16.h`
- `atari_font_16x16.c` / `atari_font_16x16.h`
- `sokoban_game_16x16.c` / `sokoban_game_16x16.h`

**Graphics:**
- Detailed 16×16 pixel sprites
- 4 characters per game object arranged as:
  ```
  [Top-Left]    [Top-Right]
  [Bottom-Left] [Bottom-Right]
  ```

---

## 16x16 Tile Character Mapping

Each 16×16 tile uses 4 consecutive character codes:

| Tile Type | Top-Left | Top-Right | Bottom-Left | Bottom-Right |
|-----------|----------|-----------|-------------|--------------|
| **Wall** | 0x40 | 0x41 | 0x42 | 0x43 |
| **Box** | 0x44 | 0x45 | 0x46 | 0x47 |
| **Goal** | 0x48 | 0x49 | 0x4A | 0x4B |
| **Box on Goal** | 0x4C | 0x4D | 0x4E | 0x4F |
| **Player** | 0x50 | 0x51 | 0x52 | 0x53 |
| **Player on Goal** | 0x54 | 0x55 | 0x56 | 0x57 |

---

## Graphics Design

### 8x8 Tile Graphics

Simple, iconic representations:
- **Wall**: Brick pattern
- **Box**: Square with border
- **Goal**: Circle target
- **Player**: Stick figure

### 16x16 Tile Graphics

More detailed sprites with better visual quality:

**Wall (16×16):**
```
████████████████
██          ████
██          ████
██          ████
██          ████
████████████████
```

**Box (16×16):**
```
████████████████
██          ████
██  ██  ██  ████
██  ██  ██  ████
██  ██  ██  ████
██          ████
████████████████
```

**Goal (16×16):**
```
    ████████    
  ██      ██  
  ██      ██  
██          ██
██          ██
  ██      ██  
  ██      ██  
    ████████    
```

**Player (16×16):**
```
      ████      
    ████████    
    ████████    
  ████████████  
  ████████████  
      ████      
    ██    ██    
  ██        ██  
```

---

## API Differences

### Console I/O Functions

**8x8 Mode:**
```c
void my_clrscr(void);
void my_cputcxy(byte x, byte y, byte character);
void my_cputsxy(byte x, byte y, const char* str);
void wait_vblank(void);
```

**16x16 Mode:**
```c
void my_clrscr_16x16(void);
void my_put_tile_16x16(byte tx, byte ty, byte tile_tl);
void my_cputsxy_16x16(byte x, byte y, const char* str);
void wait_vblank_16x16(void);
```

**Key Difference:**
- 8x8 mode: `my_cputcxy()` puts a single character
- 16x16 mode: `my_put_tile_16x16()` puts a 2×2 tile (4 characters)

### Game Logic Functions

Both modes use the same function names, but with `_16x16` suffix in big tile mode:

```c
// 8x8 mode
void load_level(const char** d, byte r_num);
void draw_level(void);
void try_move_player(signed char dx, signed char dy);

// 16x16 mode
void load_level_16x16(const char** d, byte r_num);
void draw_level_16x16(void);
void try_move_player_16x16(signed char dx, signed char dy);
```

**The main file uses aliases**, so you don't need to change the game loop code!

---

## Level Data Format

Both modes use the **same level data format**:

```c
const char* level[] = {
    "  ####  ",
    "  #  #  ",
    "  #$ #  ",
    " ##  ## ",
    " # $@.# ",
    " #  ..# ",
    " ###### "
};
```

**Characters:**
- `' '` = Empty space
- `'#'` = Wall
- `'$'` = Box
- `'.'` = Goal
- `'@'` = Player
- `'*'` = Box on goal
- `'+'` = Player on goal

**Important:** 16x16 mode has a smaller grid (20×12 vs 40×24), so levels should be designed accordingly.

---

## Memory Usage

### 8x8 Mode
- Character set: ~256 bytes for custom graphics
- Screen memory: 960 bytes (40×24)
- Level data: ~960 bytes

### 16x16 Mode
- Character set: ~768 bytes for custom graphics (24 tiles × 32 bytes)
- Screen memory: 960 bytes (40×24 characters, but only 20×12 tiles used)
- Level data: ~240 bytes (20×12)

**16x16 mode uses more character set memory but less level data memory.**

---

## Animation System

Both modes support player animation:

### 8x8 Mode
- Standing frame
- Walking frame
- Blinking frame (idle)

### 16x16 Mode
- Standing frame (4 characters)
- Walking frame (4 characters)
- Blinking frame (4 characters)

**Animation is synchronized to VBLANK (60Hz NTSC / 50Hz PAL)**

---

## Creating Custom 16x16 Graphics

To create your own 16x16 tile graphics:

1. **Design the 16×16 pixel sprite** (use a pixel art editor)

2. **Split into 4 quadrants** (8×8 each):
   - Top-Left
   - Top-Right
   - Bottom-Left
   - Bottom-Right

3. **Convert each quadrant to 8 bytes** (1 byte per row)

4. **Add to `custom_graphics_16x16[]` array** in `atari_font_16x16.c`

### Example: Creating a Heart Tile

```
16×16 Heart:
  ████  ████  
████████████  
████████████  
████████████  
  ████████    
    ████      
      ██      
```

Split into 4 quadrants and convert to hex:

```c
// Heart tile (0x58-0x5B)
// Top-Left
0x00,0x1E,0x3F,0x3F,0x3F,0x1F,0x0F,0x07,
// Top-Right
0x00,0x78,0xFC,0xFC,0xFC,0xF8,0xF0,0xE0,
// Bottom-Left
0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
// Bottom-Right
0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
```

---

## Performance Considerations

### 8x8 Mode
- ✅ Faster rendering (1 character per tile)
- ✅ More screen space (40×24 tiles)
- ✅ Simpler graphics code
- ❌ Less detailed graphics

### 16x16 Mode
- ✅ More detailed graphics
- ✅ Better visual quality
- ✅ Easier to see game elements
- ❌ Slower rendering (4 characters per tile)
- ❌ Less screen space (20×12 tiles)
- ❌ More complex graphics code

**Both modes run smoothly on Atari 8-bit hardware!**

---

## Compilation

The 8bitworkshop IDE automatically compiles the correct files based on the `USE_BIG_TILES` define.

**8x8 Mode:**
```
sokoban.c → links → atari_conio.c, atari_font.c, sokoban_game.c
```

**16x16 Mode:**
```
sokoban.c → links → atari_conio_16x16.c, atari_font_16x16.c, sokoban_game_16x16.c
```

---

## Examples

### Switching to 16x16 Mode

1. Open `sokoban.c`
2. Find this line:
   ```c
   //#define USE_BIG_TILES
   ```
3. Uncomment it:
   ```c
   #define USE_BIG_TILES
   ```
4. Save and reload in 8bitworkshop IDE
5. The game now uses 16×16 tiles!

### Creating a Custom Level for 16x16 Mode

```c
#ifdef USE_BIG_TILES
const char* my_level[] = {
    "    ####    ",
    "    #  #    ",
    "    #$ #    ",
    "  ###  ###  ",
    "  #  $   #  ",
    "  # # ## #  ",
    "  # # ## #  ",
    "  #  $  .#  ",
    "  #### #@#  ",
    "     #  .#  ",
    "     ####   "
};
#endif
```

---

## Troubleshooting

### Graphics Look Wrong
- Make sure you uncommented/commented `USE_BIG_TILES` correctly
- Reload the file in the IDE after changing the define

### Level Doesn't Fit
- 16x16 mode: Max 20×12 tiles
- 8x8 mode: Max 40×24 tiles
- Adjust your level data accordingly

### Linker Errors
- Make sure the `//#link` directives match your mode
- The conditional compilation should handle this automatically

---

## Summary

| Feature | 8x8 Mode | 16x16 Mode |
|---------|----------|------------|
| **Tile Size** | 8×8 pixels | 16×16 pixels |
| **Characters per Tile** | 1 | 4 (2×2 grid) |
| **Screen Grid** | 40×24 | 20×12 |
| **Graphics Detail** | Simple | Detailed |
| **Memory Usage** | Lower | Higher |
| **Rendering Speed** | Faster | Slower |
| **Switch Method** | Comment `USE_BIG_TILES` | Uncomment `USE_BIG_TILES` |

**Both modes are fully functional and can be switched with a single line change!**

---

## Credits

- **8x8 Tile System**: Original Sokoban implementation
- **16x16 Tile System**: Extended version with detailed graphics
- **Platform**: Atari 800/XL/XE (6502 CPU)
- **Compiler**: CC65

