# Kitchen Knight 3D 🎮⚔️

A Doom-style first-person 3D game built with **Raylib** in C. Battle rogue kitchen appliances with an arsenal of cooking-themed weapons!

![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Windows%20%7C%20Linux-blue)
![Language](https://img.shields.io/badge/Language-C11-green)
![Library](https://img.shields.io/badge/Library-Raylib%205.5-red)

---

## 🎮 Game Features

- **First-person 3D controls** (WASD + mouse look)
- **Combat system** with melee and ranged weapons
- **4 Kitchen-themed weapons** (Spatula, Frying Pan, Ketchup Gun, Egg Launcher)
- **Multiple enemy types** (Toasters, Blenders, Microwaves)
- **Enemy AI** with chase, attack, and hurt states
- **Particle effects** (explosions, hit sparks)
- **Billboard sprite rendering** for 2.5D visual style
- **ASCII map loading** for easy level creation
- **Cross-platform** - runs on macOS, Windows, and Linux

---

## 📁 Project Structure

```
KitchenKnight/
├── README.md                   # This file
├── .gitignore                  # Git ignore rules
└── raylib_game/                # Main C project
    ├── CMakeLists.txt          # CMake build configuration
    ├── assets/
    │   ├── knight.png          # Player sprite
    │   ├── toster.png          # Toaster enemy sprite
    │   └── levels/
    │       └── level1.txt      # Sample ASCII level
    └── src/
        ├── main.c              # Entry point, game loop
        ├── game.h/c            # Game state, init, update, draw
        ├── player.h/c          # FPS movement & camera
        ├── enemy.h/c           # Legacy single enemy AI
        ├── enemies/
        │   └── enemy_types.h/c # Enemy pool system & AI states
        ├── arena.h/c           # Floor & walls rendering
        ├── combat.h/c          # Weapons, hit detection, projectiles
        ├── map_loader.h/c      # ASCII map parsing
        ├── particles.h/c       # Visual effects system
        └── audio.h/c           # Sound management (stubs)
```

---

## 🛠️ Building & Running

### Prerequisites

- **CMake** 3.15+
- **Raylib** 5.0+ 
- **C Compiler** (Clang, GCC, or MSVC)

---

### 🍎 macOS

```bash
# Install dependencies
brew install raylib cmake

# Build
cd KitchenKnight/raylib_game
mkdir -p build && cd build
cmake ..
make

# Run
./kitchen_knight
```

---

### 🪟 Windows (MinGW)

```bash
# Install via MSYS2
pacman -S mingw-w64-x86_64-raylib mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc

# Build
cd KitchenKnight/raylib_game
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .

# Run
./kitchen_knight.exe
```

---

### 🪟 Windows (Visual Studio)

```cmd
# Install vcpkg and raylib
vcpkg install raylib:x64-windows

# Build
cd KitchenKnight\raylib_game
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release

# Run
Release\kitchen_knight.exe
```

---

### 🐧 Linux

```bash
# Ubuntu/Debian
sudo apt install libraylib-dev cmake build-essential

# Arch
sudo pacman -S raylib cmake base-devel

# Build & Run (same as macOS)
```

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| **W/S** | Move forward/backward |
| **A/D** | Strafe left/right |
| **Mouse** | Look around |
| **LMB** | Attack |
| **1** | Equip Spatula (fast melee) |
| **2** | Equip Frying Pan (heavy melee) |
| **3** | Equip Ketchup Gun (rapid ranged) |
| **4** | Equip Egg Launcher (explosive) |
| **P** | Pause/Unpause |
| **ESC** | Quit game |

---

## ⚔️ Weapons

| Weapon | Type | Damage | Cooldown | Special |
|--------|------|--------|----------|---------|
| Spatula | Melee | 10 | 0.4s | Fast attacks |
| Frying Pan | Melee | 25 | 0.8s | High damage |
| Ketchup Gun | Ranged | 8 | 0.2s | Rapid fire |
| Egg Launcher | Ranged | 40 | 1.2s | Explosive |

---

## 🔧 Game Constants

Adjust in `src/game.h`:

| Constant | Default | Description |
|----------|---------|-------------|
| `ARENA_SIZE` | 50.0 | Play area size |
| `PLAYER_SPEED` | 10.0 | Movement speed |
| `ENEMY_SPEED` | 3.0 | Enemy chase speed |
| `MAX_ENEMIES` | 64 | Enemy pool size |

---

## 🗺️ Level Format

Create levels in `assets/levels/` using ASCII:

```
####################
#..................#
#..T...............#
#.........#####....#
#.........#.S.#....#
#.........#####....#
#............T.....#
####################
```

| Symbol | Meaning |
|--------|---------|
| `#` | Wall |
| `.` | Floor |
| `S` | Player start |
| `T` | Toaster spawn |
| `B` | Blender spawn |
| `M` | Microwave spawn |
| `H` | Health pickup |

---

## 🧠 Enemy AI

Enemies use a state machine:
- **IDLE** → Waits for player proximity
- **CHASE** → Moves toward player
- **ATTACK** → Wind-up and attack
- **HURT** → Brief stun when damaged
- **DEAD** → Removed from play

---

## 📦 Implementation Status

- [x] **Phase 1**: Combat Foundation (weapons, hit detection, projectiles)
- [x] **Phase 2**: Enemy System (pool, AI states, multiple types)
- [x] **Phase 3**: Map System (ASCII loading, level generation)
- [x] **Phase 4**: Polish (particles, screen shake)
- [x] **Phase 5**: Audio (stubs ready for sound files)
- [x] **Phase 6**: UI (weapon switching, HUD indicators)

---

## 🚀 Future Enhancements

- [ ] Sound effects (whoosh, splat, explosion)
- [ ] Music tracks
- [ ] More enemy types
- [ ] Power-ups and pickups
- [ ] Multiple levels
- [ ] Boss battles

---

## 📄 License

MIT License - feel free to use and modify!

---

## 🤝 Credits

- **Raylib** - Simple game library by Ramon Santamaria
- Built with AI pair programming
