# Quick Start Guide

## 🎮 Sokoban with Dual Tile System

This is a Sokoban puzzle game for Atari 8-bit computers with support for **two tile sizes**:
- **8x8 tiles** (standard) - Simple graphics, more screen space
- **16x16 tiles** (big) - Detailed graphics, better visuals

---

## 🚀 Quick Start

### 1. Choose Your Tile Size

Open `sokoban.c` and find line 17:

**For 8x8 tiles (default):**
```c
//#define USE_BIG_TILES
```

**For 16x16 tiles:**
```c
#define USE_BIG_TILES
```

### 2. Load in 8bitworkshop IDE

1. Go to https://8bitworkshop.com/
2. Select platform: **Atari 8-bit**
3. Load `sokoban.c`
4. The game will compile and run automatically!

### 3. Play!

**Controls:**
- Joystick or arrow keys to move
- Push all boxes onto goal markers (circles)
- Win when all boxes are on goals!

---

## 📊 Tile Mode Comparison

| Feature | 8x8 Mode | 16x16 Mode |
|---------|----------|------------|
| **Tile Size** | 8×8 pixels | 16×16 pixels |
| **Screen Grid** | 40×24 tiles | 20×12 tiles |
| **Graphics** | Simple | Detailed |
| **Visual Quality** | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **Screen Space** | More | Less |

---

## 🎨 What You'll See

### 8x8 Mode
```
Simple, iconic graphics:
# = Wall (brick)
$ = Box (square)
. = Goal (circle)
@ = Player (stick figure)
```

### 16x16 Mode
```
Detailed, smooth graphics:
# = Wall (detailed bricks)
$ = Box (wooden crate)
. = Goal (smooth circle)
@ = Player (detailed figure)
```

---

## 📁 File Structure

```
sokobanc/
├── sokoban.c              ← Main file (edit this to switch modes)
│
├── 8x8 Mode Files
│   ├── atari_conio.c/h
│   ├── atari_font.c/h
│   └── sokoban_game.c/h
│
├── 16x16 Mode Files
│   ├── atari_conio_16x16.c/h
│   ├── atari_font_16x16.c/h
│   └── sokoban_game_16x16.c/h
│
└── Documentation
    ├── QUICK_START.md     ← You are here
    ├── TILE_SYSTEM_GUIDE.md
    ├── LIBRARY_USAGE.md
    └── CHANGELOG.md
```

---

## 🔧 How It Works

### Automatic Mode Selection

The `sokoban.c` file uses **conditional compilation**:

```c
#ifdef USE_BIG_TILES
    // Use 16x16 libraries
    #include "atari_conio_16x16.h"
    #include "atari_font_16x16.h"
    #include "sokoban_game_16x16.h"
#else
    // Use 8x8 libraries
    #include "atari_conio.h"
    #include "atari_font.h"
    #include "sokoban_game.h"
#endif
```

**You don't need to change anything else!** The same game loop code works for both modes.

---

## 🎯 Example: Switching Modes

### Step 1: Edit sokoban.c

Find this section:
```c
// ============================================================
// TILE SIZE CONFIGURATION
// ============================================================
// Uncomment the next line to use 16x16 tiles instead of 8x8
//#define USE_BIG_TILES
```

### Step 2: Uncomment for 16x16 Mode

Change to:
```c
#define USE_BIG_TILES
```

### Step 3: Save and Reload

The 8bitworkshop IDE will automatically:
1. Detect the change
2. Compile the 16x16 libraries
3. Link everything together
4. Run the game with big tiles!

---

## 📖 Documentation

### For Users
- **QUICK_START.md** (this file) - Get started quickly
- **TILE_SYSTEM_GUIDE.md** - Complete tile system guide

### For Developers
- **LIBRARY_USAGE.md** - How to use the libraries
- **REFACTORING.md** - Technical documentation
- **CHANGELOG.md** - Version history

---

## 🎮 Game Rules

### Objective
Push all boxes ($) onto goal markers (.)

### Rules
1. You can only **push** boxes, not pull them
2. You can only push **one box at a time**
3. Boxes can't be pushed into walls or other boxes
4. Win when all boxes are on goals

### Tips
- Plan your moves carefully!
- Don't push boxes into corners (you can't pull them out)
- Use the goal markers as guides
- The move counter shows your efficiency

---

## 🛠️ Troubleshooting

### Graphics Look Wrong
**Problem:** Tiles appear as random characters  
**Solution:** Make sure you saved `sokoban.c` after changing `USE_BIG_TILES`

### Level Doesn't Fit
**Problem:** Level is cut off  
**Solution:** 16x16 mode has a smaller grid (20×12). The level data automatically adjusts.

### Compilation Errors
**Problem:** Linker can't find functions  
**Solution:** The `//#link` directives should be automatically selected. Check that they match your mode.

### Game Runs Slowly
**Problem:** 16x16 mode feels slow  
**Solution:** This is normal - 16x16 mode renders 4× more characters. It's still playable!

---

## 🚀 Advanced Usage

### Creating Custom Levels

Edit the level data in `sokoban.c`:

```c
const char* my_level[] = {
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
- `'@'` = Player starting position

**Grid Limits:**
- 8x8 mode: Max 40×24
- 16x16 mode: Max 20×12

### Customizing Graphics

Edit the graphics arrays in:
- 8x8 mode: `atari_font.c`
- 16x16 mode: `atari_font_16x16.c`

See `TILE_SYSTEM_GUIDE.md` for details on creating custom graphics.

---

## 📦 Repository

**GitHub:** https://github.com/astroforgit/sokobanc

**Features:**
- ✅ Dual tile system (8x8 and 16x16)
- ✅ Modular library architecture
- ✅ Comprehensive documentation
- ✅ Auto-commit scripts
- ✅ Full source code

---

## 🎓 Learning Resources

### Understanding the Code

1. **Start with:** `sokoban.c` - Main game loop
2. **Then read:** `LIBRARY_USAGE.md` - How libraries work
3. **Deep dive:** `REFACTORING.md` - Technical details

### Tile System

1. **Overview:** `TILE_SYSTEM_GUIDE.md` - Complete guide
2. **Graphics:** `atari_font_16x16.c` - See how tiles are defined
3. **Rendering:** `atari_conio_16x16.c` - See how tiles are drawn

---

## 💡 Tips for Best Experience

### For Visual Quality
✅ Use **16x16 mode** for better graphics  
✅ Play on a CRT monitor or CRT shader for authentic look

### For Gameplay
✅ Use **8x8 mode** for more screen space  
✅ Larger levels work better in 8x8 mode

### For Development
✅ Start with 8x8 mode (simpler)  
✅ Test both modes to ensure compatibility  
✅ Use the auto-commit scripts for version control

---

## 🎉 Have Fun!

Enjoy playing Sokoban on your Atari 8-bit computer!

**Questions?** Check the documentation files or visit the GitHub repository.

**Want to contribute?** Fork the repo and submit a pull request!

---

## 📝 Credits

- **Platform:** Atari 800/XL/XE (6502 CPU)
- **Compiler:** CC65
- **IDE:** 8bitworkshop
- **Repository:** https://github.com/astroforgit/sokobanc

---

**Version 2.0** - Now with 16x16 tile support! 🎨

