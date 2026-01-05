# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Terminal Stars is an **educational 3D starfield visualizer** running in the terminal using ncurses. It demonstrates core concepts of terminal graphics programming in clean, well-commented C code (~1,500 lines).

This project is intentionally minimal and focused on teaching. For the complete terminal shader program with flight simulation and combat systems, see [atari-style](https://github.com/jcaldwell-labs/atari-style).

## Project Status

**EDUCATIONAL VISUALIZER** - A clean, focused codebase demonstrating:

- 3D perspective projection (converting 3D positions to 2D screen)
- Camera transformations (rotation using Euler angles)
- Double-buffered rendering (flicker-free terminal animation)
- Delta-time animation (frame-rate independent movement)
- Six visual effects (linear, spiral, warp, tunnel, explode, wave)
- CLI parameter control (--stars, --speed, --effect, --zoom)
- Interactive keyboard controls

## Build Commands

```bash
make              # Build the application
make run          # Build and run
make test         # Run unit tests
make benchmark    # Run performance benchmarks
make clean        # Clean build artifacts
make help         # Show all available targets
```

### Requirements

- GCC compiler
- ncurses library (`sudo apt install libncurses-dev`)

## Code Architecture

### File Structure (~1,500 lines total)

```
terminal-stars/
├── src/
│   ├── main.c       (358 lines)  Main loop, CLI, input handling
│   ├── effects.c    (270 lines)  Six visual effect implementations
│   ├── starfield.c  (206 lines)  Star management and recycling
│   ├── render.c     (273 lines)  3D projection and frame buffer
│   └── terminal.c   (137 lines)  ncurses wrapper
├── include/
│   ├── types.h      (88 lines)   Core data structures
│   ├── effects.h    (67 lines)   Effect declarations
│   ├── starfield.h  Starfield management interface
│   ├── render.h     Frame buffer interface
│   └── terminal.h   Terminal abstraction
└── tests/
    ├── test_starfield.c  Unit tests
    ├── test_render.c     Render tests
    └── benchmark.c       Performance benchmarks
```

### Core Data Structures (types.h)

```c
// A single star in 3D space
typedef struct {
    double x, y, z;       // 3D position
    uint8_t brightness;   // 1-4 for color selection
    char character;       // Display character (*, ., +, ')
} Star;

// Camera for view transformations
typedef struct {
    double pos_x, pos_y, pos_z;  // Position in 3D space
    double yaw, pitch, roll;      // Euler angles for rotation
} Camera;

// The starfield with all rendering parameters
typedef struct {
    Star *stars;
    size_t star_count;
    double speed;
    Camera camera;
    double zoom;
    int effect_mode;
} Starfield;

// Double-buffered frame for flicker-free rendering
typedef struct {
    char *buffer;    // Character buffer
    int *colors;     // Color pair indices
    int width, height;
} FrameBuffer;
```

### Module Responsibilities

**main.c** - Application entry point

- CLI argument parsing with getopt_long
- Main loop: input -> update -> render -> display
- Keyboard control handling
- HUD overlay display

**effects.c** - Six animation techniques

- `effect_linear()` - Basic Z-axis translation
- `effect_spiral()` - Polar coordinate rotation
- `effect_warp()` - Fast motion with character morphing
- `effect_tunnel()` - Constrained cylindrical motion
- `effect_explode()` - Vector-based directional expansion
- `effect_wave()` - Sinusoidal oscillation

**render.c** - 3D graphics pipeline

- Frame buffer management (create, clear, display, destroy)
- 3D-to-2D perspective projection
- Camera rotation transforms (yaw, pitch, roll)
- Depth-based brightness selection

**starfield.c** - Star lifecycle

- Star array allocation and initialization
- Star recycling when they leave view
- Effect-specific update dispatching

**terminal.c** - ncurses abstraction

- Terminal initialization with proper modes
- Color pair setup (white, cyan, blue, yellow)
- Resize detection via SIGWINCH

### Key Algorithms

**Perspective Projection** (render.c):

```c
// The core formula: distant objects appear smaller
screen_x = center_x + (view_x / view_z) * zoom;
screen_y = center_y + (view_y / view_z) * zoom;
```

**Camera Rotation** (render.c):

```c
// Apply Euler angles in order: yaw -> pitch -> roll
// Yaw: rotate around Y axis
temp_x = view_x * cos(-yaw) - view_z * sin(-yaw);
// Pitch: rotate around X axis
temp_z = view_z * cos(-pitch) - view_y * sin(-pitch);
// Roll: rotate around Z axis
temp_x = view_x * cos(-roll) - view_y * sin(-roll);
```

**Delta-Time Animation** (effects.c):

```c
// Movement scaled by elapsed time = consistent speed at any frame rate
star->z -= speed * delta_time;
```

## CLI Options

```
./terminal-stars [options]

Options:
  -s, --stars N     Number of stars (default: 500, range: 10-5000)
  -p, --speed F     Speed multiplier (default: 1.0, range: 0.1-5.0)
  -e, --effect E    Effect name or number 1-6 (default: linear)
  -z, --zoom F      Zoom level (default: 50.0, range: 10-200)
  -h, --help        Show help message
```

## Keyboard Controls

| Key   | Action                  |
| ----- | ----------------------- |
| Tab   | Cycle to next effect    |
| 1-6   | Select effect directly  |
| +/-   | Increase/decrease speed |
| [/]   | Decrease/increase zoom  |
| Q/ESC | Quit                    |

## Learning Path

1. **Start with types.h** - Understand the core data structures
2. **Read terminal.c** - See how ncurses is initialized
3. **Study render.c** - Learn the 3D-to-2D projection formula
4. **Explore effects.c** - See six different animation techniques
5. **Follow main.c** - Understand the main loop structure

## Related Projects

| Project                                                      | Description                                        |
| ------------------------------------------------------------ | -------------------------------------------------- |
| [atari-style](https://github.com/jcaldwell-labs/atari-style) | Full terminal shader program with flight simulator |
| [boxes-live](https://github.com/jcaldwell-labs/boxes-live)   | Terminal UI library                                |

## Enhancement Opportunities

This is an educational project. Simple enhancements that maintain clarity:

1. **Additional effects** - Implement new visual patterns
2. **Color schemes** - Add different color palettes
3. **Star shapes** - Vary characters based on distance
4. **Mouse support** - Camera control with mouse
5. **Config file** - Load settings from file

Keep changes focused and well-documented to maintain educational value.
