# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Terminal Stars is a **fully implemented** 3D space combat and flight simulator running in the terminal using ncurses. It features 6-DOF physics, multiple game modes, weapons systems, joystick support, and various visual effects. The application has evolved from a simple starfield visualization into a comprehensive flight and combat simulator.

## Project Status

**FULLY IMPLEMENTED** - This is a mature, production-ready codebase with:
- ✅ Five complete game modes (Single Player, Dual Player, Co-op, Training, Skeet Shooting)
- ✅ Full 6-DOF flight physics with realistic momentum and drag
- ✅ Dual-missile weapons system with collision detection
- ✅ Advanced HUD with radar, enemy tracking, and tactical displays
- ✅ USB joystick/gamepad support (up to 2 controllers)
- ✅ Six starfield visual effects
- ✅ Horizon rendering for ground-based modes
- ✅ Frame-buffered 60fps rendering
- ✅ Comprehensive documentation (FEATURES.md, MODE-GUIDE.md)

## Current Architecture

### Implementation Details
- **Language**: C (C99 standard)
- **Rendering**: ncurses with double-buffered frame buffer
- **Performance**: Target 60 FPS with delta-time physics
- **Input**: Non-blocking keyboard + SDL2 gamepad support
- **Build System**: GNU Make with conditional SDL2 linking

### Core Systems

#### Flight Physics (ship.c)
- **6 Degrees of Freedom**: Full 3D movement and rotation
- **Inertial Physics**: Velocity-based movement with drag
- **Angular Momentum**: Smooth rotation with damping
- **Speed Limiting**: Automatic clamping to max velocity
- **Arcade-style**: Immediate response with realistic momentum

#### Weapons System (weapons.c)
- **Dual Missiles**: Fires two missiles per volley (left/right offset)
- **Guided Targeting**: Missiles track toward crosshair aim point
- **Object Pooling**: Pre-allocated arrays for missiles (100) and explosions (50)
- **Collision Detection**: Real-time checks against all targets
- **Visual Feedback**: Expanding explosion animations

#### Rendering Pipeline (render.c)
- **Frame Buffer**: Character and color buffers for each screen position
- **3D Projection**: Full camera transform pipeline (translate, rotate pitch/yaw/roll, project)
- **Multiple Render Modes**: Starfield, ships, weapons, horizon, targets
- **HUD Overlay**: Multi-layered display system (radar, indicators, stats)
- **Efficient Updates**: Only redraw changed regions when possible

#### Game Modes (modes.c)
- **Mode Selection Menu**: Arrow key navigation with instant selection
- **Training Mode**: Target spawning, collision detection, scoring
- **Skeet Mode**: Clay pigeon physics with gravity, progressive difficulty
- **Player Setup**: Auto-configuration based on selected mode
- **Statistics Tracking**: Score, accuracy, time, performance metrics

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

**Core Game Loop (main.c - 31KB)**:
- Application entry point and main render loop
- 60fps frame timing with delta-time calculation
- Mode menu handling and game state management
- Input processing (keyboard + joystick)
- HUD rendering (crosshair, radar, enemy indicators, stats)
- Integration point for all subsystems

**Flight Physics (ship.c - 6KB)**:
- Ship initialization and update logic
- 6-DOF physics calculations
- Pitch, yaw, roll control functions
- Camera update for both cockpit and chase views
- Forward vector calculation for thrust and weapons

**Weapons System (weapons.c - 6KB)**:
- Missile and explosion management
- Dual-missile firing with offset spawn positions
- Guided missile trajectories
- Collision detection against targets
- Explosion animation with expanding radius

**Game Modes (modes.c - 20KB)**:
- Mode selection menu rendering and input handling
- Player setup based on selected mode
- Training mode: target spawning, collision, scoring
- Skeet mode: clay pigeon physics, launchers, difficulty progression
- HUD rendering for each mode

**Rendering (render.c - 18KB)**:
- Frame buffer creation, clearing, and display
- 3D ship rendering with camera transforms
- Starfield rendering with six different effects
- Horizon rendering for skeet mode
- Target circle and clay pigeon rendering
- Text rendering for HUD elements

