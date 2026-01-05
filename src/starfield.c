/*
 * starfield.c - Starfield Management
 *
 * This module handles the creation, update, and destruction of starfields.
 * A starfield is a collection of stars with properties that control
 * their animation and appearance.
 *
 * Key Concepts:
 * - Object pooling: All stars are pre-allocated in a single array
 * - Recycling: Stars that go out of bounds are reset to new random positions
 * - Effect dispatch: The effect_mode selects which animation to apply
 */

#include "starfield.h"
#include "effects.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * rand_range - Generate random double in range [min, max]
 *
 * Uses the standard rand() function scaled to the desired range.
 * Note: For better randomness in production, consider using
 * a better PRNG like xorshift or reading from /dev/urandom.
 */
static double rand_range(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

/*
 * init_star - Initialize a single star with random properties
 *
 * Stars are placed randomly in a 3D cube:
 * - X, Y: -max_radius to +max_radius
 * - Z: 0.1 (closest) to max_depth (farthest)
 *
 * Brightness affects both the color and character displayed.
 * Brighter stars get more visible characters.
 */
static void init_star(Star *star, double max_radius, double max_depth) {
    // Random position in 3D space
    star->x = rand_range(-max_radius, max_radius);
    star->y = rand_range(-max_radius, max_radius);
    star->z = rand_range(0.1, max_depth);

    // Random brightness (50-255 range)
    star->brightness = (uint8_t)rand_range(50, 255);

    // Select character based on brightness - brighter stars are more prominent
    if (star->brightness > 200) {
        star->character = '*';  // Very bright - full star
    } else if (star->brightness > 150) {
        star->character = '+';  // Bright - plus sign
    } else if (star->brightness > 100) {
        star->character = '.';  // Medium - dot
    } else {
        star->character = '\''; // Dim - tiny mark
    }
}

/*
 * starfield_create - Create a new starfield with the given number of stars
 *
 * Allocates and initializes:
 * - The Starfield structure
 * - The star array (pre-allocated for efficiency)
 * - Default parameters (speed, zoom, effect, camera)
 *
 * Returns NULL on allocation failure.
 */
Starfield* starfield_create(size_t star_count) {
    if (star_count == 0) {
        return NULL;
    }

    // Seed random number generator for star positions
    srand((unsigned int)time(NULL));

    // Allocate the starfield structure
    Starfield *field = malloc(sizeof(Starfield));
    if (!field) {
        return NULL;
    }

    // Allocate the star array (all at once for cache efficiency)
    field->stars = malloc(star_count * sizeof(Star));
    if (!field->stars) {
        free(field);
        return NULL;
    }

    // Set default parameters
    field->star_count = star_count;
    field->speed = 1.0;           // Normal speed
    field->zoom = 50.0;           // Default perspective
    field->effect_mode = EFFECT_LINEAR;

    // Initialize camera at origin, looking forward
    field->camera.pos_x = 0.0;
    field->camera.pos_y = 0.0;
    field->camera.pos_z = 0.0;
    field->camera.yaw = 0.0;
    field->camera.pitch = 0.0;
    field->camera.roll = 0.0;

    // Initialize all stars with random positions
    for (size_t i = 0; i < star_count; i++) {
        init_star(&field->stars[i], 100.0, 50.0);
    }

    return field;
}

/*
 * starfield_update - Update starfield for one frame
 *
 * This is called every frame to:
 * 1. Apply the current visual effect (moves stars)
 * 2. Recycle stars that have gone out of bounds
 *
 * Star recycling creates the illusion of infinite stars - when a star
 * passes behind the viewer, it's reset to a new random position in the
 * distance.
 */
void starfield_update(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Apply the current effect (this moves the stars)
    starfield_apply_effect(field, field->effect_mode);

    // Recycle stars that have gone out of bounds
    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Reset stars that are behind the viewer or too far away
        if (star->z < 0.1 || star->z > 100.0) {
            init_star(star, 100.0, 50.0);
        }

        // Reset stars that have drifted too far from center (X/Y bounds)
        if (star->x < -200.0 || star->x > 200.0 ||
            star->y < -200.0 || star->y > 200.0) {
            init_star(star, 100.0, 50.0);
        }
    }

    // delta_time is passed to effect functions via starfield_apply_effect
    (void)delta_time;
}

/*
 * starfield_apply_effect - Apply the selected visual effect
 *
 * This dispatches to the appropriate effect function based on effect_mode.
 * Each effect modifies star positions differently to create unique
 * visual patterns.
 *
 * The 0.016 delta time approximates 60fps. For truly smooth animation,
 * the actual frame delta should be passed through.
 */
void starfield_apply_effect(Starfield *field, int effect_mode) {
    if (!field) {
        return;
    }

    // Dispatch to appropriate effect function
    // Each effect implements a different star motion pattern
    switch (effect_mode) {
        case EFFECT_LINEAR:
            effect_linear(field, 0.016);   // Classic forward motion
            break;
        case EFFECT_SPIRAL:
            effect_spiral(field, 0.016);   // Rotating spiral
            break;
        case EFFECT_WARP:
            effect_warp(field, 0.016);     // Hyperspeed streaking
            break;
        case EFFECT_TUNNEL:
            effect_tunnel(field, 0.016);   // Cylindrical tunnel
            break;
        case EFFECT_EXPLODE:
            effect_explode(field, 0.016);  // Outward explosion
            break;
        case EFFECT_WAVE:
            effect_wave(field, 0.016);     // Sinusoidal waves
            break;
        default:
            effect_linear(field, 0.016);   // Fallback to linear
            break;
    }
}

/*
 * starfield_destroy - Free all starfield memory
 *
 * Always free the star array before the structure to prevent leaks.
 */
void starfield_destroy(Starfield *field) {
    if (field) {
        free(field->stars);
        free(field);
    }
}
