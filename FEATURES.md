# Terminal Stars - Complete Feature Documentation

## Overview

Terminal Stars is a full-featured 3D space combat and flight simulator running entirely in the terminal using ncurses. The application provides five distinct game modes, realistic 6-DOF physics, dual-missile weapons systems, joystick support, and various visual effects including starfield motion and horizon rendering.

## Game Modes

The game features a mode selection menu at startup with five different play modes:

### 1. Single Player (vs AI)
- **Description**: Fight against an AI-controlled opponent
- **Player Setup**: Player 1 (human) vs Player 2 (AI)
- **AI Behavior**: Orbital pattern - AI flies in circular orbit with bobbing motion
- **Weapons**: Full combat with missiles and explosions
- **HUD**: Radar, enemy tracking, distance indicators
- **Best For**: Solo practice and combat training

### 2. Dual Player (Competitive)
- **Description**: Two players compete against each other
- **Player Setup**: Player 1 vs Player 2 (both human-controlled)
- **Controls**: Each player can independently use keyboard or gamepad
- **Weapons**: Full missile combat between players
- **Best For**: Head-to-head space combat

### 3. Co-op Mode (Team Up)
- **Description**: Two players work together
- **Player Setup**: Both players active and human-controlled
- **Objective**: Team up to explore and navigate together
- **Controls**: Independent keyboard/gamepad for each player
- **Best For**: Cooperative gameplay and formation flying

### 4. Training Simulator
- **Description**: Practice flight and weapons in a controlled environment
- **Features**:
  - Up to 20 simultaneous targets
  - Four different target patterns (Static Grid, Moving Line, Orbiting, Random)
  - Score tracking with accuracy percentage
  - Shot counter and performance metrics
  - Pattern spawning with keys 1-4
  - Session reset with 'R' key
- **Statistics**: Score, targets hit, shots fired, accuracy, session time
- **Best For**: Learning controls and improving combat skills

### 5. Skeet Shooting Mode
- **Description**: Clay pigeon shooting range with realistic physics
- **Features**:
  - Ballistic clay pigeon trajectories with gravity
  - Three launcher positions (left, right, center)
  - Progressive difficulty (Easy → Medium → Hard → Expert)
  - Horizon rendering with ground effects
  - Target circle reticle for aiming
  - Automatic difficulty scaling every 30 seconds
- **Physics**: Realistic gravity (50 units/s²), arc trajectories, 5-second flight time
- **Scoring**: Base 10 points + difficulty multiplier (up to 25 points on Expert)
- **Statistics**: Score, hits, misses, accuracy, time, difficulty level
- **Best For**: Target practice and precision shooting

## Joystick/Gamepad Support

### Hardware Support
- Supports up to **2 USB joysticks/gamepads** simultaneously
- Compatible with standard gamepad layouts (Xbox, PlayStation, generic USB controllers)
- Works with 8-direction analog sticks and digital buttons
- **Optional**: Requires SDL2 library for full joystick support

### Joystick Controls

#### Standard Gamepad Layout:
- **Left Analog Stick**: Pitch (up/down) and Yaw (left/right)
- **Right Analog Stick**: Roll (banking left/right)
- **Left Bumper (LB)**: Roll left
- **Right Bumper (RB)**: Roll right
- **Right Trigger (RT)**: Thrust
- **A Button**: Thrust (alternative)
- **X Button**: Fire weapon
- **Y Button**: Toggle camera view
- **Start Button**: Menu access

#### Features:
- **Dead Zone**: 15% dead zone to prevent stick drift
- **Analog Control**: Smooth, proportional control based on stick position
- **Haptic Feedback**: Rumble on weapon fire (if supported by controller)
- **Auto-Detection**: Automatically assigns joysticks to players

### Keyboard Controls (Unchanged)

#### Player 1:
- **W/S**: Pitch up/down
- **A/D**: Yaw left/right
- **Q/E**: Roll left/right
- **Space**: Thrust
- **F**: Fire weapon
- **V**: Toggle camera view

#### Player 2:
- **Arrow Keys**: Pitch and Yaw
- **< / >**: Roll left/right
- **Enter**: Thrust

