# Atari 8-bit Color Text and Graphics Modes

## Overview

Yes! The Atari 8-bit has **multiple color text modes** that can display fonts in different colors. The current Sokoban implementation uses **ANTIC Mode 2** (monochrome text), but there are several color alternatives.

---

## Available Text Modes

### ANTIC Mode 2 (Graphics 0) - **Currently Used**
- **Resolution**: 40 columns × 24 rows
- **Character size**: 8×8 pixels
- **Colors**: **1 color + background** (monochrome)
- **Memory**: 960 bytes screen RAM
- **Use case**: Standard text mode

**Limitations:**
- ❌ Only one text color (all characters same color)
- ❌ Cannot have different colored characters

---

### ANTIC Mode 4 (Graphics 12) - **4-Color Text** ✨
- **Resolution**: 40 columns × 8 rows
- **Character size**: 8×8 pixels
- **Colors**: **4 colors** (PF0, PF1, PF2, PF3)
- **Memory**: 320 bytes screen RAM
- **Use case**: Colorful text displays

**How it works:**
- Each character can use **4 different colors**
- Colors are defined in the character set itself (2 bits per pixel)
- You can redefine characters to use different color combinations

**Advantages:**
- ✅ Multiple colors per character
- ✅ Can create colorful game graphics
- ✅ Good for status displays

**Limitations:**
- ❌ Only 8 rows (vs 24 in mode 2)
- ❌ Requires custom character set design

---

### ANTIC Mode 5 (Graphics 13) - **4-Color Text, Tall**
- **Resolution**: 40 columns × 12 rows
- **Character size**: 8×16 pixels (double height)
- **Colors**: **4 colors** (PF0, PF1, PF2, PF3)
- **Memory**: 480 bytes screen RAM
- **Use case**: Large colorful text

**Same as Mode 4 but with taller characters**

---

### ANTIC Mode 6 (Graphics 1) - **5-Color Text** ✨✨
- **Resolution**: 20 columns × 24 rows
- **Character size**: 8×8 pixels
- **Colors**: **5 colors** (background + 4 playfield colors)
- **Memory**: 480 bytes screen RAM
- **Use case**: Colorful text with individual character colors

**How it works:**
- Each character position can have **2 colors** (foreground + background)
- The character code determines which 2 colors from the 5 available
- Upper 128 characters use different color pair than lower 128

**Color mapping:**
- Characters 0-127: Use PF0 (foreground) + background
- Characters 128-255: Use PF1 (foreground) + background

**Advantages:**
- ✅ Each character can have different color
- ✅ Easy to use (just change character code)
- ✅ Good for colorful game boards

**Limitations:**
- ❌ Only 20 columns (half of mode 2)
- ❌ Each character limited to 2 colors

---

### ANTIC Mode 7 (Graphics 2) - **5-Color Text, Tall**
- **Resolution**: 20 columns × 12 rows
- **Character size**: 8×16 pixels (double height)
- **Colors**: **5 colors** (background + 4 playfield colors)
- **Memory**: 240 bytes screen RAM
- **Use case**: Large colorful text

**Same as Mode 6 but with taller characters**

---

## Graphics Modes (Non-Text)

### ANTIC Mode 8 (Graphics 3) - **4-Color Bitmap**
- **Resolution**: 40×24 pixels
- **Colors**: 4 colors
- **Use case**: Low-res colorful graphics

### ANTIC Mode 9 (Graphics 4) - **2-Color Bitmap**
- **Resolution**: 80×48 pixels
- **Colors**: 2 colors
- **Use case**: Medium-res graphics

### ANTIC Mode 10 (Graphics 5) - **4-Color Bitmap**
- **Resolution**: 80×48 pixels
- **Colors**: 4 colors
- **Use case**: Medium-res colorful graphics

### ANTIC Mode 15 (Graphics 8) - **Monochrome Hi-Res**
- **Resolution**: 320×192 pixels
- **Colors**: 2 colors (1 + background)
- **Use case**: High-resolution graphics

---

## Special GTIA Modes (256 Colors!)

### Graphics 9 - **16 Luminances** 🌈
- **Resolution**: 80×192 pixels
- **Colors**: **16 shades of one hue** (from background color)
- **Total palette**: 16 luminances
- **Use case**: Grayscale or monochrome shaded graphics

### Graphics 10 - **9 Colors** 🌈
- **Resolution**: 80×192 pixels
- **Colors**: **9 colors** (all 9 palette registers)
- **Total palette**: 9 colors from 256
- **Use case**: Colorful graphics with palette control

### Graphics 11 - **16 Hues** 🌈
- **Resolution**: 80×192 pixels
- **Colors**: **16 hues** (luminance from background)
- **Total palette**: 16 hues
- **Use case**: Rainbow graphics

---

## Best Modes for Sokoban

### Option 1: ANTIC Mode 6 (Graphics 1) - **Recommended for Color** ✨

**Perfect for colorful Sokoban!**

**Specifications:**
- 20 columns × 24 rows (same height as current mode)
- Each character can be different color
- 5 colors total

**Example color scheme:**
```
Background: Black
PF0 (chars 0-127): Brown (walls, boxes)
PF1 (chars 128-255): Green (player)
PF2: Yellow (goals)
PF3: Red (boxes on goals)
```

**Implementation:**
- Change display list to use ANTIC mode 6
- Use character codes 0-127 for brown objects (walls, boxes)
- Use character codes 128-255 for green objects (player)
- Modify character set to use 2-color mode

**Screen size:**
- 20×24 tiles (perfect for 16×16 tile mode!)
- Would work great with your new 16×16 tile system

---

