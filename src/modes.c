#include "modes.h"
#include "render.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

void modes_init_game_state(GameState *state) {
    state->mode = MODE_SINGLE_PLAYER;
    state->paused = false;
    state->show_menu = true;  // Start with menu
    state->menu_selection = 0;
    state->training = NULL;
}

void modes_init_training(TrainingSession *training) {
    if (!training) return;

    // Clear all targets
    for (int i = 0; i < MAX_TRAINING_TARGETS; i++) {
        training->targets[i].active = false;
    }

    training->score = 0;
    training->targets_hit = 0;
    training->shots_fired = 0;
    training->session_time = 0.0;
    training->accuracy = 0.0;

    // Spawn initial targets
    modes_spawn_training_targets(training, PATTERN_STATIC_GRID);
}

void modes_spawn_training_targets(TrainingSession *training, int pattern) {
    if (!training) return;

    // Clear existing targets
    for (int i = 0; i < MAX_TRAINING_TARGETS; i++) {
        training->targets[i].active = false;
    }

    switch (pattern) {
        case PATTERN_STATIC_GRID: {
            // Create a 4x5 grid of static targets
            int idx = 0;
            for (int x = -2; x <= 1 && idx < MAX_TRAINING_TARGETS; x++) {
                for (int y = -2; y <= 2 && idx < MAX_TRAINING_TARGETS; y++) {
                    TrainingTarget *t = &training->targets[idx];
                    t->x = x * 150.0;
                    t->y = y * 150.0;
                    t->z = 800.0;
                    t->velocity_x = 0.0;
                    t->velocity_y = 0.0;
                    t->velocity_z = 0.0;
                    t->radius = 20.0;
                    t->points = 10;
                    t->active = true;
                    t->is_static = true;
                    t->health = 1;
                    t->character = 'O';
                    t->color = COLOR_PAIR(3); // Yellow
                    idx++;
                }
            }
            break;
        }

        case PATTERN_MOVING_LINE: {
            // Create a line of targets moving horizontally
            for (int i = 0; i < 10 && i < MAX_TRAINING_TARGETS; i++) {
                TrainingTarget *t = &training->targets[i];
                t->x = -400.0 + i * 100.0;
                t->y = 0.0;
                t->z = 600.0;
                t->velocity_x = 50.0;
                t->velocity_y = 0.0;
                t->velocity_z = 0.0;
                t->radius = 25.0;
                t->points = 20;
                t->active = true;
                t->is_static = false;
                t->health = 1;
                t->character = '@';
                t->color = COLOR_PAIR(5); // Cyan
            }
            break;
        }

        case PATTERN_ORBITING: {
            // Create targets in orbital paths
            const int num_orbital_targets = 8;
            for (int i = 0; i < num_orbital_targets && i < MAX_TRAINING_TARGETS; i++) {
                TrainingTarget *t = &training->targets[i];
                // Evenly distribute targets around a circle (angle in radians)
                // Note: 2.0 ensures floating-point division for precise angle calculation
                double angle = 2.0 * M_PI * i / (double)num_orbital_targets;
                double radius = 300.0;
                t->x = cos(angle) * radius;
                t->y = sin(angle) * radius;
                t->z = 700.0;
                // Orbital velocity (tangent to circle)
                t->velocity_x = -sin(angle) * 40.0;
                t->velocity_y = cos(angle) * 40.0;
                t->velocity_z = 0.0;
                t->radius = 30.0;
                t->points = 30;
                t->active = true;
                t->is_static = false;
                t->health = 2;
                t->character = '#';
                t->color = COLOR_PAIR(4); // Magenta
            }
            break;
        }

        case PATTERN_RANDOM: {
            // Random positions
            for (int i = 0; i < 15 && i < MAX_TRAINING_TARGETS; i++) {
                TrainingTarget *t = &training->targets[i];
                t->x = (rand() % 800) - 400.0;
                t->y = (rand() % 800) - 400.0;
                t->z = 400.0 + (rand() % 600);
                t->velocity_x = (rand() % 60) - 30.0;
                t->velocity_y = (rand() % 60) - 30.0;
                t->velocity_z = 0.0;
                t->radius = 20.0 + (rand() % 20);
                t->points = 15;
                t->active = true;
                t->is_static = (rand() % 2) == 0;
                t->health = 1 + (rand() % 2);
                t->character = '*';
                t->color = COLOR_PAIR(2 + (rand() % 4));
            }
            break;
        }
    }
}

