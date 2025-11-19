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
    int menu_y = fb->height / 2 - 6;

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
        "4. Training Simulator",
        "5. Skeet Shooting"
    };

    for (int i = 0; i < 5; i++) {
        int color = (i == selection) ? COLOR_PAIR(2) : COLOR_PAIR(7);
        char buf[100];
        snprintf(buf, sizeof(buf), "%s %s",
                 (i == selection) ? ">" : " ",
                 options[i]);
        render_text(fb, menu_x, menu_y + i, buf, color);
    }

    menu_y += 7;
    int help_x = (menu_x >= 5) ? menu_x - 5 : menu_x;
    render_text(fb, help_x, menu_y, "Arrow keys to select, Enter to start", COLOR_PAIR(6));
}

bool modes_handle_menu_input(int key, int *selection, GameMode *mode) {
    switch (key) {
        case KEY_UP:
        case 'w':
        case 'W':
            (*selection)--;
            if (*selection < 0) *selection = 4;
            return false;

        case KEY_DOWN:
        case 's':
        case 'S':
            (*selection)++;
            if (*selection > 4) *selection = 0;
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
                case 4: *mode = MODE_SKEET; break;
            }
            return true; // Mode selected

        case '1': *mode = MODE_SINGLE_PLAYER; return true;
        case '2': *mode = MODE_DUAL_PLAYER; return true;
        case '3': *mode = MODE_COOP; return true;
        case '4': *mode = MODE_TRAINING; return true;
        case '5': *mode = MODE_SKEET; return true;
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

        case MODE_SKEET:
            player1->active = true;
            // Preserve joystick assignment if already set
            if (player1->control_mode != CONTROL_JOYSTICK) {
                player1->control_mode = CONTROL_KEYBOARD;
            }
            player2->active = false; // No second player in skeet mode
            player2->control_mode = CONTROL_INACTIVE;
            break;
    }
}

// ============================================================================
// SKEET MODE IMPLEMENTATION
// ============================================================================

void modes_init_skeet(SkeetSession *skeet) {
    if (!skeet) return;

    // Clear all clay pigeons
    for (int i = 0; i < MAX_CLAY_PIGEONS; i++) {
        skeet->pigeons[i].active = false;
        skeet->pigeons[i].hit = false;
    }

    skeet->score = 0;
    skeet->pigeons_hit = 0;
    skeet->pigeons_launched = 0;
    skeet->pigeons_missed = 0;
    skeet->session_time = 0.0;
    skeet->accuracy = 0.0;
    skeet->next_launch_time = SKEET_INITIAL_LAUNCH_DELAY;
    skeet->difficulty = 0;  // Start at easy difficulty
}

void modes_launch_clay_pigeon(SkeetSession *skeet, LauncherPosition launcher) {
    if (!skeet) return;

    // Find an inactive pigeon slot
    int slot = -1;
    for (int i = 0; i < MAX_CLAY_PIGEONS; i++) {
        if (!skeet->pigeons[i].active) {
            slot = i;
            break;
        }
    }

    if (slot == -1) return;  // No available slots

    ClayPigeon *pigeon = &skeet->pigeons[slot];

    // Set launch position and velocity based on launcher
    switch (launcher) {
        case LAUNCHER_BOTTOM_LEFT:
            pigeon->x = -200.0;
            pigeon->y = -100.0;
            pigeon->z = 300.0;
            // Arc toward right and up
            pigeon->velocity_x = 80.0 + (rand() % 40);
            pigeon->velocity_y = 120.0 + (rand() % 40);
            pigeon->velocity_z = 50.0 + (rand() % 30);
            break;

        case LAUNCHER_BOTTOM_RIGHT:
            pigeon->x = 200.0;
            pigeon->y = -100.0;
            pigeon->z = 300.0;
            // Arc toward left and up
            pigeon->velocity_x = -80.0 - (rand() % 40);
            pigeon->velocity_y = 120.0 + (rand() % 40);
            pigeon->velocity_z = 50.0 + (rand() % 30);
            break;

        case LAUNCHER_BOTTOM_CENTER:
            pigeon->x = 0.0;
            pigeon->y = -120.0;
            pigeon->z = 200.0;
            // Arc straight up and forward
            pigeon->velocity_x = (rand() % 40) - 20;
            pigeon->velocity_y = 140.0 + (rand() % 40);
            pigeon->velocity_z = 60.0 + (rand() % 30);
            break;
    }

    pigeon->age = 0.0;
    pigeon->lifetime = CLAY_PIGEON_LIFETIME;
    pigeon->points = 10 + (skeet->difficulty * 5);
    pigeon->active = true;
    pigeon->hit = false;
    pigeon->character = 'O';
    pigeon->color = COLOR_PAIR(4);  // Yellow

    skeet->pigeons_launched++;
}

