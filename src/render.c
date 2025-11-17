#include "render.h"
#include "effects.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

FrameBuffer* framebuffer_create(int width, int height) {
    if (width <= 0 || height <= 0) {
        return NULL;
    }

    FrameBuffer *fb = malloc(sizeof(FrameBuffer));
    if (!fb) {
        return NULL;
    }

    fb->width = width;
    fb->height = height;

    // Allocate character buffer
    size_t buffer_size = width * height;
    fb->buffer = malloc(buffer_size);
    if (!fb->buffer) {
        free(fb);
        return NULL;
    }

    // Allocate color buffer
    fb->colors = malloc(buffer_size * sizeof(int));
    if (!fb->colors) {
        free(fb->buffer);
        free(fb);
        return NULL;
    }

    // Initialize to spaces
    memset(fb->buffer, ' ', buffer_size);
    memset(fb->colors, 0, buffer_size * sizeof(int));

    return fb;
}

void framebuffer_clear(FrameBuffer *fb) {
    if (!fb) {
        return;
    }

    size_t buffer_size = fb->width * fb->height;
    memset(fb->buffer, ' ', buffer_size);
    memset(fb->colors, 0, buffer_size * sizeof(int));
}

void render_starfield(FrameBuffer *fb, const Starfield *field) {
    if (!fb || !field || !field->stars) {
        return;
    }

    int center_x = fb->width / 2;
    int center_y = fb->height / 2;

    for (size_t i = 0; i < field->star_count; i++) {
        const Star *star = &field->stars[i];

        // Apply camera rotation (except for torus effect which handles its own)
        double view_x = star->x;
        double view_y = star->y;
        double view_z = star->z;

        if (field->effect_mode != EFFECT_TORUS) {
            // Apply yaw rotation (around Y axis)
            double cos_yaw = cos(-field->camera.yaw);
            double sin_yaw = sin(-field->camera.yaw);
            double temp_x = view_x * cos_yaw - view_z * sin_yaw;
            double temp_z = view_x * sin_yaw + view_z * cos_yaw;
            view_x = temp_x;
            view_z = temp_z;

            // Apply pitch rotation (around X axis)
            double cos_pitch = cos(-field->camera.pitch);
            double sin_pitch = sin(-field->camera.pitch);
            temp_z = view_z * cos_pitch - view_y * sin_pitch;
            double temp_y = view_z * sin_pitch + view_y * cos_pitch;
            view_y = temp_y;
            view_z = temp_z;

            // Apply roll rotation (around Z axis)
            double cos_roll = cos(-field->camera.roll);
            double sin_roll = sin(-field->camera.roll);
            temp_x = view_x * cos_roll - view_y * sin_roll;
            temp_y = view_x * sin_roll + view_y * cos_roll;
            view_x = temp_x;
            view_y = temp_y;
        }

        // Skip stars that are behind the viewer
        if (view_z <= 0.1) {
            continue;
        }

        // Project 3D position to 2D screen using perspective projection
        // Formula: screen_pos = (pos / z) * zoom
        double scale = field->zoom / view_z;
        int screen_x = center_x + (int)(view_x * scale);
        int screen_y = center_y + (int)(view_y * scale);

        // Check if star is within screen bounds
        if (screen_x >= 0 && screen_x < fb->width &&
            screen_y >= 0 && screen_y < fb->height) {

            int index = screen_y * fb->width + screen_x;

            // Set character based on distance/brightness
            fb->buffer[index] = star->character;

            // Set color based on brightness
            if (star->brightness > 200) {
                fb->colors[index] = 4; // Yellow - very bright
            } else if (star->brightness > 150) {
                fb->colors[index] = 1; // White - bright
            } else if (star->brightness > 100) {
                fb->colors[index] = 2; // Cyan - medium
            } else {
                fb->colors[index] = 3; // Blue - dim
            }
        }
    }
}

void framebuffer_display(const FrameBuffer *fb) {
    if (!fb) {
        return;
    }

    // Move cursor to home position
    move(0, 0);

    // Render each character with its color
    for (int y = 0; y < fb->height; y++) {
        for (int x = 0; x < fb->width; x++) {
            int index = y * fb->width + x;
            char ch = fb->buffer[index];
            int color = fb->colors[index];

            if (color > 0 && has_colors()) {
                attron(COLOR_PAIR(color));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(color));
            } else {
                mvaddch(y, x, ch);
            }
        }
    }

    refresh();
}

void framebuffer_destroy(FrameBuffer *fb) {
    if (fb) {
        free(fb->buffer);
        free(fb->colors);
        free(fb);
    }
}
