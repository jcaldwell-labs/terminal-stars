# Terminal Stars: Unified Roadmap

**Status**: Three branches, three visions → Need one clear path forward
**Date**: 2025-11-17
**Decision Required**: Choose your adventure 🎯

---

## Current State Analysis

### Branch Overview

| Branch | Commit | Content | Status |
|--------|--------|---------|--------|
| **PR #1 Implementation** | `66d8f9f` | 787 lines of working C code<br>6-DOF camera, 7 effects, rendering | ✅ Code works<br>⚠️ Architecture issues |
| **Review/Decomposition** | `a74f07a` | Analysis of PR #1<br>7-phase visualizer roadmap | 📋 Planning doc |
| **Game Handoff** | `6fe8b13` | Complete game design docs<br>Dogfight game spec | 📋 Design doc |

### What Each Branch Proposes

**PR #1 Branch** says:
> "I built it! 6-DOF camera, torus roller coaster, 7 effects, 60 FPS rendering!"

**Review Branch** says:
> "Hold on, let's rebuild this properly in 7 incremental phases with clean architecture."

**Game Branch** says:
> "Forget the visualizer, let's make a 2-player dogfight game instead! 12-18 hours to MVP!"

---

## The Decision Matrix

### Option A: Build the Visualizer (7-Phase Plan)

**What You Get:**
- Beautiful starfield effects (LINEAR, SPIRAL, WARP, TUNNEL, EXPLODE, WAVE)
- Camera system to fly through space
- Track/path system (torus roller coaster, custom paths)
- Path recording and playback

**Why Choose This:**
- ✅ Original vision from README
- ✅ Educational (learn 3D rendering, camera systems)
- ✅ Portfolio piece (visual demo)
- ✅ Foundation for future projects

**Time Investment:**
- Phase 1-2 (Foundation + Basic Motion): **4-6 hours**
- Phase 3 (Camera System): **4-6 hours**
- Phase 4 (Additional Effects): **6-8 hours**
- Phase 5-6 (Tracks/Paths): **8-10 hours**
- **Total: 22-30 hours**

**Risk:**
- Medium complexity (camera math, 3D transformations)
- Need good testing strategy
- PR #1 code needs refactoring (architectural issues)

---

### Option B: Build the Dogfight Game

**What You Get:**
- 2-player terminal dogfight game
- Ships with arcade physics
- Shooting, collision detection, scoring
- Complete game loop (menu → play → game over)

**Why Choose This:**
- ✅ More engaging end result (interactive game!)
- ✅ Clearer goal (fun gameplay)
- ✅ Reuses same rendering tech
- ✅ Ships to friends/social media

**Time Investment:**
- Phase 1 (Two ships, basic controls): **4 hours**
- Phase 2 (Shooting, collisions, health): **6 hours**
- Phase 3 (Game loop, score, polish): **4 hours**
- Phase 4 (AI, power-ups, effects): **2-4 hours**
- **Total: 12-18 hours**

**Risk:**
- Low complexity (simpler than visualizer)
- Game balance requires playtesting
- Need two players for full testing

---

### Option C: Hybrid Approach (Best of Both Worlds)

**What You Get:**
- Start with dogfight game (faster, more fun)
- Extract rendering engine as library
- Later: Add visualizer as "demo mode" or "replay viewer"

**Why Choose This:**
- ✅ Ship something playable quickly
- ✅ Validate rendering engine works
- ✅ Can still build visualizer later
- ✅ Code reuse between projects

**Time Investment:**
- Week 1: Dogfight game MVP (**12-18 hours**)
- Week 2: Extract rendering library (**4-6 hours**)
- Week 3+: Add visualizer features incrementally

**Risk:**
- Lowest risk (iterative approach)
- Might lose motivation after game is done
- Code extraction can be tedious

---

## Recommendation: Option C (Hybrid)

### Why This Is The Best Path

**Short-term win:**
- Build dogfight game first (12-18 hours)
- Playable, fun, shareable
- Validates all the hard parts (rendering, input, physics)

**Long-term flexibility:**
- Rendering engine is proven
- Can add visualizer later as "attract mode"
- Can add replay system that uses visualizer code
- Can extract as library for other projects

**Risk mitigation:**
- Fastest path to working software
- Tests core technology immediately
- Easier to stay motivated (playable game > visual demo)

