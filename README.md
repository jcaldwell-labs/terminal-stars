# Terminal Stars

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CI](https://github.com/jcaldwell-labs/terminal-stars/actions/workflows/ci.yml/badge.svg)](https://github.com/jcaldwell-labs/terminal-stars/actions/workflows/ci.yml)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com)

A terminal-based 3D space combat simulator written in C using frame buffering. Features 6-DOF flight physics, weapons systems, five game modes including skeet shooting, and USB joystick support.

## Why Terminal Stars?

**Immersive 3D gameplay directly in your terminal** - no GUI, no browser, no dependencies beyond ncurses.

- **Lightweight**: Runs on any system with a terminal and C compiler
- **Authentic retro feel**: ASCII graphics with modern 60fps rendering
- **Complete game experience**: Five distinct modes from combat to training
- **Controller support**: Play with USB gamepads or keyboard
- **Educational**: Learn 3D rendering, physics, and game architecture in pure C

**Perfect for:**
- Developers wanting a fun break without leaving the terminal
- Retro gaming enthusiasts who appreciate ASCII aesthetics
- Anyone learning C game development patterns
- SSH sessions where GUI games aren't an option

## Demo

```
            *        *                    *
      *                    * . *              *
              ___________
         *   /  RADAR  \        *       *
            |    @    E |     SPEED: 85 m/s
  *         |     |     |     HEALTH: ***
            \_________/      MISSILES: 44
       *              *              [READY]
    *      o     o              *
              [+]           *        *
         *        >====>                   *
```

*Try it yourself - build and run in under 30 seconds!*

## Quick Start

### Prerequisites

```bash
# Debian/Ubuntu
sudo apt-get install libncurses-dev

# macOS
brew install ncurses
```

### Build and Run

```bash
git clone https://github.com/jcaldwell-labs/terminal-stars.git
cd terminal-stars
make
./build/terminal-stars
```

That's it! Select a mode with arrow keys and press Enter.

### Optional: Joystick Support

```bash
# Debian/Ubuntu
sudo apt-get install libsdl2-dev

# macOS
brew install sdl2
```

## Features

### Five Game Modes

| Mode | Description | Best For |
|------|-------------|----------|
| **Single Player** | Combat against AI opponent | Solo practice |
| **Dual Player** | Head-to-head space combat | Competitive play |
| **Co-op** | Team up for formation flying | Learning together |
| **Training** | Target practice with patterns | Skill building |
| **Skeet Shooting** | Clay pigeons with ballistics | Precision practice |

### Flight Physics (6-DOF)

Full 3 degrees of freedom for movement and 3 for rotation:
- **Pitch, Yaw, Roll**: Independent control of all axes
- **Inertial physics**: Momentum-based movement with drag
- **Arcade-style**: Responsive controls with realistic feel
- **Speed limiting**: Automatic clamping to max velocity

### Weapons System

- **Dual missiles**: Fire two per volley with left/right offset
- **Guided targeting**: Missiles track toward crosshair
- **Visual feedback**: Expanding explosion animations
- **Collision detection**: Real-time checks against all targets

### Advanced HUD

- **Radar overlay**: 9x9 grid showing enemies and missiles
- **Enemy tracking**: Direction indicator when off-screen
- **Status displays**: Speed, health, ammo, fire readiness
- **Camera modes**: Cockpit (first-person) and chase (third-person)

### Six Starfield Effects

Cycle through with Tab key:
1. **Linear** - Forward motion through stars
2. **Spiral** - Mesmerizing rotation pattern
3. **Warp** - Hyperspeed with star stretching
4. **Tunnel** - Cylindrical tunnel effect
5. **Explode** - Outward expansion from center
6. **Wave** - Undulating wave motion

## Controls

### Keyboard (Player 1)

| Key | Action |
|-----|--------|
| W/S | Pitch up/down |
| A/D | Yaw left/right |
| Q/E | Roll left/right |
| Space | Thrust |
| F | Fire missiles |
| V | Toggle camera |
| Tab | Cycle effects |
| M | Return to menu |

### Keyboard (Player 2)

| Key | Action |
|-----|--------|
| Arrow Keys | Pitch and Yaw |
| < / > | Roll |
| Enter | Thrust/Fire |

### Gamepad

- **Left stick**: Pitch and yaw
- **Right stick/bumpers**: Roll
- **RT/A**: Thrust
- **X**: Fire
- **Y**: Toggle camera

See [MODE-GUIDE.md](docs/guides/MODE-GUIDE.md) for complete control reference.

## Documentation

| Document | Description |
|----------|-------------|
| [Features](docs/FEATURES.md) | Complete feature documentation |
| [Mode Guide](docs/guides/MODE-GUIDE.md) | Detailed gameplay guide for all modes |
| [Contributing](CONTRIBUTING.md) | How to contribute |
| [Changelog](CHANGELOG.md) | Version history |
| [Roadmap](.github/planning/ROADMAP.md) | Future development plans |

## Build Commands

```bash
make              # Build the application
make run          # Build and run
make test         # Run unit tests
make benchmark    # Run performance benchmarks
make validate     # Validate terminal compatibility
make install      # Install to /usr/local
make clean        # Clean build artifacts
make help         # Show all targets
```

## Project Structure

```
terminal-stars/
├── src/                 # Source files
│   ├── main.c           # Game loop and HUD
│   ├── ship.c           # 6-DOF physics
│   ├── weapons.c        # Missiles and explosions
│   ├── modes.c          # Game modes
│   ├── render.c         # 3D rendering
│   ├── gamepad.c        # Joystick support
│   └── ...
├── include/             # Headers
├── tests/               # Unit tests
├── docs/                # Documentation
│   ├── guides/          # How-to guides
│   └── FEATURES.md      # Feature reference
└── .github/
    └── planning/        # Roadmap and backlog
```

## Requirements

- **Compiler**: GCC or Clang with C99 support
- **Libraries**: ncurses (required), SDL2 (optional for gamepad)
- **Terminal**: 80x24 minimum, 8+ colors
- **Performance**: Targets 60 FPS

Tested on: Linux (Ubuntu, Debian), macOS, WSL2

## Contributing

Contributions welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Quick Contribution Guide

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes
4. Push to the branch
5. Open a Pull Request

## Community

- **Issues**: [GitHub Issues](https://github.com/jcaldwell-labs/terminal-stars/issues)
- **Discussions**: [GitHub Discussions](https://github.com/jcaldwell-labs/terminal-stars/discussions)

## Related Projects

- [boxes-live](https://github.com/jcaldwell-labs/boxes-live) - Terminal-based box visualization (shared frame buffering patterns)

## License

MIT License - See [LICENSE](LICENSE) file for details.

---

**Built with love for the terminal.**