#### Global Controls:
- **ESC or Shift+Q**: Quit
- **M**: Return to menu
- **Tab**: Cycle starfield effects
- **[ / ]**: Adjust starfield speed
- **B**: Toggle AI mode (legacy)

## Training Mode

### Features
Training mode provides a safe environment to practice flight and combat skills.

#### Target Types:
1. **Static Grid** (Pattern 1): Grid of stationary targets
2. **Moving Line** (Pattern 2): Horizontal line of moving targets
3. **Orbiting** (Pattern 3): Targets in orbital paths
4. **Random** (Pattern 4): Random positions and movements

#### Statistics Tracked:
- **Score**: Points earned from hitting targets
- **Targets Hit**: Number of targets destroyed
- **Shots Fired**: Total weapons fired
- **Accuracy**: Hit percentage
- **Session Time**: Time elapsed in training

#### Training Controls:
- **1-4**: Spawn different target patterns
- **R**: Reset training session
- **F**: Fire weapon (tracks shots)

### Training HUD
The training mode displays a comprehensive HUD showing:
- Current score
- Targets destroyed
- Shots fired
- Accuracy percentage (color-coded: green >50%, red <50%)
- Session timer
- Remaining targets
- Pattern selection keys

## Installation

### Basic Installation (Keyboard Only)
```bash
sudo apt-get install libncurses-dev
make
./terminal-stars
```

### Full Installation (With Joystick Support)
```bash
sudo apt-get install libncurses-dev libsdl2-dev
make
./terminal-stars
```

**Note**: SDL2 is optional. If not installed, the game will compile without joystick support but all other features will work normally.

## Technical Details

## Ship Physics System (6 Degrees of Freedom)

### Movement Capabilities
- **Position**: Full 3D movement in X, Y, Z coordinates
- **Orientation**: Independent Pitch (up/down), Yaw (left/right), Roll (banking)
- **Linear Velocity**: Momentum-based movement with drag
- **Angular Velocity**: Smooth rotation with damping
- **Maximum Speed**: 100 units/second (configurable)
- **Acceleration**: Responsive thrust control
- **Drag Coefficient**: Realistic speed decay when not thrusting

### Flight Characteristics
- **Arcade-style Controls**: Immediate response with momentum
- **Pitch Limiting**: Prevents complete flips (max ±90 degrees)
- **Smooth Damping**: Angular velocity decays naturally for stable flight
- **Speed Clamping**: Automatic speed limiting to max velocity

## Weapons System

### Dual Missile Launcher
- **Fire Pattern**: Launches two missiles simultaneously (left/right offset)
- **Guided Missiles**: Each missile tracks toward crosshair aim point
- **Speed**: 150 units/second
- **Lifetime**: 3 seconds before auto-detonation
- **Starting Ammo**: 50 missiles per player
- **Fire Rate**: 0.5-second cooldown between volleys
- **Ammo Consumption**: 2 missiles per trigger pull

### Collision Detection
- **Training Targets**: Hit radius of 10 units
- **Clay Pigeons**: Hit radius of 30 units (easier to hit)
- **Real-time Checking**: All active missiles checked against all targets every frame

### Explosions
- **Visual Effect**: Expanding radius animation
- **Duration**: 0.3 seconds
- **Maximum Radius**: 10 units
- **Trigger**: Missile timeout or target hit
- **Color**: Yellow burst with particle effects

## Advanced HUD Systems

### Primary Display (Top Left)
- **Speed Indicator**: Current velocity in m/s
- **Health Display**: Visual hearts (max 3)
- **Enemy Distance**: Real-time range in meters
- **Position Coordinates**: Debug X, Y, Z display
- **Missile Count**: Remaining ammunition
- **Fire Status**: "READY" or "RELOADING" indicator with color coding

### Camera Info (Top Right)
- **View Mode**: COCKPIT or CHASE camera
- **Player 2 Mode**: AI or MANUAL control indicator

### Targeting System
- **Center Crosshair**: Multi-line reticle
- **Target Label**: "TARGET" text above crosshair
- **Directional Indicators**: Horizontal and vertical alignment aids

