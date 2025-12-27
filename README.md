# Kitchen Knight 3D 🎮⚔️

A Doom-style first-person 3D game built with **Raylib** in C. Features an AI-controlled enemy that chases the player around an arena.

![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Windows%20%7C%20Linux-blue)
![Language](https://img.shields.io/badge/Language-C11-green)
![Library](https://img.shields.io/badge/Library-Raylib%205.5-red)

---

## 🎮 Game Features

- **First-person 3D controls** (WASD + mouse look)
- **AI enemy** that chases the player using grid-based pathfinding
- **Arena environment** with floor and walls
- **Collision detection** to keep player within bounds
- **Cross-platform** - runs on macOS, Windows, and Linux

---

## 📁 Project Structure

```
KitchenKnight/
├── README.md               # This file
├── .gitignore              # Git ignore rules
└── raylib_game/            # Main C project
    ├── CMakeLists.txt      # CMake build configuration
    ├── src/
    │   ├── main.c          # Entry point, game loop, window setup
    │   ├── game.h          # Game state structure & constants
    │   ├── game.c          # Core game logic (init, update, draw)
    │   ├── player.h        # Player controller interface
    │   ├── player.c        # First-person movement & camera
    │   ├── enemy.h         # Enemy AI interface
    │   ├── enemy.c         # Chase AI & enemy rendering
    │   ├── arena.h         # Arena interface
    │   └── arena.c         # Floor & walls rendering
    └── build/              # Build output (generated)
```

### File Descriptions

| File | Purpose |
|------|---------|
| `main.c` | Window initialization, main game loop, HUD rendering |
| `game.h/c` | Central game state, constants (arena size, speeds, etc.) |
| `player.h/c` | WASD movement, mouse look, wall collisions |
| `enemy.h/c` | AI that converts positions to grid, chooses movement |
| `arena.h/c` | Draws floor (green) and 4 walls (brown) |

---

## 🛠️ Building & Running

### Prerequisites

- **CMake** 3.15+
- **Raylib** 5.0+ 
- **C Compiler** (Clang, GCC, or MSVC)

---

### 🍎 macOS

#### 1. Install Dependencies
```bash
brew install raylib cmake
```

#### 2. Build
```bash
cd KitchenKnight/raylib_game
mkdir -p build && cd build
cmake ..
make
```

#### 3. Run
```bash
./kitchen_knight
```

---

### 🪟 Windows (MinGW)

#### 1. Install Dependencies
- Download and install [MSYS2](https://www.msys2.org/)
- Open MSYS2 terminal and run:
```bash
pacman -S mingw-w64-x86_64-raylib mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc
```

#### 2. Build
```bash
cd KitchenKnight/raylib_game
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

#### 3. Run
```bash
./kitchen_knight.exe
```

---

### 🪟 Windows (Visual Studio)

#### 1. Install Dependencies
- Install [Visual Studio](https://visualstudio.microsoft.com/) with C++ workload
- Install [vcpkg](https://vcpkg.io/) and run:
```cmd
vcpkg install raylib:x64-windows
```

#### 2. Build
```cmd
cd KitchenKnight\raylib_game
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

#### 3. Run
```cmd
Release\kitchen_knight.exe
```

---

### 🐧 Linux

#### 1. Install Dependencies
```bash
# Ubuntu/Debian
sudo apt install libraylib-dev cmake build-essential

# Arch
sudo pacman -S raylib cmake base-devel

# Fedora
sudo dnf install raylib-devel cmake gcc
```

#### 2. Build
```bash
cd KitchenKnight/raylib_game
mkdir -p build && cd build
cmake ..
make
```

#### 3. Run
```bash
./kitchen_knight
```

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| **W** | Move forward |
| **S** | Move backward |
| **A** | Strafe left |
| **D** | Strafe right |
| **Mouse** | Look around |
| **P** | Pause/Unpause |
| **ESC** | Quit game |

---

## 🔧 Game Constants

These can be adjusted in `src/game.h`:

| Constant | Default | Description |
|----------|---------|-------------|
| `ARENA_SIZE` | 50.0 | Width/depth of the play area |
| `PLAYER_SPEED` | 10.0 | Player movement speed |
| `ENEMY_SPEED` | 3.0 | Enemy chase speed |
| `MOUSE_SENSITIVITY` | 0.003 | Mouse look sensitivity |
| `GRID_SIZE` | 10 | AI grid resolution (10x10) |

---

## 🧠 AI System

The enemy uses a simple **grid-based chase algorithm**:

1. Convert world positions to 10x10 grid coordinates
2. Compare enemy grid position to player grid position  
3. Move toward player on the axis with larger difference
4. Repeat each frame

The grid system is designed to be compatible with trained neural network models (via ONNX) for future enhancement.

---

## 🚀 Future Enhancements

- [ ] ONNX integration for trained PPO model
- [ ] Texture support for more detailed visuals
- [ ] Sound effects and music
- [ ] Multiple enemy types
- [ ] Weapons and combat system
- [ ] Level loading from files

---

## 📄 License

MIT License - feel free to use and modify!

---

## 🤝 Credits

- **Raylib** - Simple and easy-to-use game library by Ramon Santamaria
- Built with assistance from AI pair programming