void modes_update_training(TrainingSession *training, WeaponsSystem *weapons, double dt) {
    if (!training) return;

    training->session_time += dt;

    // Update moving targets
    for (int i = 0; i < MAX_TRAINING_TARGETS; i++) {
        TrainingTarget *t = &training->targets[i];
        if (!t->active || t->is_static) continue;

        t->x += t->velocity_x * dt;
        t->y += t->velocity_y * dt;
        t->z += t->velocity_z * dt;

        // Wrap around boundaries
        if (fabs(t->x) > 1000.0) t->velocity_x *= -1.0;
        if (fabs(t->y) > 1000.0) t->velocity_y *= -1.0;
    }

    // Check missile collisions with targets
    for (int m = 0; m < MAX_MISSILES; m++) {
        Missile *missile = &weapons->missiles[m];
        if (!missile->active) continue;

        for (int t = 0; t < MAX_TRAINING_TARGETS; t++) {
            TrainingTarget *target = &training->targets[t];
            if (!target->active) continue;

            // Calculate distance between missile and target
            double dx = missile->x - target->x;
            double dy = missile->y - target->y;
            double dz = missile->z - target->z;
            double dist = sqrt(dx*dx + dy*dy + dz*dz);

            // Check collision
            if (dist < target->radius) {
                target->health--;
                missile->active = false; // Destroy missile

                if (target->health <= 0) {
                    training->score += target->points;
                    training->targets_hit++;
                    target->active = false; // Destroy target
                }
                break;
            }
        }
    }

    // Update accuracy
    if (training->shots_fired > 0) {
        training->accuracy = (training->targets_hit * 100.0) / training->shots_fired;
    }
}

void modes_render_training_hud(FrameBuffer *fb, TrainingSession *training) {
    if (!fb || !training) return;

    int y = 1;
    char buf[100];

    // Draw training HUD in top-right corner (with bounds checking)
    int x = fb->width - 35;
    if (x < 0) x = 0;

    snprintf(buf, sizeof(buf), "=== TRAINING MODE ===");
    render_text(fb, x, y++, buf, COLOR_PAIR(3));

    y++;
    snprintf(buf, sizeof(buf), "Score:      %d", training->score);
    render_text(fb, x, y++, buf, COLOR_PAIR(7));

    snprintf(buf, sizeof(buf), "Targets:    %d", training->targets_hit);
    render_text(fb, x, y++, buf, COLOR_PAIR(7));

    snprintf(buf, sizeof(buf), "Shots:      %d", training->shots_fired);
    render_text(fb, x, y++, buf, COLOR_PAIR(7));

    snprintf(buf, sizeof(buf), "Accuracy:   %.1f%%", training->accuracy);
    render_text(fb, x, y++, buf, COLOR_PAIR(training->accuracy > 50 ? 2 : 1));

    snprintf(buf, sizeof(buf), "Time:       %.1fs", training->session_time);
    render_text(fb, x, y++, buf, COLOR_PAIR(7));

    // Count active targets
    int active_count = 0;
    for (int i = 0; i < MAX_TRAINING_TARGETS; i++) {
        if (training->targets[i].active) active_count++;
    }

    y++;
    snprintf(buf, sizeof(buf), "Remaining:  %d", active_count);
    render_text(fb, x, y++, buf, COLOR_PAIR(active_count > 0 ? 3 : 1));

    // Instructions
    y++;
    render_text(fb, x, y++, "Keys:", COLOR_PAIR(6));
    render_text(fb, x, y++, "1-4: Spawn patterns", COLOR_PAIR(7));
    render_text(fb, x, y++, "R:   Reset targets", COLOR_PAIR(7));
}