### Unit Circle Display
- **Function**: Shows ship orientation in 3D space
- **Visual**: Circular representation of pitch, yaw, roll
- **Purpose**: Spatial awareness and orientation tracking

### Enemy Direction Indicator
- **Off-Screen Tracking**: Shows 'X' marker when enemy is out of view
- **Directional Aid**: Points toward enemy location
- **Behind Warning**: "BEHIND" label when enemy is in rear hemisphere
- **Always Active**: Continuous tracking for situational awareness

### Radar Overlay (Bottom Right)
- **Display**: 9x9 grid tactical radar
- **Player Icon**: '@' symbol at center
- **Enemy Icon**: 'E' with color-coded depth:
  - **Cyan**: Same Z-plane (level with player)
  - **Red**: Far ahead in Z-axis
  - **Yellow**: Behind in Z-axis
- **Missile Warning**: Incoming enemy missiles shown as '*'
- **Scale**: Adjustable radar range (default 0.08 scale factor)
- **Border**: Labeled "RADAR" with frame

## Camera System

### Cockpit View (First Person)
- **Position**: Camera at ship location
- **Orientation**: Directly controlled by ship rotation
- **Immersion**: Full first-person flight experience
- **Default**: Starting camera mode
- **Toggle**: Press 'V' (keyboard) or Y button (gamepad)

### Chase Camera (Third Person)
- **Position**: 15 units behind ship
- **Elevation**: 5 units above ship
- **Tracking**: Automatically follows ship movement
- **Roll Matching**: Camera tilts 50% of ship roll for realism
- **View Direction**: Always looks at ship
- **Toggle**: Press 'V' (keyboard) or Y button (gamepad)

## Starfield Visual Effects

### Six Effect Modes
1. **Linear**: Classic forward motion through stars (default for flight modes)
2. **Spiral**: Stars rotate in mesmerizing spiral pattern
3. **Warp**: Hyperspeed effect with star stretching
4. **Tunnel**: Cylindrical tunnel motion effect
5. **Explode**: Stars expand outward from center
6. **Wave**: Undulating wave motion through starfield

