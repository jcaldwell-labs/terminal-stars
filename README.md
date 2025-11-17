# Terminal Stars

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A terminal-based starfield visualization application written in C using frame buffering. Features multiple motion effects with interactive controls for panning, zooming, and real-time effect tuning.

## Overview

Terminal Stars provides smooth, 60fps starfield animations directly in your terminal. Similar to the `boxes-live` project, it uses efficient frame buffering and ncurses for terminal manipulation to create various mesmerizing starfield effects.

## Features (Planned)

- **Multiple Starfield Effects**:
  - Linear: Classic starfield motion toward viewer
  - Spiral: Rotating spiral motion
  - Warp Speed: Star Wars hyperspace effect with stretched stars
  - Tunnel: Cylindrical tunnel motion
  - Explode: Radial explosion from center
  - Wave: Sinusoidal wave patterns

- **Interactive Controls**:
  - Arrow keys for panning the view
  - Zoom in/out controls
  - Speed adjustment
  - Real-time effect switching
  - Fine-tune effect parameters

- **Performance**:
  - Double-buffered rendering for smooth 60fps animation
  - Efficient terminal I/O to minimize flicker
  - Handles terminal resize dynamically

## Quick Start

### Prerequisites

- GCC compiler
- ncurses library
  ```bash
  # Debian/Ubuntu
  sudo apt-get install libncurses-dev

  # macOS
  brew install ncurses
  ```

### Building

```bash
make              # Build the application
make run          # Build and run
make test         # Run tests (when available)
make clean        # Clean build artifacts
make help         # Show all available targets
```

### Controls (Planned)

- **Arrow Keys**: Pan the starfield view
- **+ / -**: Zoom in/out
- **[ / ]**: Decrease/increase speed
- **Space**: Cycle through effects
- **R**: Toggle rotation
- **Q**: Quit

## Project Status

🚧 **Under Development** - This project is currently in the planning and initial setup phase. The code structure and placeholder files are in place, ready for implementation.

## Project Structure

```
terminal-stars/
├── src/           # Source files
│   ├── main.c           # Application entry point
│   ├── starfield.c      # Starfield simulation logic
│   ├── render.c         # Frame buffer and rendering
│   ├── terminal.c       # Terminal management
│   ├── input.c          # Keyboard input handling
│   └── effects.c        # Starfield effect implementations
├── include/       # Header files
├── tests/         # Unit tests
├── examples/      # Example configurations
├── demos/         # Demo scripts
└── Makefile       # Build configuration
```

## Related Projects

This project shares architectural patterns with [boxes-live](../boxes-live), another terminal-based visualization application.

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! This project is in early development, so there's plenty of room for improvement and new features.
