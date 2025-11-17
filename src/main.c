#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>
#include "starfield.h"
#include "render.h"
#include "terminal.h"
#include "ship.h"
#include "effects.h"
#include "weapons.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEFAULT_STAR_COUNT 500
#define TARGET_FPS 60
#define FRAME_TIME_US (1000000 / TARGET_FPS)

static void draw_hud(const Ship3D *player1, const Ship3D *player2, const WeaponsSystem *weapons, int width, int height, bool ai_mode, int frame_count);
static void draw_crosshair(int width, int height);
static void calculate_target_point(const Ship3D *ship, double *target_x, double *target_y, double *target_z);
static void draw_unit_circle_hud(const Ship3D *ship, int width, int height);
static void draw_enemy_direction_indicator(const Ship3D *player, const Ship3D *enemy, const Camera *camera, int width, int height, int frame_count);
static void draw_radar_overlay(const Ship3D *player, const Ship3D *enemy, const WeaponsSystem *weapons, int width, int height);

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Seed random
    srand(time(NULL));

    // Initialize subsystems
    if (!terminal_init()) {
        fprintf(stderr, "Failed to initialize terminal\n");
        return EXIT_FAILURE;
    }

    // Create starfield
    Starfield *field = starfield_create(DEFAULT_STAR_COUNT);
    if (!field) {
        fprintf(stderr, "Failed to create starfield\n");
        terminal_cleanup();
        return EXIT_FAILURE;
    }

    // Set to LINEAR effect for classic space flight feel
    field->effect_mode = EFFECT_LINEAR;
    field->speed = 1.0;
    field->zoom = 50.0;

    // Create player 1 ship at origin
    Ship3D player1;
    ship_init(&player1, 0, 0.0, 0.0, 0.0);

    // Create player 2 ship (manual control)
    Ship3D player2;
    ship_init(&player2, 1, 50.0, 30.0, 0.0);

    // AI mode toggle
    bool ai_mode = false;  // Start with manual control

    // Create weapons system
    WeaponsSystem weapons;
    weapons_init(&weapons);

    // Create frame buffer
    int width, height;
    terminal_get_size(&width, &height);
    FrameBuffer *fb = framebuffer_create(width, height);
    if (!fb) {
        fprintf(stderr, "Failed to create frame buffer\n");
        starfield_destroy(field);
        terminal_cleanup();
        return EXIT_FAILURE;
    }

    // Main flight loop
    bool running = true;
    struct timespec last_time, current_time;
    clock_gettime(CLOCK_MONOTONIC, &last_time);

    double orbit_angle = 0.0;
    int frame_count = 0;  // For blinking effects

    while (running) {
        frame_count++;
        if (frame_count > 1000) frame_count = 0;  // Prevent overflow
        // Calculate delta time
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        double delta_time = (current_time.tv_sec - last_time.tv_sec) +
                           (current_time.tv_nsec - last_time.tv_nsec) / 1e9;
        last_time = current_time;

        // Handle terminal resize
        terminal_handle_resize();
        terminal_get_size(&width, &height);

        // Handle input (non-blocking)
        timeout(0);
        int key = getch();

        if (key == 'Q' || key == 27) {  // Uppercase Q or ESC to quit
            running = false;
        } else if (key != ERR) {
            // Flight controls for player 1
            switch (key) {
                case 'w':
                case 'W':
                    ship_pitch_down(&player1, delta_time);  // Nose down
                    break;
                case 's':
                case 'S':
                    ship_pitch_up(&player1, delta_time);    // Nose up
                    break;
                case 'a':
                case 'A':
                    ship_yaw_left(&player1, delta_time);    // Turn left
                    break;
                case 'd':
                case 'D':
                    ship_yaw_right(&player1, delta_time);   // Turn right
                    break;
                case 'q':
                    ship_roll_left(&player1, delta_time);   // Bank left
                    break;
                case 'e':
                case 'E':
                    ship_roll_right(&player1, delta_time);  // Bank right
                    break;
                case ' ':
                    ship_thrust(&player1, 1.0);             // Full thrust
                    break;
                case 'v':
                case 'V':
                    ship_toggle_view(&player1);             // Toggle camera view
                    break;
                case 'f':
                case 'F':
                    // Fire missiles!
                    if (weapons_can_fire(&weapons, player1.player_id)) {
                        double target_x, target_y, target_z;
                        calculate_target_point(&player1, &target_x, &target_y, &target_z);
                        weapons_fire(&weapons, &player1, target_x, target_y, target_z);
                    }
                    break;
                case '[':
                    field->speed *= 0.8;  // Slow down starfield
                    if (field->speed < 0.1) field->speed = 0.1;
                    break;
                case ']':
                    field->speed *= 1.2;  // Speed up starfield
                    if (field->speed > 5.0) field->speed = 5.0;
                    break;
                case '\t':  // Tab
                    // Cycle through effects
                    field->effect_mode = (field->effect_mode + 1) % EFFECT_COUNT;
                    break;
                case 'b':
                case 'B':
                    // Toggle AI mode for player 2
                    ai_mode = !ai_mode;
                    break;

                // Player 2 controls (Arrow keys, I/K/J/L, and Enter)
                case KEY_UP:
                    ship_pitch_down(&player2, delta_time);  // Nose down
                    break;
                case KEY_DOWN:
                    ship_pitch_up(&player2, delta_time);    // Nose up
                    break;
                case KEY_LEFT:
                    ship_yaw_left(&player2, delta_time);    // Turn left
                    break;
                case KEY_RIGHT:
                    ship_yaw_right(&player2, delta_time);   // Turn right
                    break;
                case ',':
                case '<':
                    ship_roll_left(&player2, delta_time);   // Bank left
                    break;
                case '.':
                case '>':
                    ship_roll_right(&player2, delta_time);  // Bank right
                    break;
                case '\n':
                case '\r':
                case KEY_ENTER:
                    ship_thrust(&player2, 1.0);             // Full thrust
                    break;
            }
        }

        // Update player 1 physics (user controlled)
        ship_update(&player1, delta_time);

        // Update player 2
        if (ai_mode) {
            // AI mode - simple orbital motion
            double orbit_radius = 50.0;
            double orbit_speed = 0.5;  // radians per second
            orbit_angle += orbit_speed * delta_time;

            // Circular orbit in XY plane, bobbing in Z
            player2.x = cos(orbit_angle) * orbit_radius;
            player2.y = sin(orbit_angle) * orbit_radius;
            player2.z = sin(orbit_angle * 0.5) * 20.0;  // Bob up and down

            // Face direction of motion
            player2.yaw = orbit_angle + M_PI / 2.0;
            player2.pitch = sin(orbit_angle * 0.5) * 0.3;  // Slight pitch variation
        } else {
            // Manual control - update physics normally
            ship_update(&player2, delta_time);
        }

        // Update camera to follow player 1
        ship_update_camera(&player1, &field->camera);

        // Update starfield (apply effects)
        starfield_update(field, delta_time);

        // Update weapons system (missiles and explosions)
        weapons_update(&weapons, delta_time);

        // Render
        framebuffer_clear(fb);
        render_starfield(fb, field);

        // Render player 2 ship in 3D space
        render_ship_3d(fb, &player2, &field->camera, field->zoom);

        // Render weapons (missiles and explosions)
        render_weapons(fb, &weapons, &field->camera, field->zoom);

        framebuffer_display(fb);

        // Draw cockpit HUD
        draw_crosshair(width, height);
        draw_hud(&player1, &player2, &weapons, width, height, ai_mode, frame_count);

        // Draw advanced HUD elements
        draw_unit_circle_hud(&player1, width, height);
        draw_enemy_direction_indicator(&player1, &player2, &field->camera, width, height, frame_count);
        draw_radar_overlay(&player1, &player2, &weapons, width, height);

        refresh();

        // Frame timing
        usleep(FRAME_TIME_US);
    }

    // Cleanup
    framebuffer_destroy(fb);
    starfield_destroy(field);
    terminal_cleanup();

    return EXIT_SUCCESS;
}