void modes_render_menu(FrameBuffer *fb, int selection) {
    if (!fb) return;

    // Calculate menu position with bounds checking for small terminals
    int menu_x = fb->width / 2 - 20;
    int menu_y = fb->height / 2 - 5;

    // Ensure menu doesn't render at negative coordinates
    if (menu_x < 5) menu_x = 5;
    if (menu_y < 2) menu_y = 2;

    const char *title = "=== TERMINAL STARS ===";
    int title_x = (menu_x >= 2) ? menu_x - 2 : menu_x;
    render_text(fb, title_x, menu_y, title, COLOR_PAIR(3));

    menu_y += 2;
    const char *options[] = {
        "1. Single Player (vs AI)",
        "2. Dual Player (Competitive)",
        "3. Co-op Mode (Team up)",
        "4. Training Simulator"
    };

    for (int i = 0; i < 4; i++) {
        int color = (i == selection) ? COLOR_PAIR(2) : COLOR_PAIR(7);
        char buf[100];
        snprintf(buf, sizeof(buf), "%s %s",
                 (i == selection) ? ">" : " ",
                 options[i]);
        render_text(fb, menu_x, menu_y + i, buf, color);
    }

    menu_y += 6;
    int help_x = (menu_x >= 5) ? menu_x - 5 : menu_x;
    render_text(fb, help_x, menu_y, "Arrow keys to select, Enter to start", COLOR_PAIR(6));
}

bool modes_handle_menu_input(int key, int *selection, GameMode *mode) {
    switch (key) {
        case KEY_UP:
        case 'w':
        case 'W':
            (*selection)--;
            if (*selection < 0) *selection = 3;
            return false;

        case KEY_DOWN:
        case 's':
        case 'S':
            (*selection)++;
            if (*selection > 3) *selection = 0;
            return false;

        case '\n':
        case KEY_ENTER:
        case ' ':
            // Map selection to game mode
            switch (*selection) {
                case 0: *mode = MODE_SINGLE_PLAYER; break;
                case 1: *mode = MODE_DUAL_PLAYER; break;
                case 2: *mode = MODE_COOP; break;
                case 3: *mode = MODE_TRAINING; break;
            }
            return true; // Mode selected

        case '1': *mode = MODE_SINGLE_PLAYER; return true;
        case '2': *mode = MODE_DUAL_PLAYER; return true;
        case '3': *mode = MODE_COOP; return true;
        case '4': *mode = MODE_TRAINING; return true;
    }

    return false;
}

void modes_setup_players(GameState *state, Ship3D *player1, Ship3D *player2) {
    if (!state || !player1 || !player2) return;

    switch (state->mode) {
        case MODE_SINGLE_PLAYER:
            player1->active = true;
            // Preserve joystick assignment if already set
            if (player1->control_mode != CONTROL_JOYSTICK) {
                player1->control_mode = CONTROL_KEYBOARD;
            }
            player2->active = true;
            player2->control_mode = CONTROL_AI;
            player2->ai_behavior = AI_HUNTING;
            break;

        case MODE_DUAL_PLAYER:
            player1->active = true;
            // Preserve joystick assignment if already set
            if (player1->control_mode != CONTROL_JOYSTICK) {
                player1->control_mode = CONTROL_KEYBOARD;
            }
            player2->active = true;
            // Preserve joystick assignment if already set
            if (player2->control_mode != CONTROL_JOYSTICK) {
                player2->control_mode = CONTROL_KEYBOARD;
            }
            player2->ai_behavior = AI_ORBITAL; // Not used in manual mode
            break;

        case MODE_COOP:
            player1->active = true;
            // Preserve joystick assignment if already set
            if (player1->control_mode != CONTROL_JOYSTICK) {
                player1->control_mode = CONTROL_KEYBOARD;
            }
            player2->active = true;
            // Preserve joystick assignment if already set
            if (player2->control_mode != CONTROL_JOYSTICK) {
                player2->control_mode = CONTROL_KEYBOARD;
            }
            // Both fight against AI enemies (future enhancement)
            break;

        case MODE_TRAINING:
            player1->active = true;
            // Preserve joystick assignment if already set
            if (player1->control_mode != CONTROL_JOYSTICK) {
                player1->control_mode = CONTROL_KEYBOARD;
            }
            player2->active = false; // No second player in training
            player2->control_mode = CONTROL_INACTIVE;
            break;
    }
}