void modes_update_skeet(SkeetSession *skeet, WeaponsSystem *weapons, double dt) {
    if (!skeet) return;

    skeet->session_time += dt;

    // Update launch timer
    skeet->next_launch_time -= dt;
    if (skeet->next_launch_time <= 0.0) {
        // Launch a new pigeon from random launcher
        LauncherPosition launchers[] = {LAUNCHER_BOTTOM_LEFT, LAUNCHER_BOTTOM_RIGHT, LAUNCHER_BOTTOM_CENTER};
        int launcher_idx = rand() % 3;
        modes_launch_clay_pigeon(skeet, launchers[launcher_idx]);

        // Set next launch time based on difficulty
        double base_interval = 2.5 - (skeet->difficulty * 0.3);
        if (base_interval < 0.8) base_interval = 0.8;
        skeet->next_launch_time = base_interval + ((rand() % 100) / 100.0);
    }

    // Update clay pigeons
    for (int i = 0; i < MAX_CLAY_PIGEONS; i++) {
        ClayPigeon *pigeon = &skeet->pigeons[i];
        if (!pigeon->active) continue;

        pigeon->age += dt;

        // Apply gravity to velocity (simulate arc trajectory)
        pigeon->velocity_y -= CLAY_PIGEON_GRAVITY * dt;

        // Update position
        pigeon->x += pigeon->velocity_x * dt;
        pigeon->y += pigeon->velocity_y * dt;
        pigeon->z += pigeon->velocity_z * dt;

        // Check if pigeon has expired
        if (pigeon->age > pigeon->lifetime) {
            if (!pigeon->hit) {
                skeet->pigeons_missed++;
            }
            pigeon->active = false;
            continue;
        }

        // If already hit, just let it fall
        if (pigeon->hit) {
            continue;
        }

        // Check missile collisions
        for (int m = 0; m < MAX_MISSILES; m++) {
            Missile *missile = &weapons->missiles[m];
            if (!missile->active) continue;

            // Calculate distance between missile and pigeon
            double dx = missile->x - pigeon->x;
            double dy = missile->y - pigeon->y;
            double dz = missile->z - pigeon->z;
            double dist = sqrt(dx*dx + dy*dy + dz*dz);

            // Check collision (generous hit radius)
            if (dist < CLAY_PIGEON_HIT_RADIUS) {
                pigeon->hit = true;
                pigeon->lifetime = pigeon->age + CLAY_PIGEON_DEBRIS_TIME;
                missile->active = false;
                skeet->score += pigeon->points;
                skeet->pigeons_hit++;
                break;
            }
        }
    }

    // Update accuracy
    if (skeet->pigeons_launched > 0) {
        skeet->accuracy = (skeet->pigeons_hit * 100.0) / skeet->pigeons_launched;
    }

    // Increase difficulty over time
    int new_difficulty = (int)(skeet->session_time / SKEET_DIFFICULTY_INTERVAL);
    if (new_difficulty > 3) new_difficulty = 3;
    skeet->difficulty = new_difficulty;
}

void modes_render_skeet_hud(FrameBuffer *fb, SkeetSession *skeet) {
    if (!fb || !skeet) return;

    int y = 1;
    char buf[100];

    // Draw skeet HUD in top-right corner
    int x = fb->width - 35;
    if (x < 0) x = 0;

    snprintf(buf, sizeof(buf), "=== SKEET SHOOTING ===");
    render_text(fb, x, y++, buf, COLOR_PAIR(3));

    y++;
    snprintf(buf, sizeof(buf), "Score:      %d", skeet->score);
    render_text(fb, x, y++, buf, COLOR_PAIR(7));

    snprintf(buf, sizeof(buf), "Hit:        %d", skeet->pigeons_hit);
    render_text(fb, x, y++, buf, COLOR_PAIR(2));

    snprintf(buf, sizeof(buf), "Missed:     %d", skeet->pigeons_missed);
    render_text(fb, x, y++, buf, COLOR_PAIR(1));

    snprintf(buf, sizeof(buf), "Accuracy:   %.1f%%", skeet->accuracy);
    render_text(fb, x, y++, buf, COLOR_PAIR(skeet->accuracy > 50 ? 2 : 1));

    snprintf(buf, sizeof(buf), "Time:       %.1fs", skeet->session_time);
    render_text(fb, x, y++, buf, COLOR_PAIR(7));

    const char *diff_names[] = {"Easy", "Medium", "Hard", "Expert"};
    snprintf(buf, sizeof(buf), "Difficulty: %s", diff_names[skeet->difficulty]);
    render_text(fb, x, y++, buf, COLOR_PAIR(3 + skeet->difficulty));

    // Count active pigeons
    int active_count = 0;
    for (int i = 0; i < MAX_CLAY_PIGEONS; i++) {
        if (skeet->pigeons[i].active) active_count++;
    }

    y++;
    snprintf(buf, sizeof(buf), "In Air:     %d", active_count);
    render_text(fb, x, y++, buf, COLOR_PAIR(active_count > 5 ? 1 : 7));

    // Instructions
    y++;
    render_text(fb, x, y++, "Controls:", COLOR_PAIR(6));
    render_text(fb, x, y++, "F: Fire", COLOR_PAIR(7));
    render_text(fb, x, y++, "M: Menu", COLOR_PAIR(7));
}

void modes_render_clay_pigeons(FrameBuffer *fb, SkeetSession *skeet, const Camera *camera, double zoom) {
    if (!fb || !skeet || !camera) return;

    // Render all active clay pigeons
    for (int i = 0; i < MAX_CLAY_PIGEONS; i++) {
        if (skeet->pigeons[i].active) {
            render_clay_pigeon(fb, &skeet->pigeons[i], camera, zoom);
        }
    }
}