static void draw_crosshair(int width, int height) {
    int center_x = width / 2;
    int center_y = height / 2;

    // Simple crosshair
    attron(COLOR_PAIR(4) | A_BOLD);  // Yellow
    mvaddch(center_y, center_x, '+');
    mvaddch(center_y, center_x - 1, '-');
    mvaddch(center_y, center_x + 1, '-');
    mvaddch(center_y - 1, center_x, '|');
    mvaddch(center_y + 1, center_x, '|');
    attroff(COLOR_PAIR(4) | A_BOLD);
}

static void draw_hud(const Ship3D *player1, const Ship3D *player2, const WeaponsSystem *weapons, int width, int height, bool ai_mode, int frame_count);
static void draw_hud_impl(const Ship3D *player1, const Ship3D *player2, const WeaponsSystem *weapons, int width, int height, bool ai_mode, int frame_count);

static void draw_hud(const Ship3D *player1, const Ship3D *player2, const WeaponsSystem *weapons, int width, int height, bool ai_mode, int frame_count) {
    draw_hud_impl(player1, player2, weapons, width, height, ai_mode, frame_count);
}

static void draw_hud_impl(const Ship3D *player1, const Ship3D *player2, const WeaponsSystem *weapons, int width, int height, bool ai_mode, int frame_count) {
    (void)frame_count;  // Will use later for blinking effects
    if (!player1) {
        return;
    }

    double speed = ship_get_speed(player1);

    // Top left: Speed and health
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(0, 2, "SPEED: %.0f m/s", speed);
    attroff(COLOR_PAIR(1) | A_BOLD);

    mvprintw(1, 2, "HEALTH: ");
    for (int i = 0; i < player1->health; i++) {
        addch('*');
    }

    // Top right: View mode
    const char *view_name = (player1->view_mode == VIEW_COCKPIT) ? "COCKPIT" : "CHASE";
    attron(COLOR_PAIR(2));
    mvprintw(0, width - 15, "VIEW: %s", view_name);
    attroff(COLOR_PAIR(2));

    // Enemy distance indicator
    if (player2) {
        double dx = player2->x - player1->x;
        double dy = player2->y - player1->y;
        double dz = player2->z - player1->z;
        double distance = sqrt(dx*dx + dy*dy + dz*dz);

        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(2, 2, "ENEMY: %.0fm", distance);
        attroff(COLOR_PAIR(2) | A_BOLD);
    }

    // AI mode indicator
    attron(COLOR_PAIR(3));
    mvprintw(2, width - 20, "P2: %s", ai_mode ? "AI" : "MANUAL");
    attroff(COLOR_PAIR(3));

    // Position (debug info)
    mvprintw(3, 2, "POS: (%.0f, %.0f, %.0f)", player1->x, player1->y, player1->z);

    // Weapons info - top left below health
    if (weapons) {
        int missiles = weapons_get_missiles_remaining(weapons, player1->player_id);
        attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(4, 2, "MISSILES: %d", missiles);
        attroff(COLOR_PAIR(4) | A_BOLD);

        // Fire cooldown indicator
        if (weapons->fire_cooldown[player1->player_id] > 0.0) {
            attron(COLOR_PAIR(3));
            mvprintw(4, 18, "[RELOADING]");
            attroff(COLOR_PAIR(3));
        } else if (missiles >= 2) {
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw(4, 18, "[READY]");
            attroff(COLOR_PAIR(1) | A_BOLD);
        }
    }

    // Targeting reticle label
    attron(COLOR_PAIR(4));
    mvprintw(height / 2 - 2, width / 2 - 4, "TARGET");
    attroff(COLOR_PAIR(4));

    // Bottom: Controls
    attron(A_BOLD);
    mvprintw(height - 1, 2, "P1:WASD+q/e+Space+F | P2:Arrows+</>+Enter | V:View B:AI SHIFT+Q/ESC:Quit");
    attroff(A_BOLD);
}

