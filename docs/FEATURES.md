# Terminal Stars - Feature Documentation

## Overview

Terminal Stars is an educational 3D starfield visualizer demonstrating core terminal graphics concepts in ~1,500 lines of clean, well-commented C code. This document explains the key features and techniques used.

For the complete terminal shader program with flight simulation and combat systems, see [atari-style](https://github.com/jcaldwell-labs/atari-style).

## Core Features

### 3D Starfield Rendering

The starfield creates the illusion of traveling through space by:

1. **Random Star Distribution** - Stars are placed randomly in a 3D volume
2. **Perspective Projection** - Distant stars appear smaller and closer to center
3. **Depth-Based Brightness** - Closer stars are brighter (white/yellow)
4. **Star Recycling** - Stars that pass the camera respawn at the far distance

```c
// Perspective projection: divide by Z to create depth effect
screen_x = center_x + (view_x / view_z) * zoom;
screen_y = center_y + (view_y / view_z) * zoom;
```

### Six Visual Effects

Each effect demonstrates a different animation technique:

| Effect      | Technique                           | Key Concept              |
| ----------- | ----------------------------------- | ------------------------ |
| **Linear**  | Basic Z-axis translation            | Simple movement          |
| **Spiral**  | Polar coordinate rotation           | Coordinate transforms    |
| **Warp**    | Fast motion with character morphing | Speed-based visuals      |
| **Tunnel**  | Constrained cylindrical motion      | Bounded coordinate space |
| **Explode** | Vector-based directional expansion  | Direction vectors        |
| **Wave**    | Sinusoidal oscillation              | Trigonometric animation  |

### Double-Buffered Rendering

Prevents screen flicker during animation:

```c
// Frame buffer structure
typedef struct {
    char *buffer;    // Character array (width * height)
    int *colors;     // Color indices (width * height)
    int width, height;
} FrameBuffer;
```

**Process:**

1. Clear the back buffer
2. Render all stars to the buffer
3. Display the complete buffer at once

### Delta-Time Animation

Ensures consistent animation speed regardless of frame rate:

```c
// Calculate time since last frame
double delta_time = current_time - last_time;

// Scale all movement by delta_time
star->z -= speed * delta_time;
```

This means the starfield moves at the same apparent speed whether running at 30 FPS or 120 FPS.

### Camera System

The camera supports full 3D rotation using Euler angles:

- **Yaw** - Rotation around Y-axis (turn left/right)
- **Pitch** - Rotation around X-axis (look up/down)
- **Roll** - Rotation around Z-axis (tilt head)

```c
// Camera structure
typedef struct {
    double pos_x, pos_y, pos_z;  // Camera position
    double yaw, pitch, roll;      // Rotation angles in radians
} Camera;
```

### CLI Parameter Control

Full runtime configuration via command-line:

```bash
./terminal-stars --stars 1000 --speed 2.0 --effect spiral --zoom 75
```

| Parameter  | Range   | Default | Purpose               |
| ---------- | ------- | ------- | --------------------- |
| `--stars`  | 10-5000 | 500     | Number of stars       |
| `--speed`  | 0.1-5.0 | 1.0     | Animation speed       |
| `--effect` | 1-6     | linear  | Visual effect         |
| `--zoom`   | 10-200  | 50.0    | Perspective intensity |

### Keyboard Controls

Interactive control during runtime:

| Key   | Action                 |
| ----- | ---------------------- |
| Tab   | Cycle to next effect   |
| 1-6   | Select effect directly |
| +/-   | Adjust speed           |
| [/]   | Adjust zoom            |
| Q/ESC | Quit                   |

### Color System

Four color pairs for depth perception:

| Color  | Pair | Usage                |
| ------ | ---- | -------------------- |
| White  | 1    | Closest stars        |
| Cyan   | 2    | Medium distance      |
| Blue   | 3    | Distant stars        |
| Yellow | 4    | Very close/highlight |

### Terminal Resize Handling

Automatic adaptation to terminal size changes:

1. SIGWINCH signal triggers resize detection
2. Frame buffer is reallocated to new dimensions
3. Rendering continues seamlessly

## Technical Implementation

### Performance Targets

- **Frame Rate**: 60 FPS target
- **Frame Time**: ~16.67ms per frame
- **Update Cycle**: input -> update -> render -> display

### Memory Management

- **Static Allocation**: Star array allocated once at startup
- **No Runtime Allocation**: No malloc/free during animation loop
- **Fixed Buffers**: Frame buffer sized to terminal dimensions

### Dependencies

| Library     | Purpose                     |
| ----------- | --------------------------- |
| **ncurses** | Terminal control and colors |
| **libm**    | Math functions (sin, cos)   |

### Build System

```bash
make              # Build the application
make run          # Build and run
make test         # Run unit tests
make benchmark    # Performance tests
make clean        # Remove build artifacts
```

## Learning Resources

### Recommended Reading Order

1. **types.h** - Core data structures
2. **terminal.c** - ncurses initialization
3. **render.c** - 3D projection math
4. **effects.c** - Animation techniques
5. **main.c** - Application loop

### Key Algorithms to Study

1. **Perspective Projection** - `render_starfield()` in render.c
2. **Euler Rotation** - Camera transforms in render.c
3. **Delta-Time Scaling** - All effect functions in effects.c
4. **Star Recycling** - `star_recycle()` in starfield.c

## Related Projects

| Project                                                      | Description                                        |
| ------------------------------------------------------------ | -------------------------------------------------- |
| [atari-style](https://github.com/jcaldwell-labs/atari-style) | Full terminal shader program with flight simulator |
| [boxes-live](https://github.com/jcaldwell-labs/boxes-live)   | Terminal UI library                                |
