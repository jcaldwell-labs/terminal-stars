# Terminal Stars

[![CI](https://github.com/jcaldwell-labs/terminal-stars/actions/workflows/ci.yml/badge.svg)](https://github.com/jcaldwell-labs/terminal-stars/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](<https://en.wikipedia.org/wiki/C_(programming_language)>)

**An educational 3D starfield visualizer for the terminal.**

Terminal Stars demonstrates core concepts of terminal graphics programming in clean, well-commented C code:

- **3D Perspective Projection** - Convert 3D positions to 2D screen coordinates
- **Camera Transformations** - Rotate the view using Euler angles (yaw, pitch, roll)
- **Double Buffering** - Flicker-free terminal animation
- **Delta-Time Animation** - Frame-rate independent movement
- **Visual Effects** - Six different animation techniques

> **Looking for the full flight combat simulator?**
> See [atari-style](https://github.com/jcaldwell-labs/atari-style) for the complete terminal shader program.

## Demo

```
           *        .    *         +
      .         *            *          .
                     *   .        '
         *    +              *        *
    .              .    *         .
            *           +     *
      +         *   .              *
           .              *    .
         *      +    .         *    '
```

## Quick Start

```bash
# Build
make

# Run with defaults
./terminal-stars

# Run with custom options
./terminal-stars --stars 1000 --speed 1.5 --effect spiral
```

### Requirements

- GCC compiler
- ncurses library (`sudo apt install libncurses-dev`)

## Usage

```
./terminal-stars [options]

Options:
  -s, --stars N     Number of stars (default: 500)
  -p, --speed F     Speed multiplier 0.1-5.0 (default: 1.0)
  -e, --effect E    Effect name or 1-6 (default: linear)
  -z, --zoom F      Zoom level 10-200 (default: 50.0)
  -h, --help        Show help
```

## Keyboard Controls

| Key       | Action                  |
| --------- | ----------------------- |
| **Tab**   | Cycle to next effect    |
| **1-6**   | Select effect directly  |
| **+/-**   | Increase/decrease speed |
| **[/]**   | Decrease/increase zoom  |
| **Q/ESC** | Quit                    |

## The Six Effects

| #   | Effect      | Technique                           |
| --- | ----------- | ----------------------------------- |
| 1   | **Linear**  | Basic Z-axis translation            |
| 2   | **Spiral**  | Polar coordinate rotation           |
| 3   | **Warp**    | Fast motion with character morphing |
| 4   | **Tunnel**  | Constrained cylindrical motion      |
| 5   | **Explode** | Vector-based directional expansion  |
| 6   | **Wave**    | Sinusoidal oscillation              |

## Code Structure

```
terminal-stars/
├── src/
│   ├── main.c       (358 lines)  Main loop, CLI, input handling
│   ├── effects.c    (270 lines)  Six visual effect implementations
│   ├── starfield.c  (206 lines)  Star management and recycling
│   ├── render.c     (273 lines)  3D projection and frame buffer
│   └── terminal.c   (137 lines)  ncurses wrapper
├── include/
│   ├── types.h      Core data structures (Star, Camera, Starfield)
│   ├── effects.h    Effect function declarations
│   ├── render.h     Frame buffer interface
│   ├── starfield.h  Starfield management interface
│   └── terminal.h   Terminal abstraction
└── tests/
    ├── test_starfield.c  Unit tests
    └── test_render.c     Render tests
```

**~1,500 lines total** (including educational comments)

## Learning Path

### 1. Start with `types.h`

Understand the core data structures: `Star`, `Camera`, `Starfield`, `FrameBuffer`.

### 2. Read `terminal.c`

See how ncurses is initialized for smooth animation.

### 3. Study `render.c`

Learn the 3D-to-2D projection formula and camera transformations.

### 4. Explore `effects.c`

See six different animation techniques with detailed comments.

### 5. Follow `main.c`

Understand the main loop: input → update → render → display.

## Key Concepts Explained

### Perspective Projection

```c
// The key formula for 3D-to-2D projection:
screen_x = center_x + (view_x / view_z) * zoom;
screen_y = center_y + (view_y / view_z) * zoom;

// Division by z makes distant objects smaller
```

### Camera Rotation (Euler Angles)

```c
// Yaw: rotate around Y axis (turn left/right)
temp_x = view_x * cos(-yaw) - view_z * sin(-yaw);

// Pitch: rotate around X axis (look up/down)
temp_z = view_z * cos(-pitch) - view_y * sin(-pitch);

// Roll: rotate around Z axis (tilt head)
temp_x = view_x * cos(-roll) - view_y * sin(-roll);
```

### Delta-Time Animation

```c
// Movement scaled by time = consistent speed regardless of frame rate
star->z -= speed * delta_time;
```

## Build Commands

```bash
make              # Build the application
make run          # Build and run
make test         # Run unit tests
make benchmark    # Run performance benchmarks
make clean        # Remove build artifacts
make help         # Show all targets
```

## Related Projects

| Project                                                      | Description                                        |
| ------------------------------------------------------------ | -------------------------------------------------- |
| [atari-style](https://github.com/jcaldwell-labs/atari-style) | Full terminal shader program with flight simulator |
| [boxes-live](https://github.com/jcaldwell-labs/boxes-live)   | Terminal UI library                                |

## Contributing

Contributions welcome! This is an educational project - improvements to code clarity and documentation are especially appreciated.

## License

MIT License - See [LICENSE](LICENSE) file for details.

---

_A clean, educational demonstration of terminal graphics in C._
