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

void effect_torus(Starfield *field, double delta_time) {
    if (!field || !field->stars) {
        return;
    }

    // Update path parameter (advance along the torus)
    double path_speed = field->speed * 1.5 * delta_time;
    field->torus_t += path_speed;

    // Keep parameter in range [0, 2π]
    if (field->torus_t > 2.0 * M_PI) {
        field->torus_t -= 2.0 * M_PI;
    }

    // Calculate camera position on torus path using parametric equations
    // Using a Lissajous-style path for interesting roller coaster motion
    double major_r = field->torus_major_r;
    double minor_r = field->torus_minor_r;
    double t = field->torus_t;

    // Parametric torus path (twisted for roller coaster effect)
    field->camera.pos_x = (major_r + minor_r * cos(t * 2.0)) * cos(t);
    field->camera.pos_y = (major_r + minor_r * cos(t * 2.0)) * sin(t);
    field->camera.pos_z = minor_r * sin(t * 2.0);

    // Calculate camera orientation (look-ahead along path)
    double t_ahead = t + 0.1; // Look slightly ahead
    double look_x = (major_r + minor_r * cos(t_ahead * 2.0)) * cos(t_ahead);
    double look_y = (major_r + minor_r * cos(t_ahead * 2.0)) * sin(t_ahead);
    double look_z = minor_r * sin(t_ahead * 2.0);

    // Calculate direction vector
    double dir_x = look_x - field->camera.pos_x;
    double dir_y = look_y - field->camera.pos_y;
    double dir_z = look_z - field->camera.pos_z;

    // Convert to yaw and pitch
    field->camera.yaw = atan2(dir_y, dir_x);
    field->camera.pitch = atan2(dir_z, sqrt(dir_x*dir_x + dir_y*dir_y));

    // Add banking (roll) based on turn sharpness
    field->camera.roll = -cos(t * 2.0) * 0.5; // Banking in turns

    // Stars move past the camera (relative motion)
    // Transform stars to camera-relative coordinates
    for (size_t i = 0; i < field->star_count; i++) {
        Star *star = &field->stars[i];

        // Translate to camera space
        double rel_x = star->x - field->camera.pos_x;
        double rel_y = star->y - field->camera.pos_y;
        double rel_z = star->z - field->camera.pos_z;

        // Apply yaw rotation
        double cos_yaw = cos(-field->camera.yaw);
        double sin_yaw = sin(-field->camera.yaw);
        double temp_x = rel_x * cos_yaw - rel_y * sin_yaw;
        double temp_y = rel_x * sin_yaw + rel_y * cos_yaw;

        // Apply pitch rotation
        double cos_pitch = cos(-field->camera.pitch);
        double sin_pitch = sin(-field->camera.pitch);
        double temp_z = rel_z * cos_pitch - temp_x * sin_pitch;
        temp_x = rel_z * sin_pitch + temp_x * cos_pitch;

        // Update star position
        star->x = temp_x;
        star->y = temp_y;
        star->z = temp_z;
    }
}

const char* effect_get_name(EffectMode mode) {
    static const char* names[] = {
        "Linear",
        "Spiral",
        "Warp Speed",
        "Tunnel",
        "Explode",
        "Wave",
        "Torus Coaster"
    };

    if (mode >= 0 && mode < EFFECT_COUNT) {
        return names[mode];
    }
    return "Unknown";
}
