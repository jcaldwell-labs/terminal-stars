#include "render.h"
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

        // Skip stars that are behind the viewer
        if (star->z <= 0.1) {
            continue;
        }

        // Project 3D position to 2D screen using perspective projection
        // Formula: screen_pos = (pos / z) * zoom
        double scale = field->zoom / star->z;
        int screen_x = center_x + (int)(star->x * scale);
        int screen_y = center_y + (int)(star->y * scale);

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
