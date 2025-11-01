# Changelog

## Version 2.0 - 16x16 Tile System (2025-11-01)

### Major Features

#### 🎨 16x16 Tile Support
- Added complete 16x16 pixel tile system using 2x2 character cells
- Each tile now uses 4 characters for detailed graphics
- Screen grid: 20×12 tiles (vs 40×24 in 8x8 mode)

#### 🔄 Dual Tile Mode System
- Single `#define` switch to toggle between 8x8 and 16x16 tiles
- Conditional compilation automatically selects correct libraries
- Same game loop code works for both modes

### New Files

#### 16x16 Tile Libraries
- `atari_font_16x16.h` / `atari_font_16x16.c` - 16x16 graphics system
- `atari_conio_16x16.h` / `atari_conio_16x16.c` - 16x16 console I/O
- `sokoban_game_16x16.h` / `sokoban_game_16x16.c` - 16x16 game logic

#### Documentation
- `TILE_SYSTEM_GUIDE.md` - Complete guide to tile system
- `CHANGELOG.md` - This file

#### Automation Scripts
- `auto-commit-push.sh` - Full-featured auto-commit script
- `quick-push.sh` - Minimal auto-commit script

### Graphics Improvements

#### 16x16 Tile Graphics
- **Wall**: Detailed brick pattern
- **Box**: Wooden crate with cross pattern
- **Goal**: Smooth circular target
- **Box on Goal**: Combined box and target visual
- **Player**: Detailed stick figure with animation

#### Animation System
- Standing frame (idle)
- Walking frame (movement)
- Blinking frame (idle animation)
- All animations work in both 8x8 and 16x16 modes

### API Changes

#### New Functions (16x16 mode)
```c
// Graphics
void setup_graphics_16x16(void);
void animate_player_16x16(void);

// Console I/O
void my_clrscr_16x16(void);
void my_put_tile_16x16(byte tx, byte ty, byte tile_tl);
void my_cputsxy_16x16(byte x, byte y, const char* str);
void wait_vblank_16x16(void);

// Game Logic
void load_level_16x16(const char** d, byte r_num);
void draw_level_16x16(void);
void try_move_player_16x16(signed char dx, signed char dy);
byte is_level_complete_16x16(void);
```

#### Function Aliases
The main file uses preprocessor aliases so the same code works for both modes:
```c
#define setup_graphics      setup_graphics_16x16
#define load_level          load_level_16x16
// ... etc
```

### Level Data

#### Dual Level Support
- 8x8 mode: Original 40×24 level
- 16x16 mode: Adapted 20×12 level
- Conditional compilation selects appropriate level

### Technical Details

#### Memory Usage
- **8x8 mode**: ~256 bytes character set, 960 bytes screen
- **16x16 mode**: ~768 bytes character set, 960 bytes screen

#### Character Mapping (16x16)
- Wall: 0x40-0x43
- Box: 0x44-0x47
- Goal: 0x48-0x4B
- Box on Goal: 0x4C-0x4F
- Player: 0x50-0x53
- Player on Goal: 0x54-0x57

### How to Use

#### Switch to 16x16 Mode
In `sokoban.c`, uncomment:
```c
#define USE_BIG_TILES
```

#### Switch to 8x8 Mode
In `sokoban.c`, comment out:
```c
//#define USE_BIG_TILES
```

### Compatibility

- ✅ Works on Atari 800/XL/XE
- ✅ Compiles with CC65
- ✅ Runs in 8bitworkshop IDE
- ✅ Both tile modes fully functional
- ✅ Same joystick controls
- ✅ Same game mechanics

---

## Version 1.0 - Modular Refactoring (2025-11-01)

### Initial Release

#### Refactoring
- Split monolithic 180-line file into modular libraries
- Created 3 reusable libraries
- Reduced main file to 86 lines (52% reduction)

#### Libraries Created
- `atari_conio.h` / `atari_conio.c` - Console I/O
- `atari_font.h` / `atari_font.c` - Font and graphics
- `sokoban_game.h` / `sokoban_game.c` - Game logic

#### Documentation
- `REFACTORING.md` - Technical documentation
- `LIBRARY_USAGE.md` - Usage guide
- `REFACTORING_SUMMARY.md` - Before/after comparison
- `README.md` - Project overview

#### Features
- Custom 8×8 character graphics
- Player animation (walking, blinking)
- Joystick controls
- Move counter
- Win detection
- Status display

---

## File Structure

```
sokobanc/
├── sokoban.c                    # Main game file with tile mode switch
│
├── 8x8 Tile Libraries
│   ├── atari_conio.h/c          # Console I/O
│   ├── atari_font.h/c           # Graphics
│   └── sokoban_game.h/c         # Game logic
│
├── 16x16 Tile Libraries
│   ├── atari_conio_16x16.h/c    # Console I/O for big tiles
│   ├── atari_font_16x16.h/c     # Graphics for big tiles
│   └── sokoban_game_16x16.h/c   # Game logic for big tiles
│
├── Documentation
│   ├── README.md                # Project overview
│   ├── REFACTORING.md           # Technical docs
│   ├── LIBRARY_USAGE.md         # Usage guide
│   ├── REFACTORING_SUMMARY.md   # Comparison
│   ├── TILE_SYSTEM_GUIDE.md     # Tile system guide
│   └── CHANGELOG.md             # This file
│
└── Scripts
    ├── auto-commit-push.sh      # Auto-commit with details
    └── quick-push.sh            # Quick auto-commit
```

---

## Statistics

### Code Metrics

| Metric | Version 1.0 | Version 2.0 |
|--------|-------------|-------------|
| **Total Files** | 10 | 17 |
| **Library Files** | 6 | 12 |
| **Documentation** | 4 | 5 |
| **Main File Lines** | 86 | 146 |
| **Tile Modes** | 1 (8x8) | 2 (8x8 + 16x16) |
| **Graphics Detail** | Simple | Simple + Detailed |

### Lines of Code

| Component | 8x8 Mode | 16x16 Mode |
|-----------|----------|------------|
| **Console I/O** | ~60 lines | ~60 lines |
| **Graphics** | ~120 lines | ~220 lines |
| **Game Logic** | ~180 lines | ~200 lines |
| **Main File** | 146 lines | 146 lines |

---

## Future Enhancements

### Planned Features
- [ ] Multiple levels
- [ ] Level selection menu
- [ ] Undo functionality
- [ ] Save/load game state
- [ ] High score tracking
- [ ] Sound effects
- [ ] Music

### Possible Improvements
- [ ] 32x32 tile mode (4x4 character cells)
- [ ] Scrolling levels
- [ ] Multiplayer mode
- [ ] Level editor
- [ ] Custom color schemes

---

## Credits

- **Platform**: Atari 800/XL/XE (6502 CPU)
- **Compiler**: CC65
- **IDE**: 8bitworkshop
- **Graphics**: Custom 8×8 and 16×16 pixel art
- **Repository**: https://github.com/astroforgit/sokobanc

---

## License

Same as original Sokoban implementation.