### Effect Controls
- **Toggle**: Tab key to cycle through effects
- **Speed Up**: ] key (bracket right)
- **Speed Down**: [ key (bracket left)
- **Speed Range**: 0.1× to 5.0× normal speed
- **Note**: Effects disabled in Skeet mode (horizon rendering instead)

## Architecture Changes

### Core Modules
- **`src/main.c`** (31KB): Game loop, mode handling, input processing, HUD rendering
- **`src/ship.c`**: 6-DOF physics engine, ship controls, camera updates
- **`src/weapons.c`**: Missile system, explosions, collision detection
- **`src/modes.c`** (20KB): Mode selection menu, training logic, skeet mechanics
- **`src/render.c`** (18KB): Frame buffer, 3D projection, horizon rendering
- **`src/starfield.c`**: Star management, visual effects
- **`src/effects.c`**: Six starfield effect implementations
- **`src/gamepad.c`** (9KB): SDL2-based joystick support with fallback
- **`src/input.c`**: Keyboard input handling
- **`src/terminal.c`**: ncurses initialization and resize handling

### Data Structures
- **`Ship3D`**: Complete ship state (position, velocity, orientation, control mode)
- **`Missile`**: Guided projectile with 3D velocity and target point
- **`Explosion`**: Expanding visual effect with radius animation
- **`TrainingTarget`**: Practice targets with health and point values
- **`ClayPigeon`**: Ballistic targets with gravity physics
- **`WeaponsSystem`**: Manages missile pool and explosion pool
- **`GameState`**: Current mode, menu state, session pointers
- **`Camera`**: View position and orientation (3D transform)
- **`Starfield`**: Star array and effect parameters

### Control System
The new control system supports:
- **Multiple input devices**: Each player can independently use keyboard or gamepad
- **Hot-swapping**: Input device assignment at startup based on availability
- **Graceful fallback**: Works without SDL2 installed
- **AI control**: Players can be set to AI control with different behavior types

### AI Behaviors (Extensible)
- `AI_ORBITAL`: Circular orbital pattern (legacy)
- `AI_HUNTING`: Chase and attack player
- `AI_EVASIVE`: Avoid player attacks
- `AI_FORMATION`: Fly in formation
- `AI_TRAINING_DUMMY`: Static target for training

## Performance and Technical Features

### Frame Rate
- **Target**: 60 FPS (16.67ms per frame)
- **Timing**: Delta-time based physics for frame-independent updates
- **Frame Limiting**: usleep-based timing control
- **Consistency**: Smooth motion regardless of system speed

### Optimization Techniques
- **Object Pooling**: Pre-allocated arrays for missiles and explosions
- **Inactive Skipping**: Only process active game objects
- **Efficient Collision**: Spatial checks only for active projectiles
- **Minimal Allocation**: No dynamic memory during gameplay
- **Diff Rendering**: Only redraw changed portions when possible

### Terminal Support
- **Requirements**: ncurses, 80x24 minimum size, 8+ colors
- **Tested Terminals**: xterm, gnome-terminal, konsole, iTerm2, Windows Terminal (WSL2)
- **Resize Handling**: Automatic detection and frame buffer recreation
- **Color Depth**: 8 ncurses color pairs with optimal allocation

## Recent Additions (PR #5)

### Skeet Shooting Mode
Complete new game mode with:
- **Horizon Rendering**: Ground/sky separation at 2/3 screen height
- **Ground Texture**: Dotted pattern simulating terrain
- **Clay Pigeon Physics**: Ballistic trajectories with 50 units/s² gravity
- **Three Launchers**: Bottom-left, bottom-right, bottom-center
- **Progressive Difficulty**: Automatic scaling every 30 seconds
- **Target Circle Reticle**: Enhanced aiming visualization
- **Debris Effects**: Visual feedback on successful hits

### Integration Status
- **Merged**: Successfully integrated into main branch
- **Build Status**: Compiles cleanly with no warnings
- **Testing**: Verified on multiple terminal types
- **Backward Compatible**: All existing modes work unchanged

## Future Enhancement Opportunities

### Potential Combat Mode Additions
1. **Active Combat AI**: Enemy ships that hunt and attack
   - Implement AI_HUNTING behavior (currently orbital only)
   - Add evasive maneuvers and counter-attacks
   - Smart targeting and missile avoidance

2. **Damage System**: Enhanced combat mechanics
   - Shield regeneration over time
   - Different damage types (missile, collision, etc.)
   - Visual damage indicators on ship

3. **Wave-Based Combat**: Progressive enemy difficulty
   - Spawn increasing numbers of enemies
   - Boss battles at interval milestones
   - Power-up drops and collectibles

4. **Advanced Weapons**: Additional weapon types
   - Rapid-fire lasers
   - Seeking missiles with better tracking
   - Area-effect bombs
   - Weapon power-ups

### Other Potential Additions
1. **Persistent Profiles**: Save player statistics and high scores
2. **Custom Key Mapping**: User-configurable controls
3. **Network Multiplayer**: Play over LAN/Internet
4. **More Game Modes**: Deathmatch, Capture the Flag, Race modes
5. **Mission System**: Story-driven objectives and campaigns
6. **Achievements**: Track milestones and accomplishments
7. **Replay System**: Record and playback flight sessions
8. **Sound Effects**: Terminal beep-based audio feedback

## Troubleshooting

### No Joysticks Detected
- Ensure your controller is plugged in before starting the game
- Check that SDL2 is properly installed: `sdl2-config --version`
- Verify controller works: `jstest /dev/input/js0` (Linux)

### Joystick Drift
- Default dead zone is 15%, adjustable in `gamepad.c`
- Recalibrate controller using system tools

### Build Errors
- Missing SDL2: Install with `sudo apt-get install libsdl2-dev`
- Missing ncurses: Install with `sudo apt-get install libncurses-dev`
- Make sure you have a C compiler installed

## Credits

**Game Modes and Training System**: Added in response to multiplayer and practice mode request
**Joystick Support**: SDL2-based implementation with graceful degradation
**Architecture**: Modular design for easy extension

---

Enjoy the enhanced Terminal Stars experience!