// Calculate target point in 3D space from ship's perspective
// This represents where the center crosshair is pointing in 3D space
static void calculate_target_point(const Ship3D *ship, double *target_x, double *target_y, double *target_z) {
    if (!ship || !target_x || !target_y || !target_z) {
        return;
    }

    // Get ship's forward vector
    double forward_x, forward_y, forward_z;
    ship_get_forward_vector(ship, &forward_x, &forward_y, &forward_z);

    // Target is at a fixed distance in front of ship
    double target_distance = 100.0;  // Distance to target point

    *target_x = ship->x + forward_x * target_distance;
    *target_y = ship->y + forward_y * target_distance;
    *target_z = ship->z + forward_z * target_distance;
}

// Draw unit circle HUD showing 6 degrees of freedom
// Positioned about 1/3 from center on the right side
static void draw_unit_circle_hud(const Ship3D *ship, int width, int height) {
    if (!ship) {
        return;
    }

    // Position: right side, about 1/3 from center
    int circle_center_x = width * 2 / 3;
    int circle_center_y = height / 2;
    int circle_radius = 8;  // Radius in character cells

    // Draw circle using ASCII art
    attron(COLOR_PAIR(2));  // Cyan
    for (int angle = 0; angle < 360; angle += 10) {
        double rad = angle * M_PI / 180.0;
        int x = circle_center_x + (int)(cos(rad) * circle_radius);
        int y = circle_center_y + (int)(sin(rad) * circle_radius * 0.5);  // Half height for terminal aspect
        if (x >= 0 && x < width && y >= 0 && y < height) {
            mvaddch(y, x, '.');
        }
    }
    attroff(COLOR_PAIR(2));

    // Draw center crosshair
    mvaddch(circle_center_y, circle_center_x, '+');

    // Convert angles to degrees for display
    double pitch_deg = ship->pitch * 180.0 / M_PI;
    double yaw_deg = ship->yaw * 180.0 / M_PI;
    double roll_deg = ship->roll * 180.0 / M_PI;

    // Draw 6 DOF information around the circle
    attron(COLOR_PAIR(1) | A_BOLD);  // White bold

    // Top: Pitch
    mvprintw(circle_center_y - circle_radius - 2, circle_center_x - 6, "P:%.1f", pitch_deg);

    // Right: Yaw
    mvprintw(circle_center_y, circle_center_x + circle_radius + 2, "Y:%.1f", yaw_deg);

    // Bottom: Roll
    mvprintw(circle_center_y + circle_radius / 2 + 2, circle_center_x - 6, "R:%.1f", roll_deg);

    attroff(COLOR_PAIR(1) | A_BOLD);

    // Velocity indicators (left side)
    attron(COLOR_PAIR(2));  // Cyan
    mvprintw(circle_center_y - 2, circle_center_x - circle_radius - 10, "VX:%.1f", ship->velocity_x);
    mvprintw(circle_center_y - 1, circle_center_x - circle_radius - 10, "VY:%.1f", ship->velocity_y);
    mvprintw(circle_center_y, circle_center_x - circle_radius - 10, "VZ:%.1f", ship->velocity_z);
    attroff(COLOR_PAIR(2));

    // Draw attitude indicator (pitch/roll visualization)
    // Draw horizon line based on pitch
    double horizon_offset = sin(ship->pitch) * 3.0;
    int horizon_y = circle_center_y + (int)horizon_offset;

    attron(COLOR_PAIR(4));  // Yellow
    for (int dx = -circle_radius; dx <= circle_radius; dx++) {
        int x = circle_center_x + dx;
        if (x >= 0 && x < width && horizon_y >= 0 && horizon_y < height) {
            // Check if within circle
            double dist = sqrt(dx * dx * 4.0 + horizon_offset * horizon_offset);
            if (dist <= circle_radius) {
                mvaddch(horizon_y, x, '-');
            }
        }
    }
    attroff(COLOR_PAIR(4));

    // Draw title
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(circle_center_y - circle_radius - 3, circle_center_x - 3, "6-DOF");
    attroff(COLOR_PAIR(1) | A_BOLD);
}

