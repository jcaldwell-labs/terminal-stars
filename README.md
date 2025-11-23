# Terminal Stars

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CI](https://github.com/jcaldwell-labs/terminal-stars/actions/workflows/ci.yml/badge.svg)](https://github.com/jcaldwell-labs/terminal-stars/actions/workflows/ci.yml)

A terminal-based 3D space combat simulator written in C using frame buffering. Features 6-DOF flight physics, weapons systems, five game modes including skeet shooting, and USB joystick support.

## Overview

Terminal Stars provides smooth, 60fps space combat and flight simulation directly in your terminal. Similar to the `boxes-live` project, it uses efficient frame buffering and ncurses for terminal manipulation to create immersive 3D flight experiences with multiple gameplay modes.

## Features

- **🎮 Five Game Modes**:
  - **Single Player**: Fight against AI opponent with orbital behavior
  - **Dual Player**: Competitive two-player space combat
  - **Co-op Mode**: Team up with a friend for formation flying
  - **Training Simulator**: Practice flight and weapons with target patterns
  - **Skeet Shooting**: Clay pigeon shooting with realistic ballistic physics

- **🕹️ USB Joystick Support**:
  - Supports up to 2 USB gamepads/joysticks simultaneously
  - 8-direction analog stick control with dead zones
  - Button mapping for all flight controls
  - Haptic feedback (rumble) on weapon fire
  - Works with Xbox, PlayStation, and generic USB controllers

- **✈️ Advanced Flight Physics**:
  - Full 6 degrees of freedom (6-DOF) flight model
  - Realistic pitch, yaw, and roll controls
  - Inertial physics with thrust and drag
  - Multiple camera views (cockpit, chase, free)

- **🚀 Weapons System**:
  - Guided missile system
  - Visual explosion effects
  - Fire cooldown and ammunition tracking
  - Collision detection

- **🎯 Training Mode**:
  - Multiple target patterns (static, moving, orbiting, random)
  - Score tracking and accuracy statistics
  - Four difficulty patterns to practice
  - Real-time performance feedback

- **🌟 Visual Effects**:
  - **Six Starfield Effects**: Linear, Spiral, Warp, Tunnel, Explode, Wave
  - **Horizon Rendering**: Ground and sky visualization (Skeet mode)
  - Real-time effect switching with Tab key
  - Adjustable speed parameters with [ and ] keys

- **📊 Advanced HUD**:
  - 6-DOF attitude indicator
  - Enemy direction indicator
  - Radar overlay
  - Weapons status and cooldown
  - Speed and health displays

- **⚡ Performance**:
  - Double-buffered rendering for smooth 60fps animation
  - Efficient terminal I/O to minimize flicker
  - Handles terminal resize dynamically

## Quick Start

### Prerequisites

**Required:**
- GCC compiler
- ncurses library
  ```bash
  # Debian/Ubuntu
  sudo apt-get install libncurses-dev

  # macOS
  brew install ncurses
  ```

**Optional (for joystick support):**
- SDL2 library
  ```bash
  # Debian/Ubuntu
  sudo apt-get install libsdl2-dev

  # macOS
  brew install sdl2
  ```

### Building

```bash
make              # Build the application
make run          # Build and run
make test         # Run unit tests
make benchmark    # Run performance benchmarks
make validate     # Validate terminal compatibility
make install      # Install to /usr/local (or custom PREFIX)
make clean        # Clean build artifacts
make help         # Show all available targets
```

**Note**: The game will automatically detect if SDL2 is available. If not found, it will compile without joystick support (keyboard controls will still work).

### Controls

#### Keyboard Controls

**Player 1:**
- **W/S**: Pitch up/down
- **A/D**: Yaw left/right
- **Q/E**: Roll left/right
- **Space**: Thrust
- **F**: Fire weapon
- **V**: Toggle camera view

**Player 2:**
- **Arrow Keys**: Pitch and Yaw
- **< / >**: Roll left/right
- **Enter**: Thrust

**Global:**
- **ESC / Q (uppercase)**: Quit
- **M**: Return to menu
- **Tab**: Cycle starfield effects
- **[ / ]**: Adjust starfield speed
- **B**: Toggle AI mode
- **1-4**: Spawn training targets (in training mode)
- **R**: Reset training (in training mode)

#### Gamepad Controls

- **Left Stick**: Pitch and Yaw
- **Right Stick**: Roll
- **Left/Right Bumpers**: Roll
- **Right Trigger / A Button**: Thrust
- **X Button**: Fire weapon
- **Y Button**: Toggle camera view

See [FEATURES.md](FEATURES.md) for complete feature documentation and [MODE-GUIDE.md](MODE-GUIDE.md) for detailed gameplay guides for each mode.

## Project Status

✅ **Fully Functional** - All core features implemented including:
- 3D flight physics and rendering with 6-DOF
- Dual-missile weapons system with explosions
- Five distinct game modes
- USB joystick support (up to 2 controllers)
- Training simulator with four target patterns
- Skeet shooting mode with realistic ballistics
- Advanced HUD with radar and enemy tracking
- Horizon rendering for ground-based modes

## Project Structure

```
terminal-stars/
├── src/                 # Source files
│   ├── main.c           # Application entry point and game loop
│   ├── starfield.c      # Starfield simulation logic
│   ├── render.c         # Frame buffer and 3D rendering
│   ├── terminal.c       # Terminal management
│   ├── input.c          # Keyboard input handling
│   ├── effects.c        # Starfield effect implementations
│   ├── ship.c           # 6-DOF ship physics
│   ├── weapons.c        # Missile and explosion systems
│   ├── gamepad.c        # USB joystick support (SDL2)
│   └── modes.c          # Game modes and training system
├── include/             # Header files
│   ├── types.h          # Core data structures
│   ├── gamepad.h        # Joystick API
│   ├── modes.h          # Game mode API
│   └── ...              # Other headers
├── tests/               # Unit tests
├── demos/               # Demo videos
├── FEATURES.md          # Detailed feature documentation
├── MODE-GUIDE.md        # Complete gameplay guide for all modes
├── CLAUDE.md            # Development guidelines
└── Makefile             # Build configuration
```

## Related Projects

This project shares architectural patterns with [boxes-live](../boxes-live), another terminal-based visualization application.

## License

MIT License - See LICENSE file for details

## Testing

Terminal Stars includes a comprehensive test suite and benchmarking tools:

```bash
make test         # Run all unit tests
make benchmark    # Run performance benchmarks
make validate     # Validate terminal compatibility
```

## Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project.
