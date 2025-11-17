# Tests

This directory will contain unit tests for terminal-stars components.

## Test Structure

Tests will be organized by module:
- `test_starfield.c` - Starfield simulation tests
- `test_render.c` - Rendering and frame buffer tests
- `test_effects.c` - Effect algorithm tests
- `test_integration.c` - End-to-end integration tests

## Running Tests

```bash
make test              # Run all tests
make test_starfield    # Run specific test module (when implemented)
```
