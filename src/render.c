#include "render.h"
#include "effects.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

        // Apply camera rotation
        double view_x = star->x;
        double view_y = star->y;
        double view_z = star->z;

        // Always apply camera transformations
        if (true) {
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

void render_ship_3d(FrameBuffer *fb, const Ship3D *ship, const Camera *camera, double zoom) {
    if (!fb || !ship || !camera || !ship->active) {
        return;
    }

    // Transform ship position to camera space
    double rel_x = ship->x - camera->pos_x;
    double rel_y = ship->y - camera->pos_y;
    double rel_z = ship->z - camera->pos_z;

    // Apply camera yaw rotation (around Y axis)
    double cos_yaw = cos(-camera->yaw);
    double sin_yaw = sin(-camera->yaw);
    double view_x = rel_x * cos_yaw - rel_z * sin_yaw;
    double view_z = rel_x * sin_yaw + rel_z * cos_yaw;

    // Apply pitch rotation (around X axis)
    double cos_pitch = cos(-camera->pitch);
    double sin_pitch = sin(-camera->pitch);
    double temp_z = view_z * cos_pitch - rel_y * sin_pitch;
    double view_y = view_z * sin_pitch + rel_y * cos_pitch;
    view_z = temp_z;

    // Apply roll rotation (around Z axis)
    double cos_roll = cos(-camera->roll);
    double sin_roll = sin(-camera->roll);
    double temp_x = view_x * cos_roll - view_y * sin_roll;
    view_y = view_x * sin_roll + view_y * cos_roll;
    view_x = temp_x;

    // Skip if ship is behind camera
    if (view_z <= 0.5) {
        return;
    }

    // Project to 2D screen coordinates
    int center_x = fb->width / 2;
    int center_y = fb->height / 2;

    double scale = zoom / view_z;
    int screen_x = center_x + (int)(view_x * scale);
    int screen_y = center_y + (int)(view_y * scale);

    // Check screen bounds
    if (screen_x < 0 || screen_x >= fb->width ||
        screen_y < 0 || screen_y >= fb->height) {
        return;
    }

    // Choose ship sprite based on distance
    char sprite;
    int color;

    double distance = sqrt(view_x*view_x + view_y*view_y + view_z*view_z);

    if (distance < 20.0) {
        // Very close - large ship sprite
        sprite = '#';
        color = 1;  // White - bright

        // Draw a larger ship (3x3)
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int px = screen_x + dx;
                int py = screen_y + dy;
                if (px >= 0 && px < fb->width && py >= 0 && py < fb->height) {
                    int index = py * fb->width + px;
                    fb->buffer[index] = (dx == 0 && dy == 0) ? '@' : '#';
                    fb->colors[index] = color;
                }
            }
        }
    } else if (distance < 50.0) {
        // Medium distance - single character
        sprite = 'X';
        color = 2;  // Cyan
        int index = screen_y * fb->width + screen_x;
        fb->buffer[index] = sprite;
        fb->colors[index] = color;
    } else if (distance < 100.0) {
        // Far - small dot
        sprite = 'o';
        color = 3;  // Blue
        int index = screen_y * fb->width + screen_x;
        fb->buffer[index] = sprite;
        fb->colors[index] = color;
    } else {
        // Very far - tiny dot
        sprite = '.';
        color = 3;  // Dim
        int index = screen_y * fb->width + screen_x;
        fb->buffer[index] = sprite;
        fb->colors[index] = color;
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

void render_missile(FrameBuffer *fb, const Missile *missile, const Camera *camera, double zoom) {
    if (!fb || !missile || !camera || !missile->active) {
        return;
    }

    // Transform missile position to camera space
    double rel_x = missile->x - camera->pos_x;
    double rel_y = missile->y - camera->pos_y;
    double rel_z = missile->z - camera->pos_z;

    // Apply camera yaw rotation (around Y axis)
    double cos_yaw = cos(-camera->yaw);
    double sin_yaw = sin(-camera->yaw);
    double view_x = rel_x * cos_yaw - rel_z * sin_yaw;
    double view_z = rel_x * sin_yaw + rel_z * cos_yaw;

    // Apply pitch rotation (around X axis)
    double cos_pitch = cos(-camera->pitch);
    double sin_pitch = sin(-camera->pitch);
    double temp_z = view_z * cos_pitch - rel_y * sin_pitch;
    double view_y = view_z * sin_pitch + rel_y * cos_pitch;
    view_z = temp_z;

    // Apply roll rotation (around Z axis)
    double cos_roll = cos(-camera->roll);
    double sin_roll = sin(-camera->roll);
    double temp_x = view_x * cos_roll - view_y * sin_roll;
    view_y = view_x * sin_roll + view_y * cos_roll;
    view_x = temp_x;

    // Skip if behind camera
    if (view_z <= 0.1) {
        return;
    }

    // Project to 2D screen coordinates
    int center_x = fb->width / 2;
    int center_y = fb->height / 2;

    double scale = zoom / view_z;
    int screen_x = center_x + (int)(view_x * scale);
    int screen_y = center_y + (int)(view_y * scale);

    // Check screen bounds
    if (screen_x < 0 || screen_x >= fb->width ||
        screen_y < 0 || screen_y >= fb->height) {
        return;
    }

    // Render missile with trail
    int index = screen_y * fb->width + screen_x;
    fb->buffer[index] = missile->trail_char;
    fb->colors[index] = missile->color;

    // Add a short trail behind it
    int trail_x = screen_x - 1;
    if (trail_x >= 0) {
        index = screen_y * fb->width + trail_x;
        fb->buffer[index] = '-';
        fb->colors[index] = missile->color;
    }
}

void render_explosion(FrameBuffer *fb, const Explosion *explosion, const Camera *camera, double zoom) {
    if (!fb || !explosion || !camera || !explosion->active) {
        return;
    }

    // Transform explosion position to camera space
    double rel_x = explosion->x - camera->pos_x;
    double rel_y = explosion->y - camera->pos_y;
    double rel_z = explosion->z - camera->pos_z;

    // Apply camera yaw rotation (around Y axis)
    double cos_yaw = cos(-camera->yaw);
    double sin_yaw = sin(-camera->yaw);
    double view_x = rel_x * cos_yaw - rel_z * sin_yaw;
    double view_z = rel_x * sin_yaw + rel_z * cos_yaw;

    // Apply pitch rotation (around X axis)
    double cos_pitch = cos(-camera->pitch);
    double sin_pitch = sin(-camera->pitch);
    double temp_z = view_z * cos_pitch - rel_y * sin_pitch;
    double view_y = view_z * sin_pitch + rel_y * cos_pitch;
    view_z = temp_z;

    // Apply roll rotation (around Z axis)
    double cos_roll = cos(-camera->roll);
    double sin_roll = sin(-camera->roll);
    double temp_x = view_x * cos_roll - view_y * sin_roll;
    view_y = view_x * sin_roll + view_y * cos_roll;
    view_x = temp_x;

    // Skip if behind camera
    if (view_z <= 0.1) {
        return;
    }

    // Project center to 2D screen coordinates
    int center_x = fb->width / 2;
    int center_y = fb->height / 2;

    double scale = zoom / view_z;
    int screen_x = center_x + (int)(view_x * scale);
    int screen_y = center_y + (int)(view_y * scale);

    // Draw expanding circle
    int screen_radius = (int)(explosion->radius * scale);
    if (screen_radius < 1) screen_radius = 1;

    // Render explosion particles in a circle pattern
    char explosion_chars[] = {'*', 'o', '.', '+', 'x'};
    int num_particles = 16;  // Points around circle

    for (int i = 0; i < num_particles; i++) {
        double angle = (i * 2.0 * M_PI) / num_particles;
        int px = screen_x + (int)(cos(angle) * screen_radius);
        int py = screen_y + (int)(sin(angle) * screen_radius * 0.5);  // Half height for terminal aspect

        if (px >= 0 && px < fb->width && py >= 0 && py < fb->height) {
            int index = py * fb->width + px;
            // Choose character based on explosion age
            double progress = explosion->age / explosion->max_age;
            int char_index = (int)(progress * 4.999);
            fb->buffer[index] = explosion_chars[char_index];
            fb->colors[index] = explosion->color;
        }
    }

    // Add center flash for young explosions
    if (explosion->age < explosion->max_age * 0.3) {
        if (screen_x >= 0 && screen_x < fb->width &&
            screen_y >= 0 && screen_y < fb->height) {
            int index = screen_y * fb->width + screen_x;
            fb->buffer[index] = '@';
            fb->colors[index] = explosion->color;
        }
    }
}

void render_weapons(FrameBuffer *fb, const WeaponsSystem *weapons, const Camera *camera, double zoom) {
    if (!fb || !weapons || !camera) {
        return;
    }

    // Render all active missiles
    for (int i = 0; i < MAX_MISSILES; i++) {
        if (weapons->missiles[i].active) {
            render_missile(fb, &weapons->missiles[i], camera, zoom);
        }
    }

    // Render all active explosions
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (weapons->explosions[i].active) {
            render_explosion(fb, &weapons->explosions[i], camera, zoom);
        }
    }
}

