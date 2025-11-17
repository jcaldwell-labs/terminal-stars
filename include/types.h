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

// Starfield configuration and state
typedef struct {
    Star *stars;           // Array of stars
    size_t star_count;     // Number of stars
    double speed;          // Motion speed
    double rotation_x;     // Rotation around X axis
    double rotation_y;     // Rotation around Y axis
    double rotation_z;     // Rotation around Z axis
    double zoom;           // Zoom level
    int effect_mode;       // Current effect mode
} Starfield;

// Frame buffer for double buffering
typedef struct {
    char *buffer;          // Character buffer
    int *colors;           // Color buffer (optional)
    int width;             // Buffer width
    int height;            // Buffer height
} FrameBuffer;

#endif // TYPES_H
