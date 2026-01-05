/*
 * effects.c - Starfield Visual Effects Implementation
 *
 * This file implements 6 different starfield animation effects.
 * Each effect demonstrates a different animation technique.
 *
 * Key Concepts:
 * - Delta-time animation: All movements scaled by time for frame-rate independence
 * - Coordinate systems: Cartesian (x,y) and polar (radius, angle)
 * - Vector mathematics: Direction, normalization, magnitude
 * - Trigonometry: sine/cosine for rotation and oscillation
 */

#include "effects.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
 * EFFECT_LINEAR - Classic Starfield
 *
 * The simplest effect: stars move straight toward the viewer by
 * decreasing their Z coordinate (depth) each frame.
 *
 * Educational: This demonstrates basic translation animation.
 * The speed_factor combines user speed setting with a base rate
 * and delta_time for smooth, consistent motion.
 */
void effect_linear(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Calculate movement per frame: base_speed * user_speed * time
    double speed_factor = field->speed * 30.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];
        // Move star toward viewer (decrease z = closer)
        star->z -= speed_factor;
    }
}

/*
 * EFFECT_SPIRAL - Rotating Starfield
 *
 * Stars rotate around the Z-axis while moving toward the viewer,
 * creating a hypnotic spiral pattern.
 *
 * Educational: This demonstrates polar coordinate transformation.
 * - Convert (x, y) to (radius, angle) using atan2 and sqrt
 * - Rotate by modifying the angle
 * - Convert back to (x, y) using cos and sin
 */
void effect_spiral(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    double speed_factor = field->speed * 30.0 * delta_time;
    double rotation_speed = field->speed * 2.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Convert cartesian (x, y) to polar (radius, angle)
        double radius = sqrt(star->x * star->x + star->y * star->y);
        double angle = atan2(star->y, star->x);

        // Rotate the angle
        angle += rotation_speed;

        // Convert back to cartesian
        star->x = radius * cos(angle);
        star->y = radius * sin(angle);

        // Also move toward viewer
        star->z -= speed_factor;
    }
}

/*
 * EFFECT_WARP - Hyperspeed Effect
 *
 * Creates the classic "warp speed" effect with very fast movement
 * and visual streaking represented by changing star characters.
 *
 * Educational: This demonstrates visual feedback based on state.
 * The character changes based on Z depth to simulate motion blur:
 * - Close stars (low z): '-' (stretched)
 * - Medium distance: '=' (slightly stretched)
 * - Far stars: '*' (normal)
 */
void effect_warp(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Much faster than linear (80x vs 30x base rate)
    double speed_factor = field->speed * 80.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Very fast movement
        star->z -= speed_factor;

        // Change character based on depth to create streak effect
        if (star->z < 10.0) {
            star->character = '-';  // Close = stretched horizontal
        } else if (star->z < 20.0) {
            star->character = '=';  // Medium = slightly stretched
        } else {
            star->character = '*';  // Far = normal star
        }
    }
}

/*
 * EFFECT_TUNNEL - Cylindrical Tunnel
 *
 * Stars move through a cylindrical tunnel, rotating around the
 * center while maintaining a minimum distance from the axis.
 *
 * Educational: This demonstrates constrained motion.
 * The radius is clamped to a minimum value to create the
 * "walls" of the tunnel. Stars that would be too close to
 * the center are pushed outward.
 */
void effect_tunnel(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    double speed_factor = field->speed * 25.0 * delta_time;
    double rotation_speed = field->speed * 1.5 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Rotate around Z axis (same as spiral)
        double angle = atan2(star->y, star->x);
        double radius = sqrt(star->x * star->x + star->y * star->y);

        angle += rotation_speed;

        star->x = radius * cos(angle);
        star->y = radius * sin(angle);

        // Move along tunnel (toward viewer)
        star->z -= speed_factor;

        // Constrain to tunnel wall - push stars outward if too close to center
        if (radius < 30.0) {
            radius = 30.0;
            star->x = radius * cos(angle);
            star->y = radius * sin(angle);
        }
    }
}

/*
 * EFFECT_EXPLODE - Outward Explosion
 *
 * Stars burst outward from a central point in all directions,
 * like an explosion or firework.
 *
 * Educational: This demonstrates vector-based directional movement.
 * - Calculate direction vector from center to each star
 * - Normalize the vector (make it length 1.0)
 * - Move each star along its direction vector
 */
void effect_explode(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    double speed_factor = field->speed * 40.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Calculate direction from center point (0, 0, 25)
        double dx = star->x;
        double dy = star->y;
        double dz = star->z - 25.0;  // Explosion center at z=25

        // Calculate vector length (magnitude)
        double length = sqrt(dx*dx + dy*dy + dz*dz);

        // Normalize and move (avoid division by zero)
        if (length > 0.1) {
            // Normalize: divide by length to get unit vector
            dx /= length;
            dy /= length;
            dz /= length;

            // Move star along its direction
            star->x += dx * speed_factor;
            star->y += dy * speed_factor;
            star->z += dz * speed_factor;
        }
    }
}

/*
 * EFFECT_WAVE - Sinusoidal Motion
 *
 * Stars oscillate in a wave pattern while moving forward,
 * creating an undulating, dream-like effect.
 *
 * Educational: This demonstrates trigonometric animation.
 * - sin() produces values from -1 to 1 in a wave pattern
 * - Using star->z as input creates waves through depth
 * - Using time accumulator creates animation over time
 * - Phase shifting (PI/2) makes X and Y waves offset
 */
void effect_wave(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    double speed_factor = field->speed * 25.0 * delta_time;

    // Time accumulator for continuous wave motion
    static double time_accumulator = 0.0;
    time_accumulator += delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Wave parameters
        double wave_freq = 2.0;       // How fast the wave oscillates
        double wave_amplitude = 15.0; // How far stars move side-to-side

        // X-axis wave: based on depth (z) and time
        star->x += sin(star->z * 0.1 + time_accumulator * wave_freq) *
                   wave_amplitude * delta_time;

        // Y-axis wave: phase shifted by PI/2 for circular motion
        star->y += cos(star->z * 0.1 + time_accumulator * wave_freq + M_PI/2) *
                   wave_amplitude * delta_time;

        // Also move toward viewer
        star->z -= speed_factor;
    }
}

/*
 * effect_get_name - Get human-readable effect name
 *
 * Returns a string name for each effect mode for display in the UI.
 */
const char* effect_get_name(EffectMode mode) {
    static const char* names[] = {
        "Linear",
        "Spiral",
        "Warp Speed",
        "Tunnel",
        "Explode",
        "Wave"
    };

    if (mode >= 0 && mode < EFFECT_COUNT) {
        return names[mode];
    }
    return "Unknown";
}
