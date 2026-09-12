# OpenClaw 2D Platform Game

![CI Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Graphics](https://img.shields.io/badge/Engine-SDL2-red.svg)
![Build System](https://img.shields.io/badge/Build-CMake-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A modernized 2D action platformer inspired by the classic 1997 game **Captain Claw**, engineered with **C++17**, **SDL2**, and **CMake**.

---

## 🌟 Overview & Highlights

This project implements core components of a high-performance 2D platforming engine modeled after the OpenClaw architecture:

- **Enhanced Player Mechanics**:
  - Implemented responsive physics: acceleration, friction, gravity, and variable-height jumping.
  - Added **Coyote Time** (120ms jump grace period after running off ledges) and **Jump Buffering** (100ms early jump input detection).
  - Combat system with directional melee sword slashes, active hitboxes, and knockback forces.
  - Health system with damage invulnerability flashing and item recovery.

- **Collision Detection System**:
  - Custom Axis-Aligned Bounding Box (**AABB**) intersection algorithms.
  - Multi-axis separation logic to prevent tunneling and edge-snapping on vertical wall collisions.
  - Support for solid floors, floating platforms, and hazard spike pits.

- **Optimized SDL2 Rendering & Game Loop**:
  - Fixed **60 FPS** physics timestep loop using high-resolution performance counters (`SDL_GetPerformanceCounter`).
  - Hardware-accelerated 2D rendering pipeline with VSync synchronization (`SDL_RENDERER_PRESENTVSYNC`).
  - Dynamic camera tracking with smooth linear interpolation (Lerp) and boundary clamping.
  - Parallax dungeon backdrop rendering with multi-layer depth.

- **Enemy AI & Level Architecture**:
  - Platform patrol behaviors with edge detection and wall bouncing for Rat Soldiers and Red-Coat Officers.
  - Collectible treasure item system (Pirate Coins, Ruby Gems, Health Potions).
  - Clean modular C++ design separating Core Engine, Entities, Physics, World, and Graphics.

---

## 🎮 Controls

| Action | Primary Key | Alternate Key |
| :--- | :--- | :--- |
| **Move Left** | `A` | `Left Arrow` |
| **Move Right** | `D` | `Right Arrow` |
| **Jump** (Hold for higher jump) | `Space` | `W` / `Up Arrow` |
| **Sword Slash / Attack** | `J` | `Left Ctrl` / `F` |
| **Restart Level** | `R` | - |
| **Quit Game** | `Esc` | - |

---

## 🏗️ Architecture & Class Design

```text
OpenClaw::Game
  ├── OpenClaw::Renderer  ──> SDL2 Hardware Accelerated Pipeline & Camera Lerp
  ├── OpenClaw::Level     ──> Platform geometry, hazards, collectibles & exit
  ├── OpenClaw::Player    ──> State machine, physics, coyote time & sword combat
  ├── OpenClaw::Enemy     ──> Patrol AI, collision, & damage response
  └── OpenClaw::Collision ──> AABB dynamic detection & sweeping resolution
```

---

## 🚀 Building & Running

### Prerequisites
- **CMake** (≥ 3.16)
- **C++17** compatible compiler (MSVC 2019+, GCC 9+, or Clang 10+)
- **SDL2** Development Libraries

### Building on Windows (Visual Studio / CMake)
```bash
# Clone the repository
git clone https://github.com/<your-username>/openclaw-platformer.git
cd openclaw-platformer

# Configure and Build
cmake -B build -S .
cmake --build build --config Release

# Run executable
./build/Release/OpenClawGame.exe
```

### Building on Linux (Ubuntu / Debian)
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y libsdl2-dev cmake build-essential

# Configure and Build
git clone https://github.com/<your-username>/openclaw-platformer.git
cd openclaw-platformer
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# Run executable
./build/OpenClawGame
```

---

## 🧪 Automated CI/CD
This repository includes a multi-platform **GitHub Actions** workflow (`.github/workflows/build.yml`) that automatically compiles and validates the codebase on both **Windows (MSVC)** and **Ubuntu Linux (GCC)** on every push.

---

## 📜 License
This project is licensed under the [MIT License](LICENSE). Inspired by *Captain Claw* (1997 Monolith Productions).
