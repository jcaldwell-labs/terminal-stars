# Terminal Stars

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A terminal-based 3D space combat simulator written in C using frame buffering. Features 6-DOF flight physics, weapons systems, multiple game modes, and USB joystick support.

## Overview

Terminal Stars provides smooth, 60fps starfield animations directly in your terminal. Similar to the `boxes-live` project, it uses efficient frame buffering and ncurses for terminal manipulation to create various mesmerizing starfield effects.

## Features

- **🎮 Four Game Modes**:
  - **Single Player**: Fight against intelligent AI opponents
  - **Dual Player**: Competitive two-player combat
  - **Co-op Mode**: Team up with a friend
  - **Training Simulator**: Practice flight and weapons skills

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

- **🌟 Multiple Starfield Effects**:
  - Linear, Spiral, Warp, Tunnel, Explode, Wave, Torus
  - Real-time effect switching
  - Adjustable speed parameters

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
make test         # Run tests
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

See [FEATURES.md](FEATURES.md) for complete documentation.

## Project Status

✅ **Fully Functional** - All core features implemented including:
- 3D flight physics and rendering
- Weapons systems
- Four game modes
- USB joystick support
- Training simulator

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
├── CLAUDE.md            # Development guidelines
└── Makefile             # Build configuration
```

## Related Projects

This project shares architectural patterns with [boxes-live](../boxes-live), another terminal-based visualization application.

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! This project is in early development, so there's plenty of room for improvement and new features.