// Draw enemy direction indicator (blinking red X)
static void draw_enemy_direction_indicator(const Ship3D *player, const Ship3D *enemy,
                                           const Camera *camera, int width, int height,
                                           int frame_count) {
    if (!player || !enemy || !camera || !enemy->active) {
        return;
    }

    // Blink every 15 frames (at 60fps = 4 times per second)
    bool show = (frame_count % 15) < 10;
    if (!show) {
        return;
    }

    // Calculate enemy direction vector in world space
    double dx = enemy->x - player->x;
    double dy = enemy->y - player->y;
    double dz = enemy->z - player->z;

    // Transform to camera space (same as render_ship_3d)
    // Apply camera yaw rotation
    double cos_yaw = cos(-camera->yaw);
    double sin_yaw = sin(-camera->yaw);
    double view_x = dx * cos_yaw - dz * sin_yaw;
    double view_z = dx * sin_yaw + dz * cos_yaw;

    // Apply pitch rotation
    double cos_pitch = cos(-camera->pitch);
    double sin_pitch = sin(-camera->pitch);
    double temp_z = view_z * cos_pitch - dy * sin_pitch;
    double view_y = view_z * sin_pitch + dy * cos_pitch;
    view_z = temp_z;

    // Apply roll rotation
    double cos_roll = cos(-camera->roll);
    double sin_roll = sin(-camera->roll);
    double temp_x = view_x * cos_roll - view_y * sin_roll;
    view_y = view_x * sin_roll + view_y * cos_roll;
    view_x = temp_x;

    // Normalize to edge of screen
    double distance = sqrt(view_x * view_x + view_y * view_y + view_z * view_z);
    if (distance < 0.1) {
        return;  // Enemy too close
    }

    // Project direction onto screen edges
    int center_x = width / 2;
    int center_y = height / 2;

    // Calculate screen position (clamped to edges)
    double scale = 50.0;  // Arbitrary scale for edge indicators
    int screen_x = center_x + (int)(view_x * scale / (fabs(view_z) + 1.0));
    int screen_y = center_y + (int)(view_y * scale / (fabs(view_z) + 1.0));

    // Clamp to screen edges with margin
    int margin = 3;
    if (screen_x < margin) screen_x = margin;
    if (screen_x >= width - margin) screen_x = width - margin - 1;
    if (screen_y < margin) screen_y = margin;
    if (screen_y >= height - margin) screen_y = height - margin - 1;

    // Draw red X indicator
    attron(COLOR_PAIR(3) | A_BOLD);  // Use color pair 3 for enemy (we'll need to check if this is red)
    mvaddch(screen_y, screen_x, 'X');
    mvaddch(screen_y - 1, screen_x - 1, '\\');
    mvaddch(screen_y - 1, screen_x + 1, '/');
    mvaddch(screen_y + 1, screen_x - 1, '/');
    mvaddch(screen_y + 1, screen_x + 1, '\\');
    attroff(COLOR_PAIR(3) | A_BOLD);

    // Add "ENEMY" label if enemy is behind
    if (view_z < 0) {
        attron(COLOR_PAIR(3));
        mvprintw(screen_y - 2, screen_x - 3, "BEHIND");
        attroff(COLOR_PAIR(3));
    }
}

