/*
 * types.h - Core Data Types for Terminal Stars
 *
 * This file defines the fundamental data structures for starfield visualization:
 * - Star: Individual star with position, brightness, and display character
 * - Camera: Viewer position and orientation in 3D space
 * - Starfield: Collection of stars with animation parameters
 * - FrameBuffer: Double-buffered terminal rendering
 *
 * Educational Note: These structures demonstrate how to represent 3D graphics
 * data in C. The separation of concerns (position vs. rendering) makes the
 * code more maintainable and easier to understand.
 */

#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Star - Individual star in the starfield
 *
 * Each star has a 3D position (x, y, z) where:
 * - x/y define the position perpendicular to the viewing direction
 * - z is the depth (distance from viewer) - stars with z close to 0 appear larger
 *
 * Brightness affects both the color and character used for rendering.
 */
typedef struct {
    double x;           // X position in 3D space (-100 to 100)
    double y;           // Y position in 3D space (-100 to 100)
    double z;           // Z position (depth) - 0.1 to 50.0
    uint8_t brightness; // Star brightness (0-255), affects color/character
    char character;     // Character to render ('.' '*' '+' '\'')
} Star;

/*
 * Camera - Viewer position and orientation
 *
 * The camera defines the point-of-view for rendering. All star positions
 * are transformed relative to the camera before projection to 2D.
 *
 * Orientation uses Euler angles (yaw, pitch, roll) in radians.
 */
typedef struct {
    double pos_x;       // Camera position X
    double pos_y;       // Camera position Y
    double pos_z;       // Camera position Z
    double yaw;         // Rotation around Y axis (left/right turn)
    double pitch;       // Rotation around X axis (up/down look)
    double roll;        // Rotation around Z axis (banking)
} Camera;

/*
 * Starfield - The complete starfield with all configuration
 *
 * Contains the array of stars and parameters that control animation:
 * - speed: How fast stars move (1.0 = normal)
 * - zoom: Perspective projection scale (higher = more 3D effect)
 * - effect_mode: Which visual effect is active (see effects.h)
 */
typedef struct {
    Star *stars;           // Dynamic array of stars
    size_t star_count;     // Number of stars in the array
    double speed;          // Motion speed multiplier (0.1 to 5.0)
    Camera camera;         // Camera position and orientation
    double zoom;           // Zoom/perspective level (10.0 to 200.0)
    int effect_mode;       // Current visual effect (0-5)
} Starfield;

/*
 * FrameBuffer - Double-buffered terminal output
 *
 * Instead of writing directly to the terminal (which causes flicker),
 * we build the entire frame in a buffer, then display it all at once.
 *
 * This is the fundamental technique for smooth terminal animation.
 */
typedef struct {
    char *buffer;          // Character buffer (width * height)
    int *colors;           // Color pair for each position
    int width;             // Buffer width (terminal columns)
    int height;            // Buffer height (terminal rows)
} FrameBuffer;

#endif // TYPES_H
