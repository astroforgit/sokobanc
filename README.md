# Sokoban for Atari 8-bit 🎮

A colorful Sokoban puzzle game for the Atari 8-bit computer family, featuring **three display modes** including a new **5-color ANTIC Mode 6**!

**[▶️ Open this project in 8bitworkshop](http://8bitworkshop.com/redir.html?platform=atari8-800&githubURL=https%3A%2F%2Fgithub.com%2Fastroforgit%2Fsokobanc&file=sokoban.c)**

---

## ✨ NEW: ANTIC Mode 6 Color Support!

The game now supports **5 vibrant colors** using ANTIC Mode 6:

- 🟤 **Brown**: Walls and boxes
- 🟢 **Green**: Player character
- 🟡 **Yellow**: Goal targets
- 🔴 **Red**: Boxes on goals
- ⚫ **Black**: Background

**Enable color mode** by editing `sokoban.c` line 19:
```c
#define USE_MODE6_COLOR    // 5-color mode! ✨
```

---

## 🎨 Three Display Modes

| Mode | Grid | Colors | Description |
|------|------|--------|-------------|
| **8×8** | 40×24 | 1+BG | Classic monochrome (default) |
| **16×16** | 20×12 | 1+BG | Big detailed tiles |
| **Mode 6** ✨ | 20×24 | **5** | **Colorful graphics!** |

---

## 🚀 Quick Start

1. **[Open in 8bitworkshop](http://8bitworkshop.com/redir.html?platform=atari8-800&githubURL=https%3A%2F%2Fgithub.com%2Fastroforgit%2Fsokobanc&file=sokoban.c)**
2. Edit `sokoban.c` to choose display mode (line 19)
3. Play with arrow keys or joystick!

**Goal:** Push all boxes ($) onto goal targets (.)

---

## 📚 Documentation

- **[MODE6_COLOR_GUIDE.md](MODE6_COLOR_GUIDE.md)** - Complete color mode guide ✨
- **[QUICK_START.md](QUICK_START.md)** - Quick start for all modes
- **[COLOR_TEXT_MODES.md](COLOR_TEXT_MODES.md)** - Atari display modes reference
- **[TILE_SYSTEM_GUIDE.md](TILE_SYSTEM_GUIDE.md)** - Tile system guide
- **[CHANGELOG.md](CHANGELOG.md)** - Version history

---

## 🛠️ Features

- ✅ Three display modes (8×8, 16×16, Mode 6 color)
- ✅ Classic Sokoban puzzle mechanics
- ✅ Custom character graphics
- ✅ Player animation
- ✅ Move counter and progress tracking
- ✅ Joystick and keyboard controls
- ✅ Modular, well-documented code

---

## 📊 Version 3.0

**Latest Release:** ANTIC Mode 6 Color Support (2025-11-01)

- 🎨 5-color display mode
- 🌈 Brown, green, yellow, red color scheme
- 📁 Complete Mode 6 library suite
- 📖 Comprehensive documentation

**Code Metrics:**
- ~3,500 total lines
- 29 files
- 3 display modes
- Up to 5 colors

---

## 🔗 Links

- **Play Online**: [8bitworkshop IDE](http://8bitworkshop.com/redir.html?platform=atari8-800&githubURL=https%3A%2F%2Fgithub.com%2Fastroforgit%2Fsokobanc&file=sokoban.c)
- **Repository**: https://github.com/astroforgit/sokobanc
- **8bitworkshop**: https://8bitworkshop.com/

---

**Enjoy colorful retro puzzle gaming!** 🎨🎮✨