void render_text(FrameBuffer *fb, int x, int y, const char *text, int color) {
    if (!fb || !text) {
        return;
    }

    size_t len = strlen(text);

    for (size_t i = 0; i < len; i++) {
        int px = x + (int)i;
        if (px >= 0 && px < fb->width && y >= 0 && y < fb->height) {
            int index = y * fb->width + px;
            fb->buffer[index] = text[i];
            fb->colors[index] = color;
        }
    }
}

void render_horizon(FrameBuffer *fb) {
    if (!fb) {
        return;
    }

    // Draw horizon line at 2/3 down the screen
    int horizon_y = (fb->height * 2) / 3;

    // Draw the horizon line
    for (int x = 0; x < fb->width; x++) {
        if (horizon_y >= 0 && horizon_y < fb->height) {
            int index = horizon_y * fb->width + x;
            fb->buffer[index] = '=';
            fb->colors[index] = COLOR_PAIR(6);  // Cyan
        }
    }

    // Add some ground shading below horizon
    // Note: x += 3 creates a sparse dot pattern for visual texture without filling entire screen
    for (int y = horizon_y + 1; y < fb->height; y++) {
        for (int x = 0; x < fb->width; x += 3) {
            int index = y * fb->width + x;
            fb->buffer[index] = '.';
            fb->colors[index] = COLOR_PAIR(2);  // Cyan ground texture
        }
    }
}