### Option 2: ANTIC Mode 4 (Graphics 12) - **4-Color Characters**

**For maximum color control per character**

**Specifications:**
- 40 columns × 8 rows
- 4 colors per character
- Can design multi-colored characters

**Example:**
- Wall: Brown bricks with gray mortar
- Box: Brown crate with yellow highlights
- Player: Multi-colored sprite
- Goal: Yellow circle with red center

**Limitations:**
- Only 8 rows (would need to design level differently)
- More complex character set design

---

## Color Palette

The Atari 8-bit has a palette of **256 colors**:
- **16 hues**: Orange, Gold, Yellow, Green, Cyan, Blue, Purple, Red, etc.
- **16 luminances**: From black (0) to white (15)
- **Total**: 16 × 16 = 256 colors

**Color registers:**
```
COLBK  ($D01A) - Background color
COLPF0 ($D016) - Playfield 0 color
COLPF1 ($D017) - Playfield 1 color
COLPF2 ($D018) - Playfield 2 color
COLPF3 ($D019) - Playfield 3 color
```

**Color format:**
```
Bits 7-4: Hue (0-15)
Bits 3-1: Luminance (0-7, even values only)
Bit 0: Unused
```

**Example colors:**
```c
#define COLOR_BLACK     0x00
#define COLOR_WHITE     0x0E
#define COLOR_RED       0x32
#define COLOR_GREEN     0xC4
#define COLOR_BLUE      0x96
#define COLOR_YELLOW    0xE8
#define COLOR_ORANGE    0x28
#define COLOR_BROWN     0x14
```

---

## Implementation Example: ANTIC Mode 6

Here's how to convert Sokoban to use **ANTIC Mode 6** for colorful text:

### 1. Update Display List

```c
// Current display list (ANTIC Mode 2)
static unsigned char dlist[] = {
    0x70, 0x70, 0x70,           // 24 blank lines
    0x42, 0x00, 0x90,           // Mode 2 + LMS
    0x02, 0x02, 0x02, ...       // 23 more Mode 2 lines
    0x41, 0x00, 0x80            // JVB
};

// New display list (ANTIC Mode 6)
static unsigned char dlist[] = {
    0x70, 0x70, 0x70,           // 24 blank lines
    0x46, 0x00, 0x90,           // Mode 6 + LMS
    0x06, 0x06, 0x06, ...       // 23 more Mode 6 lines
    0x41, 0x00, 0x80            // JVB
};
```

### 2. Set Color Registers

```c
void setup_colors(void) {
    // Background: Black
    POKE(0xD01A, 0x00);
    
    // PF0: Brown (for walls, boxes)
    POKE(0xD016, 0x14);
    
    // PF1: Green (for player)
    POKE(0xD017, 0xC4);
    
    // PF2: Yellow (for goals)
    POKE(0xD018, 0xE8);
    
    // PF3: Red (for boxes on goals)
    POKE(0xD019, 0x32);
}
```

### 3. Use Character Codes for Colors

```c
// Characters 0-127 use PF0 (brown)
#define CHAR_WALL       '#'      // Brown wall
#define CHAR_BOX        '$'      // Brown box

// Characters 128-255 use PF1 (green)
#define CHAR_PLAYER     (128+'@') // Green player

// For goals and special items, use custom characters
#define CHAR_GOAL       (0x80)   // Custom character, PF1 color
#define CHAR_BOXGOAL    (0x81)   // Custom character, PF1 color
```

### 4. Screen Dimensions

```c
// ANTIC Mode 6 dimensions
#define SCREEN_COLS 20
#define SCREEN_ROWS 24
#define SCREEN_MEM  0x9000
```

---

## Comparison Table

| Mode | ANTIC | Cols×Rows | Char Size | Colors | Best For |
|------|-------|-----------|-----------|--------|----------|
| **Gr.0** | 2 | 40×24 | 8×8 | 1+BG | Standard text |
| **Gr.1** | 6 | 20×24 | 8×8 | **5** | **Colorful text** ✨ |
| **Gr.2** | 7 | 20×12 | 8×16 | 5 | Large colorful text |
| **Gr.12** | 4 | 40×8 | 8×8 | **4** | **Multi-color chars** ✨ |
| **Gr.13** | 5 | 40×12 | 8×16 | 4 | Large multi-color |

---

## Recommendation for Sokoban

### Best Choice: **ANTIC Mode 6 (Graphics 1)** 🎯

**Why:**
1. ✅ **20×24 grid** - Perfect for your 16×16 tile system!
2. ✅ **5 colors** - Enough for all game elements
3. ✅ **Easy to implement** - Just change character codes for colors
4. ✅ **Same vertical resolution** - 24 rows like current mode
5. ✅ **Individual character colors** - Each tile can be different color

**Color scheme:**
- **Background**: Black
- **PF0 (brown)**: Walls, boxes
- **PF1 (green)**: Player
- **PF2 (yellow)**: Goals
- **PF3 (red)**: Boxes on goals, special items

**Perfect match:**
- Your 16×16 tile system uses 20×12 tiles
- ANTIC Mode 6 has 20 columns
- Each 16×16 tile would occupy 2 rows in Mode 6
- Total: 20×12 tiles = 20×24 characters ✅

---

## Next Steps

Would you like me to:

1. **Create a color version** of the Sokoban game using ANTIC Mode 6?
2. **Add color support** to the existing 8×8 or 16×16 tile system?
3. **Create a new library** for ANTIC Mode 6 with color support?
4. **Show example code** for setting up ANTIC Mode 6?

Let me know and I can implement colorful graphics for your Sokoban game! 🎨

