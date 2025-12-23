# Branch Cleanup Analysis

**Date**: 2025-11-17
**Current Master**: db2d3dc (PR #1 merged)
**Total Branches**: 7 (6 feature + 1 master)

## Branch Status Summary

### ✅ MERGED
- `origin/claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM` - **PR #1 MERGED**
  - 6-DOF camera system + torus roller coaster effect
  - Status: Safe to delete

### 🎯 RECOMMENDED TO MERGE

#### **Priority 1: Keep and Merge**
- **`origin/claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE`** - **RECOMMENDED PRIMARY BRANCH**
  - Contains: Two-player flight simulator with manual controls + AI toggle
  - Commits:
    - a907ae2 Add two-player manual controls with AI toggle
    - dbf7b6a feat: Add second ship with 3D rendering and orbital AI
    - f317811 Add unified roadmap consolidating all development paths
    - a74f07a Add comprehensive PR #1 review and feature decomposition
  - Why: Most complete implementation aligned with user's vision
  - Action: Create PR to merge to master

### 📦 CONTAINS USEFUL WORK

#### **Can Be Deleted (Work Superseded)**
- **`origin/claude/3d-flight-simulator-017FqJnva9yQ1q2tdhEuj2JE`**
  - Contains: Original single-player 3D flight simulator
  - Commits:
    - 076b0d0 docs: Add 3D flight simulator guide and roadmap
    - 7abaed6 feat: Implement 3D space flight simulator with cockpit view
  - Why superseded: All work evolved into review-terminal-stars-pr branch
  - Action: Delete (work preserved in newer branch)

- **`origin/claude/3d-flight-v2-017FqJnva9yQ1q2tdhEuj2JE`**
  - Contains: Second ship with orbital AI (intermediate step)
  - Commits:
    - eabfcf2 feat: Add second ship with 3D rendering and orbital AI
    - a74f07a Add comprehensive PR #1 review and feature decomposition
  - Why superseded: These commits exist in review-terminal-stars-pr with additional improvements
  - Action: Delete (work preserved in review-terminal-stars-pr)

- **`origin/claude/dogfight-mvp-017FqJnva9yQ1q2tdhEuj2JE`**
  - Contains: 2D dogfight game MVP
  - Commits:
    - bc9ded2 docs: Add comprehensive MVP documentation and next steps
    - b6625ba feat: Implement complete 2-player dogfight game MVP
  - Why not merge: User pivoted away from 2D dogfight to 3D flight simulator
  - Action: Delete (different direction from current vision)

### 📚 DOCUMENTATION ONLY

- **`origin/claude/game-handoff-docs-01RMLDwMGDztTRZp7q4sx96K`**
  - Contains: Game development handoff documentation
  - Commits:
    - 6fe8b13 docs: Add comprehensive game development handoff package
  - Action: Review docs, merge useful parts into main docs, then delete

## Recommended Cleanup Actions

### Step 1: Merge the Primary Feature Branch
```bash
# Create PR for the two-player flight simulator
git checkout claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE
git pull origin claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE
gh pr create --title "Two-player 3D flight simulator with manual controls" \
  --body "Implements complete two-player 3D flight simulator with cockpit view"
```

### Step 2: Delete Merged Branches
```bash
# Delete the already-merged PR #1 branch
git push origin --delete claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM
git branch -d claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM  # if exists locally
```

### Step 3: Delete Superseded Branches
```bash
# These branches' work is preserved in review-terminal-stars-pr
git push origin --delete claude/3d-flight-simulator-017FqJnva9yQ1q2tdhEuj2JE
git push origin --delete claude/3d-flight-v2-017FqJnva9yQ1q2tdhEuj2JE

# Delete abandoned 2D dogfight branch
git push origin --delete claude/dogfight-mvp-017FqJnva9yQ1q2tdhEuj2JE
```

### Step 4: Handle Documentation Branch
```bash
# Review and potentially merge documentation
git checkout claude/game-handoff-docs-01RMLDwMGDztTRZp7q4sx96K
# Review files, extract useful content
# Then delete
git push origin --delete claude/game-handoff-docs-01RMLDwMGDztTRZp7q4sx96K
```

### Step 5: Clean Up Local Branches
```bash
git checkout master
git pull origin master
git branch -d claude/3d-flight-v2-017FqJnva9yQ1q2tdhEuj2JE  # if exists
git branch -d claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE  # after merging
# etc.
```

## Final State
After cleanup, you should have:
- **master**: Contains PR #1 (6-DOF camera + torus) + Two-player flight simulator
- **No feature branches**: All work merged or properly archived

## Comparison: What's in Master vs What Should Be

### Currently in Master (PR #1)
- 6-DOF camera with free roaming
- Torus roller coaster effect
- 7 starfield effects (LINEAR, SPIRAL, WARP, TUNNEL, EXPLODE, WAVE, TORUS)
- Camera controls (WASDQE)

### What review-terminal-stars-pr Adds
- Ship3D physics system with arcade-style controls
- Two-player ship rendering in 3D space
- Player 1 manual controls (WASD + Q/E + Space)
- Player 2 manual controls (Arrows + </> + Enter)
- AI toggle for Player 2 (B key)
- Cockpit view with HUD (speed, heading, altitude, enemy distance)
- Chase camera view option (V key)
- LOD-based ship rendering
- Orbital AI pattern for Player 2

### Conflict Resolution
The two branches modify the same files extensively. Merging will require:
- Deciding whether to keep PR #1's free camera OR replace with ship-based camera
- Potentially supporting both modes (free camera vs flight simulator)
- Careful review of src/main.c, render.c, types.h changes

## Recommendation

**Option A: Replace with Flight Simulator** (Recommended)
- Merge review-terminal-stars-pr as-is (may lose some PR #1 features)
- User's stated vision: "separate terminals for each player - star backgrounds - see each other fly around"
- This matches the flight simulator better than free camera

**Option B: Preserve Both Modes**
- Add mode toggle to switch between free camera and flight simulator
- More complex but preserves all work
- Requires additional integration work

User should decide which approach aligns with their vision.