void render_target_circle(FrameBuffer *fb) {
    if (!fb) {
        return;
    }

    int center_x = fb->width / 2;
    int center_y = fb->height / 2;
    int radius = 3;  // Small targeting reticle

    // Draw circle using basic circle algorithm
    for (int i = 0; i < 32; i++) {
        double angle = (i * 2.0 * M_PI) / 32.0;
        int x = center_x + (int)(cos(angle) * radius);
        int y = center_y + (int)(sin(angle) * radius * 0.5);  // Half height for terminal aspect

        if (x >= 0 && x < fb->width && y >= 0 && y < fb->height) {
            int index = y * fb->width + x;
            fb->buffer[index] = 'o';
            fb->colors[index] = COLOR_PAIR(3);  // Yellow
        }
    }

    // Draw crosshairs
    for (int i = -4; i <= 4; i++) {
        // Horizontal line
        int hx = center_x + i;
        if (hx >= 0 && hx < fb->width && center_y >= 0 && center_y < fb->height) {
            int index = center_y * fb->width + hx;
            fb->buffer[index] = '-';
            fb->colors[index] = COLOR_PAIR(3);
        }

        // Vertical line
        int vy = center_y + i;
        if (center_x >= 0 && center_x < fb->width && vy >= 0 && vy < fb->height) {
            int index = vy * fb->width + center_x;
            fb->buffer[index] = '|';
            fb->colors[index] = COLOR_PAIR(3);
        }
    }

    // Center dot
    if (center_x >= 0 && center_x < fb->width && center_y >= 0 && center_y < fb->height) {
        int index = center_y * fb->width + center_x;
        fb->buffer[index] = '+';
        fb->colors[index] = COLOR_PAIR(1);  // Bright white
    }
}

void render_clay_pigeon(FrameBuffer *fb, const ClayPigeon *pigeon, const Camera *camera, double zoom) {
    if (!fb || !pigeon || !camera || !pigeon->active) {
        return;
    }

    // Transform pigeon position to camera space
    double rel_x = pigeon->x - camera->pos_x;
    double rel_y = pigeon->y - camera->pos_y;
    double rel_z = pigeon->z - camera->pos_z;

    // Apply camera yaw rotation (around Y axis)
    double cos_yaw = cos(-camera->yaw);
    double sin_yaw = sin(-camera->yaw);
    double view_x = rel_x * cos_yaw - rel_z * sin_yaw;
    double view_z = rel_x * sin_yaw + rel_z * cos_yaw;

    // Apply pitch rotation (around X axis)
    double cos_pitch = cos(-camera->pitch);
    double sin_pitch = sin(-camera->pitch);
    double temp_z = view_z * cos_pitch - rel_y * sin_pitch;
    double view_y = view_z * sin_pitch + rel_y * cos_pitch;
    view_z = temp_z;

    // Apply roll rotation (around Z axis)
    double cos_roll = cos(-camera->roll);
    double sin_roll = sin(-camera->roll);
    double temp_x = view_x * cos_roll - view_y * sin_roll;
    view_y = view_x * sin_roll + view_y * cos_roll;
    view_x = temp_x;

    // Skip if behind camera
    if (view_z <= 0.1) {
        return;
    }

    // Project to 2D screen coordinates
    int center_x = fb->width / 2;
    int center_y = fb->height / 2;

    double scale = zoom / view_z;
    int screen_x = center_x + (int)(view_x * scale);
    int screen_y = center_y + (int)(view_y * scale);

    // Check screen bounds
    if (screen_x < 0 || screen_x >= fb->width ||
        screen_y < 0 || screen_y >= fb->height) {
        return;
    }

    // Render clay pigeon
    int index = screen_y * fb->width + screen_x;
    fb->buffer[index] = pigeon->hit ? 'X' : pigeon->character;
    fb->colors[index] = pigeon->color;

    // If it's been hit, show debris
    if (pigeon->hit) {
        // Draw debris around the impact point
        static const int debris_offsets[][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        for (int i = 0; i < 8; i++) {
            int dx = debris_offsets[i][0];
            int dy = debris_offsets[i][1];
            int px = screen_x + dx;
            int py = screen_y + dy;
            if (px >= 0 && px < fb->width && py >= 0 && py < fb->height) {
                int idx = py * fb->width + px;
                fb->buffer[idx] = '*';
                fb->colors[idx] = COLOR_PAIR(1);  // White debris
            }
        }
    }
}

void framebuffer_destroy(FrameBuffer *fb) {
    if (fb) {
        free(fb->buffer);
        free(fb->colors);
        free(fb);
    }
}
