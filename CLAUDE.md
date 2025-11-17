# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Terminal-based starfield motion effects visualization application using frame buffering. Inspired by the `../boxes-live` C implementation. The application provides interactive controls for panning, zooming, and special effects.

## Project Status

Project structure is set up with placeholder files. Implementation is ready to begin. The architecture follows the `boxes-live` pattern with frame buffering and ncurses.

## Core Requirements

Based on README.md:
- **Frame Buffering**: Use double/triple buffering for smooth terminal rendering
- **Starfield Effects**: Various motion effects for visual starfield
- **Interactive Controls**:
  - Arrow keys for panning
  - Zoom controls
  - Style/effect selection
  - Special effects tuning parameters
- **Performance**: Efficient terminal rendering for smooth animation

## Implementation Considerations

### Language Choice
Since this references a C implementation (`boxes-live`), consider:
- **C/C++**: For maximum performance and frame buffer control
- **Rust**: For memory safety with similar performance
- **Go**: For simpler concurrency if frame buffering in goroutines
- **Python with curses**: For rapid prototyping (may have performance limitations)

### Terminal Rendering
- Use appropriate terminal library (ncurses for C, termion/crossterm for Rust, tcell for Go)
- Handle terminal resize events
- Implement efficient diff-based rendering to minimize terminal I/O
- Consider ANSI escape sequences for direct control

### Architecture Suggestions
- **Rendering Pipeline**: Separate computation (starfield simulation) from rendering (frame buffer output)
- **Input Handling**: Non-blocking keyboard input with event loop
- **State Management**: Clean separation between effect parameters and rendering state
- **Effect System**: Pluggable effect modules for different starfield behaviors

## Build Commands

```bash
make              # Build the application
make run          # Build and run the placeholder
make test         # Run unit tests (when implemented)
make clean        # Clean build artifacts
make help         # Show all available make targets
```

## Code Architecture

### Module Structure

- **main.c**: Application entry point, main render loop with 60fps timing
- **starfield.c/h**: Starfield data structure and update logic
- **render.c/h**: Frame buffer management and starfield rendering to buffer
- **terminal.c/h**: Terminal initialization, cleanup, and resize handling
- **input.c/h**: Non-blocking keyboard input with action mapping
- **effects.c/h**: Six effect implementations (linear, spiral, warp, tunnel, explode, wave)

### Key Design Patterns

**Frame Buffering**: Double-buffered rendering to avoid flicker
- Clear buffer → Render to buffer → Display buffer

**Effect System**: Effect functions operate on starfield state
- Each effect modifies star positions based on delta time
- Effects are switchable at runtime

**Coordinate System**: 3D stars projected to 2D screen
- Stars have (x, y, z) positions
- Z-axis represents depth (distance from viewer)
- Projection scales x,y based on z for perspective

## Implementation Roadmap

1. **Phase 1: Foundation**
   - Implement terminal initialization with ncurses
   - Create basic frame buffer structure
   - Add simple star rendering (single dots)

2. **Phase 2: Motion**
   - Implement linear starfield effect
   - Add delta-time-based animation
   - Achieve smooth 60fps rendering

3. **Phase 3: Controls**
   - Add keyboard input handling
   - Implement pan/zoom controls
   - Add speed adjustment

4. **Phase 4: Effects**
   - Implement remaining 5 effects
   - Add effect switching with visual feedback
   - Add effect-specific parameters

5. **Phase 5: Polish**
   - Add color/brightness variation
   - Optimize rendering performance
   - Handle edge cases and terminal resize

## Related Projects

Reference `../boxes-live` for frame buffering implementation patterns.
