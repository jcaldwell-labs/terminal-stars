# Terminal Stars - Complete Mode Guide

## Table of Contents
1. [Getting Started](#getting-started)
2. [Mode Selection Menu](#mode-selection-menu)
3. [Single Player Mode](#single-player-mode-vs-ai)
4. [Dual Player Mode](#dual-player-mode-competitive)
5. [Co-op Mode](#co-op-mode-team-up)
6. [Training Simulator](#training-simulator)
7. [Skeet Shooting Mode](#skeet-shooting-mode)
8. [Controls Reference](#controls-reference)
9. [Tips and Strategies](#tips-and-strategies)

---

## Getting Started

### Launching the Game
```bash
./terminal-stars
```

Upon launch, you'll be greeted with the mode selection menu. Use arrow keys or W/S to navigate, and press Enter to select a mode.

### First Time Players
We recommend starting with **Training Simulator** (mode 4) to learn the controls and get comfortable with 6-DOF flight physics.

---

## Mode Selection Menu

### Navigation
- **Arrow Keys** or **W/S**: Move selection up/down
- **Enter**: Confirm selection and start game
- **Number Keys (1-5)**: Direct mode selection
- **ESC**: Quit game

### Available Modes
1. **Single Player (vs AI)** - Combat against AI opponent
2. **Dual Player (Competitive)** - Head-to-head space combat
3. **Co-op Mode (Team up)** - Cooperative flight
4. **Training Simulator** - Practice with targets
5. **Skeet Shooting** - Clay pigeon shooting range

### Returning to Menu
Press **M** at any time during gameplay to return to the mode selection menu.

---

## Single Player Mode (vs AI)

### Overview
Face off against an AI-controlled opponent in 3D space combat. The AI flies in an orbital pattern while you practice combat maneuvers, targeting, and weapons systems.

### Objectives
- Survive against AI opponent
- Practice combat flying
- Master weapons targeting
- Learn to use radar and tracking systems

### AI Behavior
- **Movement**: Circular orbit (50-unit radius)
- **Speed**: 0.5 radians/second orbital velocity
- **Pattern**: Sine wave bobbing in Z-axis for realism
- **Facing**: Always oriented in direction of motion
- **Current Status**: Orbital pattern (not yet attacking)

### HUD Elements

#### Top Left
```
SPEED: 85 m/s
HEALTH: ***
ENEMY: 124m
POS: (12, -5, 48)
MISSILES: 44
[READY]
```

#### Top Right
```
VIEW: COCKPIT
P2: AI
```

#### Bottom Right - Radar
```
+---RADAR---+
|     |     |
|  @--+--E  |
|     |     |
+----------+
```
- **@** = Your ship (always center)
- **E** = Enemy ship (color shows depth)
- **\*** = Incoming missiles

### Strategy Tips
1. **Use Radar**: Keep track of enemy position at all times
2. **Lead Your Shots**: Missiles take time to reach target
3. **Watch Your Six**: Enemy indicator shows if they're behind you
4. **Manage Ammo**: You start with 50 missiles (25 volleys)
5. **Use Both Views**: Toggle between Cockpit and Chase with 'V'

### Controls
- **WASD**: Pitch and Yaw
- **Q/E**: Roll left/right
- **Space**: Thrust
- **F**: Fire missiles
- **V**: Toggle camera view
- **M**: Return to menu

---

## Dual Player Mode (Competitive)

### Overview
Two-player competitive combat on the same terminal. Each player controls their own ship with independent controls (keyboard or gamepad).

### Player Setup
- **Player 1**: WASD + Q/E + Space + F (or Joystick 1)
- **Player 2**: Arrows + </> + Enter (or Joystick 2)
- Both players active simultaneously

### Objectives
- Outmaneuver your opponent
- Land successful missile hits
- Dodge incoming fire
- Survive longer than opponent

### Split Control Layout

#### Player 1 (Left Side)
```
W/S     - Pitch
A/D     - Yaw
Q/E     - Roll
Space   - Thrust
F       - Fire
V       - View toggle
```

#### Player 2 (Right Side)
```
UP/DOWN  - Pitch
LEFT/RIGHT - Yaw
< / >    - Roll
Enter    - Fire
```

### Joystick Support
If gamepads are detected:
- **Joystick 1** -> Player 1
- **Joystick 2** -> Player 2
- Automatic assignment on startup

### Tactical Considerations
1. **Spatial Awareness**: Use radar to track opponent
2. **Energy Management**: Don't waste missiles
3. **Positioning**: Try to get behind opponent
4. **Evasion**: Use all 6 degrees of freedom to dodge
5. **Communication**: Coordinate if playing cooperatively

### Winning Conditions
- Currently: Practice and skill development
- Future: Health system and elimination scoring

---

## Co-op Mode (Team Up)

### Overview
Two players work together exploring space and practicing formation flying. Perfect for learning multiplayer coordination.

### Player Setup
- Both Player 1 and Player 2 are active
- Independent control (keyboard/joystick mix supported)
- No AI opponent (peaceful exploration)

### Objectives
- **Formation Flying**: Stay in sync
- **Coordination**: Practice synchronized maneuvers
- **Exploration**: Navigate 3D space together
- **Skill Building**: Teach new players

### Cooperative Activities

#### Formation Flying Challenges
1. **Line Abreast**: Fly side-by-side at same speed
2. **Leader-Wingman**: One leads, other follows
3. **Diamond Formation**: Complex 3D positioning
4. **Synchronized Rolls**: Coordinate barrel rolls

#### Training Exercises
1. **Follow the Leader**: Mimic leader's movements
2. **Race Courses**: Navigate between waypoints
3. **Precision Flying**: Maintain exact distances
4. **Acrobatics**: Synchronized stunts

### Communication is Key
Since both players share one screen:
- Verbal coordination recommended
- Call out maneuvers before executing
- Use radar to maintain visual contact
- Practice spatial awareness together

---

## Training Simulator

### Overview
Practice environment with destructible targets. Perfect for learning flight controls, weapons systems, and improving accuracy.

### Features
- Up to 20 simultaneous targets
- Four different target patterns
- Score tracking and statistics
- No time limits
- Unlimited resets

### Target Patterns

#### Pattern 1: Static Grid (Key: 1)
```
Description: Grid of stationary targets
Difficulty: Easy
Best For: Learning to aim and fire
Layout: 3x3 grid in front of player
```
**Strategy**: Take your time, line up shots carefully

#### Pattern 2: Moving Line (Key: 2)
```
Description: Horizontal line of moving targets
Difficulty: Medium
Best For: Leading moving targets
Layout: 5 targets moving left to right
```
**Strategy**: Lead your shots, predict motion

#### Pattern 3: Orbiting (Key: 3)
```
Description: Targets in circular orbital paths
Difficulty: Hard
Best For: Advanced tracking
Layout: Multiple orbital radii and speeds
```
**Strategy**: Track circular motion, time your shots

#### Pattern 4: Random (Key: 4)
```
Description: Randomly positioned targets
Difficulty: Variable
Best For: Realistic combat simulation
Layout: Scattered in 3D space
```
**Strategy**: Prioritize closest targets, scan with radar

### Training HUD

Located in top-right corner:
```
=== TRAINING MODE ===

Score:      250
Targets Hit: 15
Shots Fired: 42
Accuracy:    35.7%
Time:        2:34

Pattern Keys: 1 2 3 4
Reset: R
```

### Performance Metrics
- **Score**: Points earned (10 per target)
- **Accuracy**: Hits / Shots fired x 100%
- **Efficiency**: How few shots to clear pattern
- **Time**: Session duration

### Training Controls
- **1**: Spawn Static Grid pattern
- **2**: Spawn Moving Line pattern
- **3**: Spawn Orbiting pattern
- **4**: Spawn Random pattern
- **R**: Reset session (clears score and targets)
- **F**: Fire missiles (tracked in shot counter)
- **M**: Return to menu

### Skill Progression

#### Beginner Goals
- 30% accuracy on Static Grid
- Complete one pattern without missing
- Learn all controls comfortably

#### Intermediate Goals
- 50% accuracy on Moving Line
- Hit 5 Orbiting targets in a row
- Achieve 100+ score in one session

#### Advanced Goals
- 60%+ accuracy overall
- Master Random pattern
- Clear all patterns with minimal shots

### Tips for Improvement
1. **Start Simple**: Begin with Static Grid
2. **Focus on Accuracy**: Don't spam missiles
3. **Practice Leading**: Moving targets require prediction
4. **Use Multiple Patterns**: Variety builds skills
5. **Reset Often**: Fresh starts help learning
6. **Watch Your Accuracy**: Aim for improvement over time

---

## Skeet Shooting Mode

### Overview
Clay pigeon shooting range with realistic ballistic physics. Targets launch from three positions and follow parabolic trajectories influenced by gravity.

### Visual Environment
Unlike other modes, Skeet mode features:
- **Horizon Line**: Ground/sky separation at 2/3 screen height
- **Ground Texture**: Dotted pattern below horizon
- **Clear Sky**: Clean targeting area above horizon
- **Target Reticle**: Circular aiming aid in center

### Launcher Positions

#### Bottom-Left Launcher
```
Position: (-200, -100, 300)
Direction: Arc right and upward
Velocity: 80-120 units/s (randomized)
```

#### Bottom-Right Launcher
```
Position: (200, -100, 300)
Direction: Arc left and upward
Velocity: 80-120 units/s (randomized)
```

#### Bottom-Center Launcher
```
Position: (0, -120, 200)
Direction: Straight up and forward
Velocity: 100-140 units/s (randomized)
```

### Clay Pigeon Physics
- **Gravity**: 50 units/s^2 downward acceleration
- **Flight Time**: 5 seconds before auto-miss
- **Hit Radius**: 30 units (generous for playability)
- **Character**: 'O' when active, 'X' when hit
- **Debris**: 8-particle explosion on hit
- **Color**: Magenta (active), White (debris)

### Difficulty Progression

Difficulty automatically increases every 30 seconds:

#### Easy (0-30s)
- Launch interval: 2.5 seconds
- Points per hit: 10
- Pigeons in air: 1-2

#### Medium (30-60s)
- Launch interval: 2.2 seconds
- Points per hit: 15
- Pigeons in air: 2-3

#### Hard (60-90s)
- Launch interval: 1.9 seconds
- Points per hit: 20
- Pigeons in air: 3-4

#### Expert (90s+)
- Launch interval: 1.6 seconds
- Points per hit: 25
- Pigeons in air: 4-5

### Skeet HUD

Located in top-right corner:
```
=== SKEET SHOOTING ===

Score:      350
Hit:        18
Missed:     5
Accuracy:   78.3%
Time:       1:42.5s
Difficulty: Hard
In Air:     3

Controls:
F: Fire
M: Menu
```

### Scoring System
- **Base Points**: 10 per pigeon
- **Difficulty Multiplier**: +5 per level
- **Expert Bonus**: 25 points per pigeon
- **Miss Penalty**: No points, counts against accuracy
- **Perfect Hits**: Bonus for hitting before peak of arc

### Aiming Techniques

#### Lead Shooting
```
Pigeon moving left to right:
   O ->        Aim here ->
     <-        Not here
   Current    Predicted
```

#### Elevation Adjustment
```
Rising pigeon: Aim slightly above
Falling pigeon: Aim directly at or below
Peak of arc: Easiest shot (momentarily stationary)
```

#### Target Circle Usage
The center reticle helps with:
- **Centering**: Confirm crosshair position
- **Distance**: Size reference for range estimation
- **Tracking**: Follow pigeon through reticle

### Strategy Guide

#### Beginner Strategy
1. **Wait for Launch**: Let pigeon reach peak of arc
2. **Center Shot**: Use target circle to line up
3. **Don't Rush**: Accuracy over speed
4. **Track Carefully**: Follow pigeon's path before shooting

#### Advanced Strategy
1. **Shoot Early**: Hit pigeons on the rise for more time
2. **Quick Acquisition**: Snap to target immediately after launch
3. **Predict Launches**: Anticipate launcher position
4. **Rapid Fire**: Multiple shots to ensure hits

### Achievement Goals

#### Bronze Tier
- 10 consecutive hits
- 50% accuracy
- 500 points total

#### Silver Tier
- 20 consecutive hits
- 70% accuracy
- 1000 points total
- Reach Hard difficulty

#### Gold Tier
- 30 consecutive hits
- 85% accuracy
- 2000 points total
- Survive to Expert difficulty

### Common Mistakes
1. **Shooting Too Late**: Pigeon already falling
2. **Poor Lead**: Not accounting for motion
3. **Spam Firing**: Wastes missiles, hurts accuracy
4. **Ignoring Difficulty**: Not adjusting for faster launches
5. **Wrong Launcher**: Expecting from different position

### Controls
- **WASD**: Aim (Pitch and Yaw)
- **Q/E**: Roll (less useful in skeet mode)
- **F**: Fire missiles
- **M**: Return to menu
- **1-4**: (Disabled in skeet mode)

---

## Controls Reference

### Universal Controls

#### Global (All Modes)
```
M          - Return to mode selection menu
ESC / Q    - Quit application (Q must be uppercase)
Tab        - Cycle starfield effects (except Skeet mode)
[ / ]      - Decrease/Increase starfield speed
```

### Keyboard Controls

#### Player 1
```
W          - Pitch down (nose down)
S          - Pitch up (nose up)
A          - Yaw left (turn left)
D          - Yaw right (turn right)
Q          - Roll left (bank left)
E          - Roll right (bank right)
Space      - Thrust
F          - Fire missiles
V          - Toggle camera view (Cockpit <-> Chase)
```

#### Player 2
```
Arrow Up   - Pitch down
Arrow Down - Pitch up
Arrow Left - Yaw left
Arrow Right- Yaw right
< (comma)  - Roll left
> (period) - Roll right
Enter      - Fire missiles
```

#### Mode-Specific Keys

**Training Mode**:
```
1          - Spawn Static Grid pattern
2          - Spawn Moving Line pattern
3          - Spawn Orbiting pattern
4          - Spawn Random pattern
R          - Reset training session
```

**Single Player**:
```
B          - Toggle AI mode for Player 2
```

### Gamepad Controls

#### Standard Gamepad Layout
```
Left Stick    - Pitch and Yaw
  Up/Down       = Pitch
  Left/Right    = Yaw

Right Stick   - Roll
  Left/Right    = Roll

Left Bumper (LB)  - Roll left (alternative)
Right Bumper (RB) - Roll right (alternative)

Right Trigger (RT) - Thrust
A Button           - Thrust (alternative)
X Button           - Fire missiles
Y Button           - Toggle camera view
Start Button       - Menu access
```

#### Gamepad Features
- **Dead Zone**: 15% to prevent drift
- **Analog Control**: Proportional to stick position
- **Rumble**: Vibration on weapon fire
- **Auto-Assign**: Joystick 1 -> Player 1, Joystick 2 -> Player 2

### Camera Controls
```
V (keyboard) or Y Button (gamepad)

Cockpit View:
  - First person perspective
  - Camera at ship position
  - Direct orientation control

Chase View:
  - Third person perspective
  - 15 units behind ship
  - 5 units above ship
  - Follows ship movement
  - Slight roll matching
```

---

## Tips and Strategies

### Flight Basics

#### Understanding 6-DOF
```
Pitch: Nose up/down (elevator)
Yaw:   Nose left/right (rudder)
Roll:  Banking left/right (ailerons)

Movement is RELATIVE to ship orientation!
```

#### Smooth Flying
1. **Small Inputs**: Don't over-correct
2. **Momentum**: You'll coast after releasing controls
3. **Drag**: Speed decays naturally, rotation dampens
4. **Thrust Management**: Pulse thrust, don't hold constantly
5. **Camera Views**: Switch views to see surroundings

#### Advanced Maneuvers

**Banking Turn**:
```
1. Roll into turn (Q or E)
2. Pitch up (S)
3. Roll back level (opposite direction)
```

**Barrel Roll**:
```
1. Add thrust for speed
2. Hold roll direction (Q or E)
3. Maintain slight pitch for circular motion
```

**Immelmann Turn**:
```
1. Pitch up into loop (S)
2. At top, roll 180 degrees (Q or E)
3. Level out going opposite direction
```

### Combat Tactics

#### Targeting
1. **Lead the Target**: Shoot where they'll be, not where they are
2. **Range Estimation**: Use enemy distance indicator
3. **Crosshair Alignment**: Center target in crosshair
4. **Firing Windows**: Wait for clear shot opportunities

#### Evasion
1. **Unpredictable Movement**: Mix pitch, yaw, and roll
2. **Radar Awareness**: Watch for incoming missiles
3. **Thrust Bursts**: Change speed randomly
4. **6-DOF Usage**: Use all axes, not just yaw

#### Energy Management
1. **Ammo Conservation**: You have 50 missiles (25 volleys)
2. **Reload Timing**: 0.5-second cooldown between shots
3. **Don't Spam**: Accuracy is better than volume

### Mode-Specific Tips

#### Single Player (vs AI)
- AI is currently passive (orbital pattern only)
- Practice tracking moving targets
- Use as missile accuracy training
- Perfect for learning radar system

#### Dual Player
- Communication is crucial (same screen)
- Use different altitudes to avoid collisions
- Radar helps track teammate/opponent
- Practice formation flying before combat

#### Training Mode
- Start with Static Grid to learn aiming
- Progress through patterns as skills improve
- Reset often to practice from clean slate
- Aim for improving accuracy percentage

#### Skeet Shooting
- Lead moving targets
- Shoot at peak of arc (easier)
- Watch launcher positions
- Adapt to difficulty increases

### Performance Optimization

#### If Experiencing Lag
1. **Reduce Terminal Size**: Smaller = faster rendering
2. **Close Other Apps**: Free up CPU
3. **Simpler Terminal**: Use xterm instead of heavy emulators
4. **Check System Load**: Ensure CPU not throttled

#### Best Performance Settings
- Terminal: 80x24 to 120x40 (sweet spot)
- Color depth: 8 colors sufficient
- Font: Monospace, medium size
- Background: Dark (less rendering)

### Troubleshooting

#### Controls Not Responding
- Check keyboard focus on terminal
- Try different terminal emulator
- Verify no key remapping software active

#### Joystick Not Detected
- Plug in before starting game
- Check SDL2 installation
- Test with: `jstest /dev/input/js0`

#### Visual Glitches
- Ensure terminal supports colors
- Check terminal size (minimum 80x24)
- Try different color schemes

---

## Progression Path

### Recommended Learning Order

1. **Tutorial Phase** (Training Mode)
   - Learn basic controls
   - Practice with Static Grid
   - Achieve 30% accuracy

2. **Skill Building** (Training Mode)
   - Try all four patterns
   - Improve to 50% accuracy
   - Master camera switching

3. **Combat Introduction** (Single Player)
   - Practice against orbital AI
   - Learn radar usage
   - Develop tracking skills

4. **Precision Practice** (Skeet Shooting)
   - Understand lead shooting
   - Adapt to difficulty progression
   - Achieve 70% accuracy

5. **Multiplayer** (Dual Player / Co-op)
   - Play with friend
   - Practice formation flying
   - Develop spatial awareness

### Mastery Checklist

- [ ] Complete all training patterns
- [ ] Achieve 60% accuracy in training
- [ ] Land 10 consecutive hits in skeet mode
- [ ] Successfully track AI opponent
- [ ] Execute barrel roll maneuver
- [ ] Maintain formation flight for 60 seconds
- [ ] Use both camera views effectively
- [ ] Master all 6 degrees of freedom
- [ ] Reach Expert difficulty in skeet mode
- [ ] Score 1000+ points in any mode

---

## Credits

**Terminal Stars** - Full-featured 3D space combat simulator for the terminal

**Mode Development**:
- Single Player, Dual Player, Co-op: Multiplayer combat framework
- Training Simulator: Target practice and skill development
- Skeet Shooting: Ballistic physics and horizon rendering (PR #5)

**Core Systems**:
- 6-DOF flight physics with realistic momentum
- Dual-missile weapons with guided targeting
- Advanced HUD with radar and enemy tracking
- SDL2 joystick support with haptic feedback
- Six starfield visual effects

Enjoy mastering all five modes of Terminal Stars!
