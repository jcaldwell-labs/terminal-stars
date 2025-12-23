# WSL Gamepad Fix

## Problem
The program was hanging on startup when running in WSL2 (Windows Subsystem for Linux). The hang occurred during SDL2 joystick/haptic subsystem initialization.

## Root Cause
SDL2's joystick subsystem tries to access `/dev/input` devices in WSL2, which can cause the `SDL_InitSubSystem()` call to block indefinitely. This is a known issue with SDL in WSL environments where hardware device access is limited.

## Solution
The program now:
1. **Detects WSL automatically** by checking `/proc/version` for "microsoft" or "WSL"
2. **Skips gamepad initialization in WSL** to avoid the hang
3. **Continues running with keyboard-only controls**

## Code Changes
- `src/gamepad.c`: Added `is_wsl()` function to detect WSL environment
- `src/gamepad.c`: Skip SDL initialization when running in WSL
- `src/gamepad.c`: Removed `SDL_INIT_HAPTIC` to reduce potential hanging issues
- Environment variable support: Set `TERMINAL_STARS_NO_GAMEPAD=1` to manually disable gamepad

## Running the Game

### In WSL (keyboard only)
```bash
./terminal-stars
```

### With Gamepad Support (native Linux)
If you're on native Linux with a physical joystick:
```bash
./terminal-stars
```

### Force Disable Gamepad (any environment)
```bash
export TERMINAL_STARS_NO_GAMEPAD=1
./terminal-stars
```

## Controls
- **Player 1**: WASD (pitch/yaw), Q/E (roll), Space (thrust), F (fire)
- **Player 2**: Arrow keys (pitch/yaw), </> (roll), Enter (thrust)
- **View Toggle**: V
- **Quit**: Shift+Q or ESC

## Testing
The program was tested successfully in WSL2 and now starts immediately without hanging.
