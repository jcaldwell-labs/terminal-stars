#include "effects.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void effect_linear(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Classic starfield - stars move straight toward viewer
    double speed_factor = field->speed * 30.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];
        // Move star toward viewer (decrease z)
        star->z -= speed_factor;
    }
}

void effect_spiral(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Stars spiral while moving toward viewer
    double speed_factor = field->speed * 30.0 * delta_time;
    double rotation_speed = field->speed * 2.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Convert to polar coordinates
        double radius = sqrt(star->x * star->x + star->y * star->y);
        double angle = atan2(star->y, star->x);

        // Rotate
        angle += rotation_speed;

        // Convert back to cartesian
        star->x = radius * cos(angle);
        star->y = radius * sin(angle);

        // Move toward viewer
        star->z -= speed_factor;
    }
}

void effect_warp(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Fast motion with motion blur (stretch stars)
    double speed_factor = field->speed * 80.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Very fast movement creates warp effect
        star->z -= speed_factor;

        // Change character to create streak effect
        if (star->z < 10.0) {
            star->character = '-';
        } else if (star->z < 20.0) {
            star->character = '=';
        } else {
            star->character = '*';
        }
    }
}

void effect_tunnel(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Stars move in a cylindrical pattern
    double speed_factor = field->speed * 25.0 * delta_time;
    double rotation_speed = field->speed * 1.5 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Rotate around Z axis
        double angle = atan2(star->y, star->x);
        double radius = sqrt(star->x * star->x + star->y * star->y);

        angle += rotation_speed;

        star->x = radius * cos(angle);
        star->y = radius * sin(angle);

        // Move along tunnel
        star->z -= speed_factor;

        // Keep stars on cylinder surface
        if (radius < 30.0) {
            radius = 30.0;
            star->x = radius * cos(angle);
            star->y = radius * sin(angle);
        }
    }
}

void effect_explode(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Stars explode outward from center in all directions
    double speed_factor = field->speed * 40.0 * delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Calculate direction from center
        double dx = star->x;
        double dy = star->y;
        double dz = star->z - 25.0; // Center point in depth

        // Normalize direction
        double length = sqrt(dx*dx + dy*dy + dz*dz);
        if (length > 0.1) {
            dx /= length;
            dy /= length;
            dz /= length;

            // Move in that direction
            star->x += dx * speed_factor;
            star->y += dy * speed_factor;
            star->z += dz * speed_factor;
        }
    }
}

void effect_wave(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Stars move in sinusoidal wave patterns
    double speed_factor = field->speed * 25.0 * delta_time;
    static double time_accumulator = 0.0;
    time_accumulator += delta_time;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Create wave motion in X and Y based on Z position
        double wave_freq = 2.0;
        double wave_amplitude = 15.0;

        // X-axis wave
        star->x += sin(star->z * 0.1 + time_accumulator * wave_freq) *
                   wave_amplitude * delta_time;

        // Y-axis wave (phase shifted)
        star->y += cos(star->z * 0.1 + time_accumulator * wave_freq + M_PI/2) *
                   wave_amplitude * delta_time;

        // Move toward viewer
        star->z -= speed_factor;
    }
}

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
