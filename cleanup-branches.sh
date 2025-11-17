#!/bin/bash
# Branch Cleanup Script for terminal-stars
# Generated: 2025-11-17

set -e  # Exit on error

echo "================================"
echo "Terminal-Stars Branch Cleanup"
echo "================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to confirm action
confirm() {
    read -p "$1 (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        return 1
    fi
    return 0
}

echo "Current branch status:"
echo ""
echo "MERGED to master:"
echo "  ✓ claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM (PR #1)"
echo ""
echo "RECOMMENDED to merge:"
echo "  → claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE (Two-player flight sim)"
echo ""
echo "SUPERSEDED (can delete):"
echo "  × claude/3d-flight-simulator-017FqJnva9yQ1q2tdhEuj2JE"
echo "  × claude/3d-flight-v2-017FqJnva9yQ1q2tdhEuj2JE"
echo "  × claude/dogfight-mvp-017FqJnva9yQ1q2tdhEuj2JE"
echo ""
echo "DOCUMENTATION (review first):"
echo "  ? claude/game-handoff-docs-01RMLDwMGDztTRZp7q4sx96K"
echo ""

# Step 1: Delete already-merged branch
echo -e "${YELLOW}Step 1: Delete merged PR #1 branch${NC}"
if confirm "Delete claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM?"; then
    echo "Deleting remote branch..."
    git push origin --delete claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM || echo "Remote branch already deleted"

    echo "Deleting local branch if it exists..."
    git branch -d claude/review-pro-01Qg3jwQqHnacvqdKBe4uzdM 2>/dev/null || echo "Local branch doesn't exist"

    echo -e "${GREEN}✓ Merged branch deleted${NC}"
else
    echo "Skipped"
fi
echo ""

# Step 2: Delete superseded branches
echo -e "${YELLOW}Step 2: Delete superseded branches${NC}"
if confirm "Delete superseded branches (3d-flight-simulator, 3d-flight-v2, dogfight-mvp)?"; then
    echo "Deleting claude/3d-flight-simulator-017FqJnva9yQ1q2tdhEuj2JE..."
    git push origin --delete claude/3d-flight-simulator-017FqJnva9yQ1q2tdhEuj2JE || echo "Already deleted"

    echo "Deleting claude/3d-flight-v2-017FqJnva9yQ1q2tdhEuj2JE..."
    git push origin --delete claude/3d-flight-v2-017FqJnva9yQ1q2tdhEuj2JE || echo "Already deleted"

    echo "Deleting claude/dogfight-mvp-017FqJnva9yQ1q2tdhEuj2JE..."
    git push origin --delete claude/dogfight-mvp-017FqJnva9yQ1q2tdhEuj2JE || echo "Already deleted"

    # Delete local branches
    git branch -d claude/3d-flight-v2-017FqJnva9yQ1q2tdhEuj2JE 2>/dev/null || echo "Local 3d-flight-v2 doesn't exist"

    echo -e "${GREEN}✓ Superseded branches deleted${NC}"
else
    echo "Skipped"
fi
echo ""

# Step 3: Review documentation branch
echo -e "${YELLOW}Step 3: Documentation branch${NC}"
echo "The game-handoff-docs branch contains:"
echo "  - docs/GAME-*.md files"
echo "  - tmux demo scripts"
echo ""
if confirm "Check out game-handoff-docs branch for review?"; then
    git checkout claude/game-handoff-docs-01RMLDwMGDztTRZp7q4sx96K
    git pull origin claude/game-handoff-docs-01RMLDwMGDztTRZp7q4sx96K
    echo ""
    echo "Files in this branch:"
    git diff master...HEAD --name-only
    echo ""
    echo "Review the files. When done, you can:"
    echo "  1. Cherry-pick useful docs to master"
    echo "  2. Delete this branch"
    echo ""
    read -p "Press enter to continue..."
    git checkout master
else
    echo "Skipped - you can review later"
fi
echo ""

# Step 4: Merge the primary feature branch
echo -e "${YELLOW}Step 4: Merge two-player flight simulator${NC}"
echo "This will merge claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE"
echo "This branch adds:"
echo "  - Two-player 3D flight simulator"
echo "  - Ship3D physics system"
echo "  - Manual controls for both players"
echo "  - AI toggle for Player 2"
echo "  - Cockpit view with HUD"
echo ""
if confirm "Merge this branch to master?"; then
    echo "Checking out branch..."
    git checkout claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE
    git pull origin claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE

    echo "Merging to master..."
    git checkout master
    git merge --no-ff claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE -m "Merge two-player 3D flight simulator

Implements complete two-player flight simulator with:
- Full 6-DOF physics for two ships
- Player 1 controls: WASD + Q/E + Space
- Player 2 controls: Arrows + </> + Enter
- AI toggle for Player 2 (B key)
- Cockpit and chase camera views
- 3D ship rendering with LOD system
- Comprehensive HUD

Co-Authored-By: Claude <noreply@anthropic.com>"

    echo -e "${GREEN}✓ Branch merged to master${NC}"

    if confirm "Push master to origin?"; then
        git push origin master
        echo -e "${GREEN}✓ Pushed to origin${NC}"
    fi

    if confirm "Delete the merged branch?"; then
        git push origin --delete claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE
        git branch -d claude/review-terminal-stars-pr-017FqJnva9yQ1q2tdhEuj2JE
        echo -e "${GREEN}✓ Merged branch deleted${NC}"
    fi
else
    echo "Skipped - you can merge manually later"
fi
echo ""

# Step 5: Clean up local tracking branches
echo -e "${YELLOW}Step 5: Clean up local branch tracking${NC}"
if confirm "Prune deleted remote branches?"; then
    git fetch --prune
    echo -e "${GREEN}✓ Pruned remote tracking branches${NC}"
fi
echo ""

# Final status
echo "================================"
echo "Cleanup Complete!"
echo "================================"
echo ""
echo "Current branches:"
git branch -a
echo ""
echo "See BRANCH-CLEANUP-ANALYSIS.md for full details."
