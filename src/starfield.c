#include "starfield.h"
#include "effects.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Random float between min and max
static double rand_range(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// Initialize a single star with random position
static void init_star(Star *star, double max_radius, double max_depth) {
    // Random position in 3D space
    star->x = rand_range(-max_radius, max_radius);
    star->y = rand_range(-max_radius, max_radius);
    star->z = rand_range(0.1, max_depth);

    // Random brightness
    star->brightness = (uint8_t)rand_range(50, 255);

    // Select character based on brightness
    if (star->brightness > 200) {
        star->character = '*';
    } else if (star->brightness > 150) {
        star->character = '+';
    } else if (star->brightness > 100) {
        star->character = '.';
    } else {
        star->character = '\'';
    }
}

Starfield* starfield_create(size_t star_count) {
    if (star_count == 0) {
        return NULL;
    }

    // Seed random number generator
    srand(time(NULL));

    Starfield *field = malloc(sizeof(Starfield));
    if (!field) {
        return NULL;
    }

    field->stars = malloc(star_count * sizeof(Star));
    if (!field->stars) {
        free(field);
        return NULL;
    }

    field->star_count = star_count;
    field->speed = 1.0;
    field->rotation_x = 0.0;
    field->rotation_y = 0.0;
    field->rotation_z = 0.0;
    field->zoom = 50.0;
    field->effect_mode = EFFECT_LINEAR;

    // Initialize all stars
    for (size_t i = 0; i < star_count; i++) {
        init_star(&field->stars[i], 100.0, 50.0);
    }

    return field;
}

void starfield_update(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Apply the current effect
    starfield_apply_effect(field, field->effect_mode);

    // Reset stars that have gone too close or too far
    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Reset stars that are behind the viewer or too far away
        if (star->z < 0.1 || star->z > 100.0) {
            init_star(star, 100.0, 50.0);
        }

        // Keep stars within reasonable X/Y bounds
        if (star->x < -200.0 || star->x > 200.0 ||
            star->y < -200.0 || star->y > 200.0) {
            init_star(star, 100.0, 50.0);
        }
    }

    (void)delta_time; // Used by effects, not here directly
}

void starfield_apply_effect(Starfield *field, int effect_mode) {
    if (!field) {
        return;
    }

    // Dispatch to appropriate effect function
    switch (effect_mode) {
        case EFFECT_LINEAR:
            effect_linear(field, 0.016); // ~60fps delta
            break;
        case EFFECT_SPIRAL:
            effect_spiral(field, 0.016);
            break;
        case EFFECT_WARP:
            effect_warp(field, 0.016);
            break;
        case EFFECT_TUNNEL:
            effect_tunnel(field, 0.016);
            break;
        case EFFECT_EXPLODE:
            effect_explode(field, 0.016);
            break;
        case EFFECT_WAVE:
            effect_wave(field, 0.016);
            break;
        default:
            effect_linear(field, 0.016);
            break;
    }
}

void starfield_destroy(Starfield *field) {
    if (field) {
        free(field->stars);
        free(field);
    }
}
