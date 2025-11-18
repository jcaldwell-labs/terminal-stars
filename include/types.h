#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Star structure for individual stars in the field
typedef struct {
    double x;           // X position in 3D space
    double y;           // Y position in 3D space
    double z;           // Z position (depth)
    uint8_t brightness; // Star brightness (0-255)
    char character;     // Character to render ('.' '*' '+' etc)
} Star;

// Camera system for POV control
typedef struct {
    double pos_x;       // Camera position X
    double pos_y;       // Camera position Y
    double pos_z;       // Camera position Z
    double yaw;         // Rotation around Y axis (left/right turn)
    double pitch;       // Rotation around X axis (up/down look)
    double roll;        // Rotation around Z axis (banking)
} Camera;

// Starfield configuration and state
typedef struct {
    Star *stars;           // Array of stars
    size_t star_count;     // Number of stars
    double speed;          // Motion speed
    Camera camera;         // Camera with position and orientation
    double zoom;           // Zoom level
    int effect_mode;       // Current effect mode

    // Torus path parameters (for EFFECT_TORUS)
    double torus_t;        // Path parameter (0 to 2π)
    double torus_major_r;  // Major radius (distance from center)
    double torus_minor_r;  // Minor radius (tube thickness)
} Starfield;

// Frame buffer for double buffering
typedef struct {
    char *buffer;          // Character buffer
    int *colors;           // Color buffer (optional)
    int width;             // Buffer width
    int height;            // Buffer height
} FrameBuffer;

// ============================================================================
// 3D FLIGHT SIMULATOR TYPES
// ============================================================================

// Camera view mode
typedef enum {
    VIEW_COCKPIT,          // First-person cockpit view
    VIEW_CHASE,            // Third-person chase camera
    VIEW_FREE              // Free camera (debug)
} ViewMode;

// Game mode - different play modes
typedef enum {
    MODE_SINGLE_PLAYER,    // Single player vs AI
    MODE_DUAL_PLAYER,      // Two player competitive
    MODE_COOP,             // Two player cooperative
    MODE_TRAINING,         // Training simulator mode
    MODE_SKEET             // Skeet shooting mode
} GameMode;

// Player control mode
typedef enum {
    CONTROL_KEYBOARD,      // Keyboard controls
    CONTROL_JOYSTICK,      // Joystick/gamepad controls
    CONTROL_AI,            // AI controlled
    CONTROL_INACTIVE       // Player slot inactive
} ControlMode;

// AI behavior type
typedef enum {
    AI_ORBITAL,            // Orbit around center
    AI_HUNTING,            // Chase and attack player
    AI_EVASIVE,            // Avoid player attacks
    AI_FORMATION,          // Fly in formation
    AI_TRAINING_DUMMY      // Static training target
} AIBehaviorType;

// Ship3D - Player's spaceship with 6-DOF physics
typedef struct {
    // Position in 3D space
    double x, y, z;

    // Orientation (Euler angles in radians)
    double pitch;          // Rotation around X axis (nose up/down)
    double yaw;            // Rotation around Y axis (nose left/right)
    double roll;           // Rotation around Z axis (banking)

    // Linear velocity
    double velocity_x;
    double velocity_y;
    double velocity_z;

    // Angular velocity (for smooth rotation)
    double angular_pitch;
    double angular_yaw;
    double angular_roll;

    // Physics parameters
    double thrust;         // Current thrust level (0.0 to 1.0)
    double max_speed;      // Maximum velocity
    double acceleration;   // Thrust acceleration
    double turn_rate;      // Rotation speed (radians/sec)
    double drag;           // Air/space drag coefficient

    // Player info
    int player_id;         // 0 or 1
    int health;
    bool active;

    // Control system
    ControlMode control_mode;     // How this ship is controlled
    AIBehaviorType ai_behavior;   // AI behavior (if AI controlled)
    int joystick_id;              // SDL joystick ID (-1 for none)

    // Camera tracking
    ViewMode view_mode;    // Current camera view
    double chase_distance; // Distance for chase cam
} Ship3D;

// ============================================================================
// WEAPONS SYSTEM TYPES
// ============================================================================

#define MAX_MISSILES 100
#define MAX_EXPLOSIONS 50