---

## Unified Implementation Roadmap

### Phase 0: Code Consolidation (1-2 hours)

**Goal**: Understand what's actually working and create clean baseline

**Tasks:**
- [ ] Check out PR #1 branch (`claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM`)
- [ ] Build and test (`make clean && make && make run`)
- [ ] Document what works / what's broken
- [ ] Create fresh branch: `develop` or `feature/dogfight-mvp`
- [ ] Copy working rendering code from PR #1
- [ ] Strip out effects (keep only rendering pipeline)

**Acceptance:**
- Can display 200 static stars at 60 FPS
- No crashes, clean terminal cleanup
- Baseline code committed

---

### Phase 1: Dogfight MVP - Two Ships (4 hours)

**Goal**: Two players can fly ships on screen

**Data Structures** (add to `types.h`):
```c
typedef struct {
    double x, y;           // Position (2D for now)
    double heading;        // Angle in radians
    double velocity_x;     // Velocity components
    double velocity_y;
    int health;            // Hit points (3 to start)
    bool active;           // Alive/dead
    char sprite;           // Display character ('>' or '<')
} Ship;

typedef struct {
    Ship player1;
    Ship player2;
    int score_p1;
    int score_p2;
} GameState;
```

**Features:**
- [ ] Ship struct and initialization
- [ ] Render two ships as `>` and `<` characters
- [ ] Player 1 controls: WASD (W=thrust, A/D=turn)
- [ ] Player 2 controls: Arrow keys
- [ ] Arcade physics (instant turn, momentum)
- [ ] Arena boundaries (80x24, wrap or bounce)
- [ ] HUD showing P1/P2 labels

**Testing:**
- Both ships visible and controllable
- Ships don't overlap on spawn
- Controls feel responsive
- No crashes at boundaries

**Acceptance:**
- Two players can fly around screen
- Smooth 60 FPS rendering
- Clean keyboard input

---

### Phase 2: Combat Mechanics (6 hours)

**Goal**: Players can shoot and destroy each other

**Data Structures**:
```c
#define MAX_BULLETS 20

typedef struct {
    double x, y;
    double velocity_x;
    double velocity_y;
    int owner;             // 0=P1, 1=P2
    bool active;
    double lifetime;       // Seconds until despawn
} Bullet;

typedef struct {
    Bullet bullets[MAX_BULLETS];
    int count;             // Active bullets
} BulletPool;
```

**Features:**
- [ ] Bullet pool (20 max)
- [ ] Fire bullet on Space (P1) / Enter (P2)
- [ ] Bullets inherit ship velocity
- [ ] Bullet-ship collision detection (distance check)
- [ ] Damage system (3 hits = destroyed)
- [ ] Respawn after 3 seconds
- [ ] Hit feedback (flash, beep)
- [ ] Score tracking (kills)

**Testing:**
- Bullets fire from ship position
- Collision detection works
- Ships respawn correctly
- Score increments on kill

**Acceptance:**
- Complete combat loop works
- Can play a full match
- Winner is clear

---

### Phase 3: Game Loop & Polish (4 hours)

**Goal**: Full game experience from start to finish

**Features:**
- [ ] Game state machine (MENU → PLAYING → GAME_OVER)
- [ ] Start menu (press Space to start)
- [ ] First to 5 kills wins
- [ ] Game over screen with winner
- [ ] Restart match (R key)
- [ ] Quit (Q key)
- [ ] Full HUD (health bars, score, time)
- [ ] Visual feedback (explosions as expanding `*`)

**Testing:**
- Full game loop works
- All transitions smooth
- No way to softlock
- Can play multiple matches

**Acceptance:**
- **SHIPPABLE GAME**
- Can play 5-minute dogfight matches
- Fun for 2 players

---

### Phase 4: Game Enhancements (Optional, 2-4 hours)

**Goal**: Make it more fun and replayable

**Pick 2-3:**
- [ ] AI opponent (simple chase + shoot logic)
- [ ] Power-ups (health, speed boost, rapid fire)
- [ ] Multiple weapon types (spread shot, laser)
- [ ] Obstacles (asteroids, debris)
- [ ] Larger arena (scrolling viewport)
- [ ] Sound effects (via terminal beep)
- [ ] Replay system (save match data)

---

