# New Features: Player Modes and Joystick Support

## Overview

This update adds comprehensive multiplayer support, USB joystick/gamepad controls, and a training simulator mode to Terminal Stars.

## Game Modes

The game now features a mode selection menu at startup with four different play modes:

### 1. Single Player (vs AI)
- **Description**: Fight against an AI-controlled opponent
- **Player Setup**: Player 1 (human) vs Player 2 (AI)
- **AI Behavior**: Hunting mode - AI actively pursues and attacks the player
- **Best For**: Solo practice and combat training

### 2. Dual Player (Competitive)
- **Description**: Two players compete against each other
- **Player Setup**: Player 1 vs Player 2 (both human-controlled)
- **Controls**: Each player can use keyboard or gamepad
- **Best For**: Head-to-head combat

### 3. Co-op Mode (Team Up)
- **Description**: Two players work together
- **Player Setup**: Both players active and human-controlled
- **Objective**: Team up against future AI enemies (expandable)
- **Best For**: Cooperative gameplay

### 4. Training Simulator
- **Description**: Practice flight and weapons in a controlled environment
- **Features**:
  - Static and moving targets
  - Score tracking
  - Accuracy statistics
  - Multiple target patterns
- **Best For**: Learning controls and improving skills

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

### Architecture Changes

#### New Files:
- **`include/gamepad.h`**: Joystick input API
- **`src/gamepad.c`**: SDL2-based joystick implementation with fallback
- **`include/modes.h`**: Game mode management API
- **`src/modes.c`**: Mode initialization, training logic, menu rendering

#### Modified Files:
- **`include/types.h`**: Added GameMode, ControlMode, AIBehaviorType enums; GameState, TrainingSession, TrainingTarget structs
- **`src/main.c`**: Integrated menu system, mode selection, joystick input processing
- **`src/ship.c`**: Added control_mode, ai_behavior, joystick_id fields
- **`include/render.h` / `src/render.c`**: Added render_text() for HUD rendering
- **`Makefile`**: Conditional SDL2 linking

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

## Future Enhancements

### Potential Additions:
1. **Advanced AI**: More sophisticated AI behaviors
2. **More Training Scenarios**: Obstacle courses, time trials
3. **Persistent Profiles**: Save player statistics and preferences
4. **Custom Key Mapping**: User-configurable controls
5. **Network Multiplayer**: Play over LAN/Internet
6. **More Game Modes**: Deathmatch, Capture the Flag, etc.
7. **Difficulty Levels**: Adjustable AI difficulty
8. **Achievements**: Track milestones and accomplishments

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
