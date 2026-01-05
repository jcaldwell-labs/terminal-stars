/*
 * render.c - Frame Buffer Rendering System
 *
 * This module handles all terminal rendering using double-buffering:
 * 1. Create a frame buffer (character + color arrays)
 * 2. Clear the buffer each frame
 * 3. Render all stars to the buffer
 * 4. Display the complete buffer at once
 *
 * Key Concepts:
 * - Double buffering: Build frame off-screen, display all at once (no flicker)
 * - 3D to 2D projection: Convert 3D star positions to screen coordinates
 * - Camera transformation: Rotate the view using Euler angles (yaw, pitch, roll)
 * - Perspective division: Objects farther away appear smaller (z-based scaling)
 */

#include "render.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * framebuffer_create - Allocate a new frame buffer
 *
 * The frame buffer holds one complete frame of output.
 * It has two parallel arrays:
 * - buffer: Character to display at each position
 * - colors: Color pair index for each position
 *
 * Educational: This is a common pattern in graphics programming.
 * Instead of writing directly to the screen (which would flicker
 * as we update each character), we build the entire frame first,
 * then display it all at once.
 */
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

    // Allocate character buffer (one byte per cell)
    size_t buffer_size = (size_t)width * (size_t)height;
    fb->buffer = malloc(buffer_size);
    if (!fb->buffer) {
        free(fb);
        return NULL;
    }

    // Allocate color buffer (one int per cell for color pair index)
    fb->colors = malloc(buffer_size * sizeof(int));
    if (!fb->colors) {
        free(fb->buffer);
        free(fb);
        return NULL;
    }

    // Initialize to spaces with no color
    memset(fb->buffer, ' ', buffer_size);
    memset(fb->colors, 0, buffer_size * sizeof(int));

    return fb;
}

/*
 * framebuffer_clear - Reset buffer to empty state
 *
 * Called at the start of each frame to erase the previous frame.
 * We fill with spaces (not null bytes) so empty areas show correctly.
 */
void framebuffer_clear(FrameBuffer *fb) {
    if (!fb) {
        return;
    }

    size_t buffer_size = (size_t)fb->width * (size_t)fb->height;
    memset(fb->buffer, ' ', buffer_size);
    memset(fb->colors, 0, buffer_size * sizeof(int));
}

/*
 * render_starfield - Render all stars to the frame buffer
 *
 * This is the core rendering function. For each star:
 * 1. Transform position using camera orientation (rotation matrices)
 * 2. Skip if behind the viewer (z <= 0)
 * 3. Project 3D position to 2D using perspective division
 * 4. Write character and color to the buffer
 *
 * Educational: The 3D-to-2D projection formula is:
 *   screen_x = center_x + (view_x / view_z) * zoom
 *   screen_y = center_y + (view_y / view_z) * zoom
 *
 * The division by z creates the perspective effect: objects farther
 * away (larger z) appear smaller (closer to center).
 */
void render_starfield(FrameBuffer *fb, const Starfield *field) {
    if (!fb || !field || !field->stars) {
        return;
    }

    // Screen center - where z-axis intersects the screen
    int center_x = fb->width / 2;
    int center_y = fb->height / 2;

    for (size_t i = 0; i < field->star_count; i++) {
        const Star *star = &field->stars[i];

        // Start with star's world position
        double view_x = star->x;
        double view_y = star->y;
        double view_z = star->z;

        /*
         * Camera rotation using Euler angles (yaw, pitch, roll)
         *
         * Each rotation is a 2D rotation in a plane:
         * - Yaw (around Y axis): rotates X and Z
         * - Pitch (around X axis): rotates Y and Z
         * - Roll (around Z axis): rotates X and Y
         *
         * The negative angles are used because we're transforming the
         * world to camera space (inverse of camera orientation).
         */

        // Yaw rotation (turn left/right)
        double cos_yaw = cos(-field->camera.yaw);
        double sin_yaw = sin(-field->camera.yaw);
        double temp_x = view_x * cos_yaw - view_z * sin_yaw;
        double temp_z = view_x * sin_yaw + view_z * cos_yaw;
        view_x = temp_x;
        view_z = temp_z;

        // Pitch rotation (look up/down)
        double cos_pitch = cos(-field->camera.pitch);
        double sin_pitch = sin(-field->camera.pitch);
        temp_z = view_z * cos_pitch - view_y * sin_pitch;
        double temp_y = view_z * sin_pitch + view_y * cos_pitch;
        view_y = temp_y;
        view_z = temp_z;

        // Roll rotation (tilt head left/right)
        double cos_roll = cos(-field->camera.roll);
        double sin_roll = sin(-field->camera.roll);
        temp_x = view_x * cos_roll - view_y * sin_roll;
        temp_y = view_x * sin_roll + view_y * cos_roll;
        view_x = temp_x;
        view_y = temp_y;

        // Culling: skip stars behind the viewer (z must be positive)
        if (view_z <= 0.1) {
            continue;
        }

        /*
         * Perspective projection
         *
         * The key formula: screen_pos = (pos / z) * zoom + center
         *
         * - Dividing by z makes distant objects smaller
         * - Multiplying by zoom controls field of view
         * - Adding center puts the origin at screen center
         */
        double scale = field->zoom / view_z;
        int screen_x = center_x + (int)(view_x * scale);
        int screen_y = center_y + (int)(view_y * scale);

        // Bounds checking - skip if outside visible area
        if (screen_x >= 0 && screen_x < fb->width &&
            screen_y >= 0 && screen_y < fb->height) {

            // Calculate buffer index: row * width + column
            int index = screen_y * fb->width + screen_x;

            // Write character and color to buffer
            fb->buffer[index] = star->character;

            // Color based on brightness (brighter = warmer colors)
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

/*
 * framebuffer_display - Output the frame buffer to the terminal
 *
 * This function copies the complete buffer to the screen.
 * Using ncurses move/addch functions with color pairs.
 *
 * The refresh() call at the end tells ncurses to actually
 * update the terminal (it buffers changes internally too).
 */
void framebuffer_display(const FrameBuffer *fb) {
    if (!fb) {
        return;
    }

    // Start at top-left corner
    move(0, 0);

    // Render each character with its color
    for (int y = 0; y < fb->height; y++) {
        for (int x = 0; x < fb->width; x++) {
            int index = y * fb->width + x;
            char ch = fb->buffer[index];
            int color = fb->colors[index];

            // Apply color if available and terminal supports it
            if (color > 0 && has_colors()) {
                attron(COLOR_PAIR(color));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(color));
            } else {
                mvaddch(y, x, ch);
            }
        }
    }

    // Tell ncurses to update the actual terminal
    refresh();
}

/*
 * render_text - Draw text string to the frame buffer
 *
 * Utility function for rendering HUD text (effect name, controls, etc.)
 * Simply copies each character to the buffer at the specified position.
 */
void render_text(FrameBuffer *fb, int x, int y, const char *text, int color) {
    if (!fb || !text) {
        return;
    }

    size_t len = strlen(text);

    for (size_t i = 0; i < len; i++) {
        int px = x + (int)i;
        // Only write if within bounds
        if (px >= 0 && px < fb->width && y >= 0 && y < fb->height) {
            int index = y * fb->width + px;
            fb->buffer[index] = text[i];
            fb->colors[index] = color;
        }
    }
}

/*
 * framebuffer_destroy - Free all frame buffer memory
 *
 * Always free in reverse order of allocation to prevent leaks.
 */
void framebuffer_destroy(FrameBuffer *fb) {
    if (fb) {
        free(fb->buffer);
        free(fb->colors);
        free(fb);
    }
}
