# GitHub Copilot Instructions

This file provides guidance to GitHub Copilot when working with code in this repository.

## Project Overview

Terminal Stars is a **fully implemented** 3D space combat and flight simulator running in the terminal using ncurses. It features 6-DOF physics, multiple game modes, weapons systems, joystick support, and various visual effects.

Key features:
- Five complete game modes (Single Player, Dual Player, Co-op, Training, Skeet Shooting)
- Full 6-DOF flight physics with realistic momentum and drag
- Dual-missile weapons system with collision detection
- Advanced HUD with radar, enemy tracking, and tactical displays
- USB joystick/gamepad support (up to 2 controllers via SDL2)
- Six starfield visual effects
- Frame-buffered 60fps rendering

## Build System

The project uses GNU Make with conditional SDL2 linking:

```bash
make              # Build the application
make run          # Build and run the application
make test         # Run all unit tests
make benchmark    # Run performance benchmarks
make test-joystick # Test SDL2 joystick detection
make validate     # Validate terminal compatibility
make install      # Install to system (default: /usr/local)
make uninstall    # Uninstall from system
make clean        # Clean build artifacts
make help         # Show all available make targets
```

**Dependencies:**
- **Required**: GCC compiler, ncurses library (`libncurses-dev`)
- **Optional**: SDL2 library (`libsdl2-dev`) for joystick/gamepad support

**Compiler Flags**: `-Wall -Wextra -Werror -std=gnu99`

## Architecture

### Directory Structure

```
terminal-stars/
├── src/              # Source files
│   ├── main.c        # Game loop, HUD rendering, mode management (31KB)
│   ├── ship.c        # 6-DOF flight physics (6KB)
│   ├── weapons.c     # Missile and explosion system (6KB)
│   ├── modes.c       # Game modes and training/skeet logic (20KB)
│   ├── render.c      # Frame buffer and 3D rendering (18KB)
│   ├── starfield.c   # Star array management
│   ├── effects.c     # Six visual effects (11KB total with starfield)
│   ├── input.c       # Keyboard input handling
│   ├── gamepad.c     # SDL2 joystick support (11KB total with input)
│   └── terminal.c    # ncurses initialization (2KB)
├── include/          # Header files
│   ├── types.h       # Core data structures
│   ├── ship.h        # Flight physics API
│   ├── weapons.h     # Weapons system API
│   ├── modes.h       # Game modes API
│   ├── render.h      # Rendering API
│   ├── starfield.h   # Starfield effects API
│   └── gamepad.h     # Joystick API
├── tests/            # Unit tests and benchmarks
│   ├── test_starfield.c
│   ├── test_render.c
│   └── benchmark.c
└── Makefile
```

### Core Components

1. **Flight Physics (ship.c)**: 6-DOF movement with velocity-based physics, angular momentum, and drag
2. **Weapons System (weapons.c)**: Dual missiles with guided trajectories, collision detection, explosions
3. **Rendering (render.c)**: Double-buffered frame buffer, 3D projection pipeline, HUD overlay
4. **Game Modes (modes.c)**: Mode selection, training targets, skeet shooting with clay pigeons
5. **Starfield (starfield.c + effects.c)**: Six visual effects (linear, spiral, warp, tunnel, explode, wave)
6. **Input (input.c + gamepad.c)**: Non-blocking keyboard, SDL2 joystick with deadzone handling

### Key Design Patterns

- **Object Pooling**: Pre-allocated arrays for missiles (100), explosions (50), targets (20-30)
- **Delta-Time Physics**: Frame-independent movement scaled by delta_time
- **State Machines**: Game modes and AI behaviors with clear transitions
- **Camera Transform Pipeline**: Translate → Yaw → Pitch → Roll → Project → Clip

## Code Style and Conventions

- **Standard**: GNU99 (allows POSIX extensions like strdup)
- **Naming**: snake_case for functions/variables, UPPER_CASE for macros/constants
- **Headers**: Guard macros (#ifndef), public API only, minimal includes
- **Memory**: Object pooling with active/inactive flags, no dynamic allocation in game loop
- **Comments**: Descriptive comments for physics calculations and coordinate transforms
- **Modularity**: One concern per module (physics, rendering, input, etc.)

**Physics Conventions**:
- All coordinates use right-handed coordinate system
- Angles stored as radians internally
- Delta-time scaled physics for frame-independent behavior
- Camera transforms applied in order: translate, yaw, pitch, roll

## Before Committing (Required Steps)

Run these commands before every commit:

1. **Build**: `make clean && make` - Ensure zero warnings with -Wall -Wextra -Werror
2. **Test**: `make test` - All unit tests must pass
3. **Manual test**: `./terminal-stars` - Verify basic gameplay works

```bash
# Quick pre-commit check
make clean && make && make test
```

**Optional but recommended**:
- `make benchmark` - Check performance hasn't regressed
- `make validate` - Verify terminal compatibility

## Pull Request Standards

When creating PRs, follow these rules:

1. **Always link the issue**: Use `Fixes #N` or `Closes #N` in the PR body
2. **Fill in all sections**: Never leave placeholder text or empty bullets

**Required PR format:**
```markdown
## Summary
[2-3 sentences describing what and why]

Fixes #[issue-number]

## Changes
- [Actual change 1]
- [Actual change 2]

## Testing
- [x] All tests pass (`make test`)
- [x] Manual gameplay tested

## Type
- [x] New feature | Bug fix | Refactor | Docs | CI
```

## Additional Documentation

For comprehensive project information, see:
- **FEATURES.md** - Complete feature list and capabilities
- **MODE-GUIDE.md** - Detailed guide to all game modes
- **README.md** - Project overview and quick start
