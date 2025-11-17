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

    // Camera tracking
    ViewMode view_mode;    // Current camera view
    double chase_distance; // Distance for chase cam
} Ship3D;

#endif // TYPES_H
