# Contributing to Terminal Stars

Thank you for your interest in contributing to Terminal Stars! This document provides guidelines and information for contributors.

## Code of Conduct

Be respectful, constructive, and professional in all interactions. We aim to foster an inclusive and welcoming environment for all contributors.

## Getting Started

### Prerequisites

Before contributing, ensure you have:
- GCC compiler (C99 standard)
- ncurses development library (`libncurses-dev`)
- SDL2 development library (`libsdl2-dev`) - optional, for joystick support
- Git for version control
- Basic understanding of C programming and terminal graphics

### Development Setup

1. **Fork and clone the repository:**
   ```bash
   git clone https://github.com/YOUR_USERNAME/terminal-stars.git
   cd terminal-stars
   ```

2. **Build the project:**
   ```bash
   make
   ```

3. **Run tests:**
   ```bash
   make test
   ```

4. **Run the application:**
   ```bash
   make run
   ```

## Development Workflow

### Branch Strategy

- `main` - Stable, production-ready code
- Feature branches - Use descriptive names: `feature/joystick-support`, `fix/rendering-bug`
- Improvement branches - Use `improve/` prefix: `improve/phase2-improvements`

### Making Changes

1. **Create a feature branch:**
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes:**
   - Follow the existing code style
   - Add comments for complex logic
   - Update documentation as needed

3. **Test your changes:**
   ```bash
   make clean
   make test
   make run
   ```

4. **Commit your changes:**
   ```bash
   git add .
   git commit -m "Brief description of changes"
   ```

5. **Push and create a pull request:**
   ```bash
   git push origin feature/your-feature-name
   ```

## Code Style Guidelines

### C Code Standards

- **Standard**: Use C99 standard (`-std=gnu99`)
- **Indentation**: 4 spaces (no tabs)
- **Line length**: Maximum 100 characters
- **Braces**: Opening brace on same line (K&R style)
- **Naming conventions**:
  - Functions: `snake_case` (e.g., `init_starfield`)
  - Structs: `PascalCase` (e.g., `FrameBuffer`)
  - Constants: `UPPER_CASE` (e.g., `MAX_STARS`)
  - Variables: `snake_case` (e.g., `delta_time`)

### Example Code Style

```c
// Good function structure
void update_ship_physics(Ship *ship, float delta_time) {
    if (!ship || delta_time <= 0.0f) {
        return;
    }

    // Apply velocity with drag
    ship->x += ship->vx * delta_time;
    ship->y += ship->vy * delta_time;
    ship->z += ship->vz * delta_time;

    // Apply drag coefficient
    ship->vx *= (1.0f - DRAG_COEFFICIENT * delta_time);
}
```

### Security Best Practices

- **Always use safe string functions**: `snprintf` instead of `sprintf`, `fgets` instead of `gets`
- **Check buffer bounds**: Validate array indices before access
- **Initialize variables**: Always initialize pointers and arrays
- **Check return values**: Verify malloc, file operations, etc.
- **Avoid magic numbers**: Use named constants

### Memory Management

- Free all allocated memory
- Avoid memory leaks - use valgrind for testing
- Use stack allocation when possible for small, fixed-size arrays
- Document ownership of dynamically allocated memory

## Testing

### Writing Tests

Tests are located in the `tests/` directory. Each module should have corresponding tests:

```c
// tests/test_render.c
#include <assert.h>
#include "../include/render.h"

void test_frame_buffer_creation() {
    FrameBuffer *fb = create_frame_buffer(80, 24);
    assert(fb != NULL);
    assert(fb->width == 80);
    assert(fb->height == 24);
    destroy_frame_buffer(fb);
}

int main() {
    test_frame_buffer_creation();
    printf("All render tests passed!\n");
    return 0;
}
```

### Running Tests

```bash
make test                 # Run all tests
make test-joystick        # Test joystick detection
```

## Documentation

### Code Documentation

- Add comments for complex algorithms
- Document function parameters and return values
- Explain non-obvious design decisions
- Keep comments up to date with code changes

### User Documentation

When adding features, update:
- `README.md` - For user-facing changes
- `FEATURES.md` - For detailed feature documentation
- `MODE-GUIDE.md` - For gameplay mechanics
- `CLAUDE.md` - For development guidance

## Pull Request Process

### Before Submitting

- [ ] Code compiles without warnings (`make clean && make`)
- [ ] All tests pass (`make test`)
- [ ] Code follows style guidelines
- [ ] Documentation is updated
- [ ] No memory leaks (check with valgrind if possible)
- [ ] Commit messages are clear and descriptive

### Pull Request Description

Include:
1. **Summary**: What does this PR do?
2. **Motivation**: Why is this change needed?
3. **Testing**: How was this tested?
4. **Screenshots/Demo**: If UI changes, include visuals
5. **Breaking Changes**: Note any API or behavior changes

### Review Process

- Maintainers will review your PR
- Address feedback constructively
- Make requested changes in new commits
- Once approved, PR will be merged

## Types of Contributions

### Bug Fixes

- Create an issue describing the bug
- Reference the issue in your PR
- Include steps to reproduce
- Add tests to prevent regression

### New Features

- Discuss major features in an issue first
- Keep PRs focused on a single feature
- Update documentation
- Add tests for new functionality

### Performance Improvements

- Include benchmarks showing improvement
- Ensure functionality remains correct
- Document optimization approach

### Documentation

- Fix typos and improve clarity
- Add examples and usage guides
- Keep documentation in sync with code

## Architecture Overview

### Module Structure

- `main.c` - Game loop, input, HUD, mode management
- `ship.c` - 6-DOF physics calculations
- `weapons.c` - Missile and explosion systems
- `modes.c` - Game mode logic (training, skeet, etc.)
- `render.c` - Frame buffer and 3D rendering
- `starfield.c` - Star management
- `effects.c` - Starfield visual effects
- `gamepad.c` - SDL2 joystick support
- `input.c` - Keyboard input
- `terminal.c` - ncurses initialization

### Design Patterns

- **Object Pooling**: Pre-allocated arrays for missiles, explosions, targets
- **Delta-Time Physics**: Frame-independent movement
- **Frame Buffering**: Double-buffered rendering for flicker-free display
- **State Machines**: Game modes and AI behaviors

## Getting Help

- **Issues**: Open an issue for bugs or feature requests
- **Discussions**: Use GitHub Discussions for questions
- **Documentation**: Check CLAUDE.md for development guidelines

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Recognition

Contributors will be recognized in the project documentation. Thank you for helping make Terminal Stars better!