### Phase 5: Extract Rendering Library (4-6 hours)

**Goal**: Separate reusable rendering code from game logic

**Tasks:**
- [ ] Create `libterm3d` library
- [ ] Move rendering code to library
- [ ] Define clean API (`term3d_init`, `term3d_render`, etc.)
- [ ] Update dogfight to use library
- [ ] Document library API

**Acceptance:**
- Game still works
- Rendering code is reusable
- Other projects can link against library

---

### Phase 6+: Add Visualizer Features (Future)

**Goal**: Build original starfield visualizer using proven engine

Now you can add:
- Starfield effects (LINEAR, SPIRAL, WARP, etc.)
- Camera path system
- Torus roller coaster
- Path recording/playback
- Use as "demo mode" when game is idle
- Use as replay viewer (show match from camera paths)

---

## Immediate Next Steps

### Decision Time ⏱️

**Choose your path:**

**A)** "Let's build the visualizer properly" → Start Phase 0, extract PR #1 rendering, follow 7-phase visualizer plan

**B)** "Let's make the dogfight game" → Start Phase 0, extract PR #1 rendering, follow dogfight plan

**C)** "Let's do both (hybrid)" → Start Phase 0, build game first, add visualizer later

### Recommended Action (Next 30 Minutes)

1. **Test PR #1 code**
   ```bash
   git checkout claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM
   make clean && make
   ./build/terminal-stars
   ```
   - Does it run?
   - What works?
   - What's broken?

2. **Document findings**
   - Screenshot of working demo
   - List of issues/bugs
   - Note what code is reusable

3. **Make decision**
   - Visualizer, Game, or Hybrid?
   - Create branch for chosen path
   - Start Phase 0 (code consolidation)

---

## Technical Debt & Architecture

### PR #1 Code Issues to Fix

**Camera vs Effects Confusion:**
- TORUS effect modifies both camera AND stars
- No clear world-space vs view-space
- **Fix**: Separate camera movement from star positions

**State Management:**
- Camera embedded in Starfield struct
- **Fix**: Extract to separate Camera module

**Testing Gaps:**
- No unit tests
- Can't test components independently
- **Fix**: Add modular testing from Phase 0

### Clean Architecture Principles

**For Dogfight Game:**
```
game.c       → Game loop, state machine
entities.c   → Ships, bullets (pure data)
physics.c    → Movement, collision
render.c     → 3D projection, drawing
input.c      → Keyboard mapping
terminal.c   → ncurses wrapper
```

**For Visualizer:**
```
main.c       → Render loop
starfield.c  → Star lifecycle
effects.c    → Motion effects
camera.c     → View transforms
paths.c      → Camera paths
render.c     → 3D projection
terminal.c   → ncurses wrapper
```

**Shared (Library):**
```
libterm3d/
  render.c     → 3D-to-2D projection
  framebuf.c   → Double buffering
  terminal.c   → ncurses init/cleanup
  math3d.c     → Vector/matrix math
```

---

## Success Metrics

### Visualizer Success
- [ ] 60 FPS with 200 stars
- [ ] 6 effects working smoothly
- [ ] Camera path system functional
- [ ] Can record and replay paths
- [ ] Visually impressive demo

### Game Success
- [ ] 60 FPS with 2 ships + 20 bullets
- [ ] Fun for 5+ minutes of gameplay
- [ ] Responsive controls (< 16ms input lag)
- [ ] No crashes or softlocks
- [ ] Friends want to play again

### Hybrid Success
- [ ] Shippable game in 2 weeks
- [ ] Reusable rendering library
- [ ] Foundation for visualizer
- [ ] Code portfolio piece

---

## My Recommendation

**Start with the dogfight game (Option C - Hybrid).**

**Why:**
1. **Faster win** - Playable in 12-18 hours vs 22-30 hours
2. **More motivating** - Interactive game > passive visualizer
3. **Proves technology** - If rendering works for game, it works for visualizer
4. **Shareable** - Friends can play, builds momentum
5. **Flexible** - Can add visualizer as "demo mode" later

**Start today:**
```bash
# Test PR #1
git checkout claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM
make clean && make
./build/terminal-stars

# If it works, create game branch
git checkout -b feature/dogfight-mvp
# Start Phase 0: Extract rendering code
```

**What do you think? Which path excites you most?**