// Draw radar overlay for incoming detection
static void draw_radar_overlay(const Ship3D *player, const Ship3D *enemy,
                               const WeaponsSystem *weapons, int width, int height) {
    if (!player || !weapons) {
        return;
    }

    // Radar position: bottom right corner
    int radar_x = width - 22;
    int radar_y = height - 12;
    int radar_size = 9;  // 9x9 grid

    // Draw radar border
    attron(COLOR_PAIR(2));  // Cyan
    mvprintw(radar_y - 1, radar_x, "+---RADAR---+");
    for (int i = 0; i < radar_size; i++) {
        mvaddch(radar_y + i, radar_x, '|');
        mvaddch(radar_y + i, radar_x + 13, '|');
    }
    mvprintw(radar_y + radar_size, radar_x, "+----------+");
    attroff(COLOR_PAIR(2));

    // Draw radar grid
    int radar_center_x = radar_x + 7;
    int radar_center_y = radar_y + radar_size / 2;

    attron(COLOR_PAIR(2));
    // Crosshairs
    for (int i = 0; i < radar_size; i++) {
        mvaddch(radar_y + i, radar_center_x, '|');
        mvaddch(radar_center_y, radar_x + 2 + i, '-');
    }
    mvaddch(radar_center_y, radar_center_x, '+');
    attroff(COLOR_PAIR(2));

    // Player is always at center
    attron(COLOR_PAIR(1) | A_BOLD);
    mvaddch(radar_center_y, radar_center_x, '@');
    attroff(COLOR_PAIR(1) | A_BOLD);

    // Draw enemy on radar if present
    if (enemy && enemy->active) {
        double dx = enemy->x - player->x;
        double dy = enemy->y - player->y;
        double dz = enemy->z - player->z;

        // Scale to radar
        double radar_scale = 0.08;
        int enemy_rx = radar_center_x + (int)(dx * radar_scale);
        int enemy_ry = radar_center_y + (int)(dy * radar_scale);

        // Clamp to radar bounds
        if (enemy_rx >= radar_x + 2 && enemy_rx < radar_x + 12 &&
            enemy_ry >= radar_y && enemy_ry < radar_y + radar_size) {

            // Color based on Z distance
            if (dz > 10) {
                attron(COLOR_PAIR(3));  // Far ahead
            } else if (dz < -10) {
                attron(COLOR_PAIR(4));  // Behind
            } else {
                attron(COLOR_PAIR(2));  // Same plane
            }
            mvaddch(enemy_ry, enemy_rx, 'E');
            attroff(COLOR_PAIR(3) | COLOR_PAIR(4) | COLOR_PAIR(2));
        }
    }

    // Draw incoming missiles on radar
    attron(COLOR_PAIR(3));  // Enemy missiles in red
    for (int i = 0; i < MAX_MISSILES; i++) {
        const Missile *m = &weapons->missiles[i];
        if (!m->active || m->owner_id == player->player_id) {
            continue;  // Only show enemy missiles
        }

        double dx = m->x - player->x;
        double dy = m->y - player->y;

        double radar_scale = 0.08;
        int m_rx = radar_center_x + (int)(dx * radar_scale);
        int m_ry = radar_center_y + (int)(dy * radar_scale);

        if (m_rx >= radar_x + 2 && m_rx < radar_x + 12 &&
            m_ry >= radar_y && m_ry < radar_y + radar_size) {
            mvaddch(m_ry, m_rx, '*');
        }
    }
    attroff(COLOR_PAIR(3));

    // Distance scale indicator
    attron(COLOR_PAIR(2));
    mvprintw(radar_y + radar_size + 1, radar_x + 2, "50u/div");
    attroff(COLOR_PAIR(2));
}