**Starfield Effects (starfield.c + effects.c - 11KB total)**:
- Star array management
- Six effect implementations:
  - EFFECT_LINEAR: Forward motion through stars
  - EFFECT_SPIRAL: Spiral rotation pattern
  - EFFECT_WARP: Hyperspeed with star stretching
  - EFFECT_TUNNEL: Cylindrical tunnel effect
  - EFFECT_EXPLODE: Outward expansion
  - EFFECT_WAVE: Undulating wave motion
- Torus roller coaster path (special effect)

**Input Systems (input.c + gamepad.c - 11KB total)**:
- Non-blocking keyboard input with getch()
- SDL2 joystick initialization and polling
- Dead zone handling for analog sticks
- Button mapping and state tracking
- Rumble/haptic feedback support

**Terminal Management (terminal.c - 2KB)**:
- ncurses initialization and cleanup
- Color pair setup (8 colors)
- Terminal resize detection and handling
- Screen size queries

### Key Design Patterns

**Object Pooling**:
- Pre-allocated arrays for missiles (MAX_MISSILES = 100)
- Pre-allocated arrays for explosions (MAX_EXPLOSIONS = 50)
- Pre-allocated arrays for targets (MAX_TRAINING_TARGETS = 20, MAX_CLAY_PIGEONS = 30)
- Active/inactive flags instead of dynamic allocation

**Delta-Time Physics**:
- Frame-independent movement and rotation
- Consistent behavior regardless of frame rate
- All physics calculations scaled by delta_time

**State Machines**:
- Game modes with clear state transitions
- Control modes (keyboard, joystick, AI, inactive)
- AI behavior types (orbital, hunting, evasive, formation)

**Separation of Concerns**:
- Physics calculations in ship.c
- Rendering in render.c
- Game logic in modes.c and main.c
- Input handling in input.c and gamepad.c

**Camera Transform Pipeline**:
1. Calculate position relative to camera
2. Apply yaw rotation (around Y-axis)
3. Apply pitch rotation (around X-axis)
4. Apply roll rotation (around Z-axis)
5. Project to 2D screen coordinates
6. Clip objects behind camera (Z <= 0)

## Implementation History

### Phase 1: Foundation (Completed)
- ✅ Terminal initialization with ncurses
- ✅ Frame buffer structure with character and color arrays
- ✅ Star rendering and perspective projection

### Phase 2: Starfield Effects (Completed)
- ✅ Six visual effects implemented
- ✅ Delta-time-based animation
- ✅ 60fps rendering achieved
- ✅ Effect switching and speed controls

### Phase 3: 3D Flight (Completed)
- ✅ 6-DOF ship physics
- ✅ Camera system (cockpit + chase views)
- ✅ Ship rendering in 3D space
- ✅ Full flight controls

### Phase 4: Combat Systems (Completed)
- ✅ Dual-missile weapons system
- ✅ Explosion visual effects
- ✅ Collision detection
- ✅ Advanced HUD (radar, enemy tracking)

### Phase 5: Multiplayer & Modes (Completed)
- ✅ Game mode selection menu
- ✅ Single player vs AI
- ✅ Dual player competitive
- ✅ Co-op mode
- ✅ Training simulator with targets
- ✅ USB joystick support (SDL2)

### Phase 6: Skeet Shooting (Completed - PR #5)
- ✅ Clay pigeon ballistic physics
- ✅ Horizon rendering
- ✅ Three launcher positions
- ✅ Progressive difficulty system
- ✅ Target circle reticle

## Enhancement Opportunities

### Immediate Opportunities
1. **Active Combat AI**: Implement AI_HUNTING behavior
   - Chase player with predictive tracking
   - Fire missiles at player
   - Evasive maneuvers when under fire

2. **Health & Damage**: Expand health system
   - Ship-to-ship collision damage
   - Missile hit detection on ships
   - Shield regeneration over time
   - Visual damage indicators

3. **Wave-Based Combat**: Add progressive enemy waves
   - Spawn multiple enemies
   - Increasing difficulty
   - Boss encounters
   - Score multipliers

### Long-term Enhancements
1. **Persistent Profiles**: Save/load game progress
2. **Custom Key Bindings**: User-configurable controls
3. **Network Multiplayer**: TCP/IP based multiplayer
4. **Mission System**: Story-driven objectives
5. **Sound Effects**: Terminal beep-based audio
6. **Replay System**: Record and playback sessions

## Related Projects

Reference `../boxes-live` for frame buffering implementation patterns.