// Missile - laser projectile
typedef struct {
    // Position in 3D space
    double x, y, z;

    // Velocity
    double velocity_x;
    double velocity_y;
    double velocity_z;

    // Target point (where it's heading)
    double target_x;
    double target_y;
    double target_z;

    // Physics
    double speed;           // Missile speed
    double lifetime;        // Time until self-destruct
    double age;             // Current age

    // Ownership
    int owner_id;           // Which player fired it
    bool active;            // Is this missile slot in use

    // Visual
    char trail_char;        // Character for missile trail
    int color;              // Color pair
} Missile;

// Explosion - visual effect when missile detonates
typedef struct {
    // Position in 3D space
    double x, y, z;

    // Animation
    double age;             // Time since explosion started
    double max_age;         // Total explosion duration
    double radius;          // Current radius
    double max_radius;      // Maximum radius

    // Visual
    bool active;            // Is this explosion slot in use
    int color;              // Color pair
} Explosion;

// Weapons System - manages all missiles and explosions
typedef struct {
    Missile missiles[MAX_MISSILES];
    Explosion explosions[MAX_EXPLOSIONS];

    // Player weapon stats
    int missiles_remaining[2];  // Per player
    double fire_cooldown[2];    // Per player
} WeaponsSystem;

// ============================================================================
// TRAINING MODE TYPES
// ============================================================================

#define MAX_TRAINING_TARGETS 20

// Training target - static or moving targets for practice
typedef struct {
    // Position in 3D space
    double x, y, z;

    // Velocity (for moving targets)
    double velocity_x;
    double velocity_y;
    double velocity_z;

    // Target properties
    double radius;          // Size of target
    int points;             // Points awarded for hit
    bool active;            // Is this target active
    bool is_static;         // Static or moving target
    int health;             // Hits required to destroy

    // Visual
    char character;         // Display character
    int color;              // Color pair
} TrainingTarget;

// Training session data
typedef struct {
    TrainingTarget targets[MAX_TRAINING_TARGETS];
    int score;              // Player's score
    int targets_hit;        // Number of targets destroyed
    int shots_fired;        // Total shots fired
    double session_time;    // Time elapsed in training
    double accuracy;        // Hit accuracy percentage
} TrainingSession;

// ============================================================================
// SKEET SHOOTING MODE TYPES
// ============================================================================

#define MAX_CLAY_PIGEONS 30

// Skeet mode physics constants
#define CLAY_PIGEON_HIT_RADIUS 30.0
#define CLAY_PIGEON_GRAVITY 50.0
#define CLAY_PIGEON_LIFETIME 5.0
#define CLAY_PIGEON_DEBRIS_TIME 0.5
#define SKEET_DIFFICULTY_INTERVAL 30.0
#define SKEET_INITIAL_LAUNCH_DELAY 2.0

// Clay pigeon - target that flies through the air in an arc
typedef struct {
    // Position in 3D space
    double x, y, z;

    // Velocity
    double velocity_x;
    double velocity_y;
    double velocity_z;

    // Physics
    double age;             // Time since launch
    double lifetime;        // Time until self-destruct (missed)
    int points;             // Points awarded for hit
    bool active;            // Is this pigeon active
    bool hit;               // Was it hit by the player

    // Visual
    char character;         // Display character
    int color;              // Color pair
} ClayPigeon;

// Launcher position - where clay pigeons are launched from
typedef enum {
    LAUNCHER_BOTTOM_LEFT,
    LAUNCHER_BOTTOM_RIGHT,
    LAUNCHER_BOTTOM_CENTER
} LauncherPosition;

// Skeet session data
typedef struct {
    ClayPigeon pigeons[MAX_CLAY_PIGEONS];
    int score;              // Player's score
    int pigeons_hit;        // Number of pigeons hit
    int pigeons_launched;   // Total pigeons launched
    int pigeons_missed;     // Pigeons that flew away
    double session_time;    // Time elapsed in session
    double accuracy;        // Hit accuracy percentage
    double next_launch_time; // Time until next pigeon launch
    int difficulty;         // 0-3 difficulty level
} SkeetSession;

// ============================================================================
// GAME STATE
// ============================================================================

// Game state - encapsulates all game data
typedef struct {
    GameMode mode;          // Current game mode
    bool paused;            // Is game paused
    bool show_menu;         // Show mode selection menu
    int menu_selection;     // Current menu selection
    TrainingSession *training;  // Training session data (if in training mode)
    SkeetSession *skeet;    // Skeet session data (if in skeet mode)
} GameState;

#endif // TYPES_H
