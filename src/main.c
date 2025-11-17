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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEFAULT_STAR_COUNT 500
#define TARGET_FPS 60
#define FRAME_TIME_US (1000000 / TARGET_FPS)

static void draw_hud(const Ship3D *player1, const Ship3D *player2, int width, int height, bool ai_mode);
static void draw_crosshair(int width, int height);

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

    while (running) {
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

        if (key == 'q' || key == 'Q' || key == 27) {  // ESC
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

        // Render
        framebuffer_clear(fb);
        render_starfield(fb, field);

        // Render player 2 ship in 3D space
        render_ship_3d(fb, &player2, &field->camera, field->zoom);

        framebuffer_display(fb);

        // Draw cockpit HUD
        draw_crosshair(width, height);
        draw_hud(&player1, &player2, width, height, ai_mode);

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

static void draw_hud(const Ship3D *player1, const Ship3D *player2, int width, int height, bool ai_mode);
static void draw_hud_impl(const Ship3D *player1, const Ship3D *player2, int width, int height, bool ai_mode);

static void draw_hud(const Ship3D *player1, const Ship3D *player2, int width, int height, bool ai_mode) {
    draw_hud_impl(player1, player2, width, height, ai_mode);
}

static void draw_hud_impl(const Ship3D *player1, const Ship3D *player2, int width, int height, bool ai_mode) {
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

    // Bottom: Controls
    attron(A_BOLD);
    mvprintw(height - 1, 2, "P1:WASD+Q/E+Space | P2:Arrows+</>+Enter | V:View B:AI ESC:Quit");
    attroff(A_BOLD);
}
