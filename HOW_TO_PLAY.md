# How to Play Sokoban - Display Mode Guide

## 🎮 Three Ways to Play!

Your Sokoban game now has **three separate main files** - one for each display mode. Just open the file you want in 8bitworkshop!

---

## 📁 Choose Your File

### Option 1: Standard 8×8 Mode (Classic)

**File:** `sokoban_8x8.c`

**Features:**
- 40×24 character grid
- Monochrome graphics
- Classic Atari look
- Largest playable area

**Best for:** Traditional Sokoban experience

**[▶️ Play Now](http://8bitworkshop.com/redir.html?platform=atari8-800&githubURL=https%3A%2F%2Fgithub.com%2Fastroforgit%2Fsokobanc&file=sokoban_8x8.c)**

---

### Option 2: 16×16 Big Tiles

**File:** `sokoban_16x16.c`

**Features:**
- 20×12 tile grid
- Each tile is 16×16 pixels (2×2 characters)
- Detailed graphics
- Larger, clearer visuals

**Best for:** Better visibility and detailed graphics

**[▶️ Play Now](http://8bitworkshop.com/redir.html?platform=atari8-800&githubURL=https%3A%2F%2Fgithub.com%2Fastroforgit%2Fsokobanc&file=sokoban_16x16.c)**

---

### Option 3: ANTIC Mode 6 Color ✨ NEW!

**File:** `sokoban_mode6.c`

**Features:**
- 20×24 character grid
- **5 vibrant colors!**
  - 🟤 Brown: Walls and boxes
  - 🟢 Green: Player
  - 🟡 Yellow: Goals
  - 🔴 Red: Boxes on goals
  - ⚫ Black: Background
- Authentic Atari 8-bit color graphics

**Best for:** Colorful retro gaming experience!

**[▶️ Play Now](http://8bitworkshop.com/redir.html?platform=atari8-800&githubURL=https%3A%2F%2Fgithub.com%2Fastroforgit%2Fsokobanc&file=sokoban_mode6.c)** ⭐ **RECOMMENDED!**

---

## 🕹️ Controls

All modes use the same controls:

### Keyboard
- **Arrow Keys** - Move player
- **W/A/S/D** - Alternative movement
- **ESC** - Quit game

### Joystick
- **Joystick Directions** - Move player
- **Button** - (reserved for future use)

---

## 🎯 How to Play

### Objective
Push all boxes ($) onto the goal targets (.) to complete the level.

### Rules
1. You can only **push** boxes, not pull them
2. You can only push **one box at a time**
3. Don't push boxes into corners - they'll be stuck!
4. Plan your moves carefully

### Tips
- 💡 Work backwards from the goals
- 💡 Keep boxes away from walls when possible
- 💡 Use the move counter to track efficiency
- 💡 The color mode makes it easier to see goals!

---

## 🖥️ Using 8bitworkshop IDE

### Method 1: Direct Links (Easiest)

Click one of the "Play Now" links above. The IDE will:
1. Load the project from GitHub
2. Open the selected file
3. Compile automatically
4. Start the game!

### Method 2: Manual Selection

1. Go to https://8bitworkshop.com/
2. Select **Atari 8-bit** platform
3. Click **File** → **Open GitHub Repository**
4. Enter: `https://github.com/astroforgit/sokobanc`
5. Select one of these files:
   - `sokoban_8x8.c` - Standard mode
   - `sokoban_16x16.c` - Big tiles
   - `sokoban_mode6.c` - Color mode ✨

### Method 3: Local Development

1. Clone the repository
2. Open 8bitworkshop IDE
3. Upload the files
4. Open the main file for your desired mode

---

## 📊 Mode Comparison

| Feature | 8×8 | 16×16 | Mode 6 |
|---------|-----|-------|--------|
| **Grid Size** | 40×24 | 20×12 | 20×24 |
| **Colors** | 1+BG | 1+BG | **5** ✨ |
| **Tile Size** | 8×8 px | 16×16 px | 8×8 px |
| **Detail Level** | Simple | High | Simple |
| **Playable Area** | Large | Medium | Medium |
| **Visual Appeal** | Classic | Detailed | **Colorful** ✨ |
| **Best For** | Large levels | Clear graphics | **Color fans** ✨ |

---

## 🔧 Technical Details

### Why Separate Files?

Each mode has its own main file to avoid ROM overflow issues:

- **8bitworkshop IDE** compiles everything linked in the main file
- Having all three modes in one file would exceed ROM capacity
- Separate files = only one mode compiled at a time
- Simple and clean - no configuration needed!

### File Structure

```
sokoban_8x8.c       ← Main file for 8×8 mode
  ├─ #link "atari_conio.c"
  ├─ #link "atari_font.c"
  └─ #link "sokoban_game.c"

sokoban_16x16.c     ← Main file for 16×16 mode
  ├─ #link "atari_conio_16x16.c"
  ├─ #link "atari_font_16x16.c"
  └─ #link "sokoban_game_16x16.c"

sokoban_mode6.c     ← Main file for Mode 6 color
  ├─ #link "atari_conio_mode6.c"
  ├─ #link "atari_font_mode6.c"
  └─ #link "sokoban_game_mode6.c"
```

Each main file links only the libraries it needs!

---

## 🎨 Color Mode Details

### ANTIC Mode 6 Specifications

- **Display Mode:** ANTIC Mode 6 (Graphics 1)
- **Resolution:** 20 columns × 24 rows
- **Character Size:** 8×8 pixels
- **Screen Size:** 160×192 pixels
- **Color Registers:** 5 (COLBK + PF0-PF3)

### Color Palette

```
Background (COLBK):  Black   (0x00)
PF0:                 Brown   (0x14) - Walls, boxes
PF1:                 Green   (0xC4) - Player
PF2:                 Yellow  (0xE8) - Goals
PF3:                 Red     (0x32) - Boxes on goals
```

### How Colors Work

- Characters 0-127 use **PF0 color** (brown)
- Characters 128-255 use **PF1 color** (green)
- Special characters can use PF2 or PF3

---

## 🐛 Troubleshooting

### Game doesn't load
- Make sure you're using the 8bitworkshop IDE
- Try refreshing the page
- Check that the correct file is selected

### ROM overflow error
- You're probably trying to compile the old `sokoban.c` file
- Use one of the new files instead:
  - `sokoban_8x8.c`
  - `sokoban_16x16.c`
  - `sokoban_mode6.c`

### Colors don't appear
- Make sure you opened `sokoban_mode6.c` (not the other files)
- The 8×8 and 16×16 modes are monochrome only

### Graphics look wrong
- Each mode has different graphics
- Make sure you're using the correct main file
- Try reloading the page

---

## 📚 More Information

- **[README.md](README.md)** - Project overview
- **[MODE6_COLOR_GUIDE.md](MODE6_COLOR_GUIDE.md)** - Complete Mode 6 guide
- **[TILE_SYSTEM_GUIDE.md](TILE_SYSTEM_GUIDE.md)** - Tile system details
- **[CHANGELOG.md](CHANGELOG.md)** - Version history

---

## 🎉 Enjoy!

Pick your favorite mode and start playing!

**Recommended:** Try the **Mode 6 Color** version for the best visual experience! 🌈

**[▶️ Play Mode 6 Color Now!](http://8bitworkshop.com/redir.html?platform=atari8-800&githubURL=https%3A%2F%2Fgithub.com%2Fastroforgit%2Fsokobanc&file=sokoban_mode6.c)** ✨

