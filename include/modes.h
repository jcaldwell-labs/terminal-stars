#ifndef MODES_H
#define MODES_H

#include "types.h"
#include <stdbool.h>

// Initialize game state
void modes_init_game_state(GameState *state);

// Initialize training session
void modes_init_training(TrainingSession *training);

// Update training targets and check collisions
void modes_update_training(TrainingSession *training, WeaponsSystem *weapons, double dt);

// Render training HUD overlay
void modes_render_training_hud(FrameBuffer *fb, TrainingSession *training);

// Render mode selection menu
void modes_render_menu(FrameBuffer *fb, int selection);

// Handle menu input and return true if mode selected
bool modes_handle_menu_input(int key, int *selection, GameMode *mode);

// Initialize players based on game mode
void modes_setup_players(GameState *state, Ship3D *player1, Ship3D *player2);

// Spawn training targets in various patterns
void modes_spawn_training_targets(TrainingSession *training, int pattern);

// Training target patterns
typedef enum {
    PATTERN_STATIC_GRID,     // Grid of static targets
    PATTERN_MOVING_LINE,     // Line of moving targets
    PATTERN_ORBITING,        // Targets orbiting center
    PATTERN_RANDOM           // Random positions
} TargetPattern;

// ============================================================================
// SKEET MODE FUNCTIONS
// ============================================================================

// Initialize skeet session
void modes_init_skeet(SkeetSession *skeet);

// Update skeet session - spawn pigeons, check collisions
void modes_update_skeet(SkeetSession *skeet, WeaponsSystem *weapons, double dt);

// Render skeet HUD overlay
void modes_render_skeet_hud(FrameBuffer *fb, SkeetSession *skeet);

// Render all clay pigeons
void modes_render_clay_pigeons(FrameBuffer *fb, SkeetSession *skeet, const Camera *camera, double zoom);

// Launch a clay pigeon from specified launcher position
void modes_launch_clay_pigeon(SkeetSession *skeet, LauncherPosition launcher);

#endif // MODES_H
