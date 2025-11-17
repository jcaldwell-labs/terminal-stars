#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include "starfield.h"
#include "render.h"
#include "terminal.h"
#include "input.h"
#include "effects.h"

#define DEFAULT_STAR_COUNT 200
#define TARGET_FPS 60
#define FRAME_TIME_US (1000000 / TARGET_FPS)

static void process_input(InputAction action, Starfield *field, bool *running);
static void draw_hud(const Starfield *field);

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialize subsystems
    if (!terminal_init()) {
        fprintf(stderr, "Failed to initialize terminal\n");
        return EXIT_FAILURE;
    }

    if (!input_init()) {
        fprintf(stderr, "Failed to initialize input\n");
        terminal_cleanup();
        return EXIT_FAILURE;
    }

    // Create starfield and frame buffer
    Starfield *field = starfield_create(DEFAULT_STAR_COUNT);
    if (!field) {
        fprintf(stderr, "Failed to create starfield\n");
        input_cleanup();
        terminal_cleanup();
        return EXIT_FAILURE;
    }

    int width, height;
    terminal_get_size(&width, &height);
    FrameBuffer *fb = framebuffer_create(width, height);
    if (!fb) {
        fprintf(stderr, "Failed to create frame buffer\n");
        starfield_destroy(field);
        input_cleanup();
        terminal_cleanup();
        return EXIT_FAILURE;
    }

    // Main render loop
    bool running = true;
    struct timespec last_time, current_time;
    clock_gettime(CLOCK_MONOTONIC, &last_time);

    while (running) {
        // Calculate delta time
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        double delta_time = (current_time.tv_sec - last_time.tv_sec) +
                           (current_time.tv_nsec - last_time.tv_nsec) / 1e9;
        last_time = current_time;

        // Handle terminal resize
        terminal_handle_resize();

        // Handle input
        InputAction action = input_poll();
        process_input(action, field, &running);

        // Update starfield
        starfield_update(field, delta_time);

        // Render
        framebuffer_clear(fb);
        render_starfield(fb, field);
        framebuffer_display(fb);

        // Draw HUD overlay
        draw_hud(field);

        // Frame timing
        usleep(FRAME_TIME_US);
    }

    // Cleanup
    framebuffer_destroy(fb);
    starfield_destroy(field);
    input_cleanup();
    terminal_cleanup();

    return EXIT_SUCCESS;
}

static void process_input(InputAction action, Starfield *field, bool *running) {
    if (!field || !running) {
        return;
    }

    switch (action) {
        case INPUT_QUIT:
            *running = false;
            break;

        case INPUT_ZOOM_IN:
            field->zoom *= 1.1;
            if (field->zoom > 200.0) {
                field->zoom = 200.0;
            }
            break;

        case INPUT_ZOOM_OUT:
            field->zoom *= 0.9;
            if (field->zoom < 10.0) {
                field->zoom = 10.0;
            }
            break;

        case INPUT_SPEED_UP:
            field->speed *= 1.2;
            if (field->speed > 5.0) {
                field->speed = 5.0;
            }
            break;

        case INPUT_SPEED_DOWN:
            field->speed *= 0.8;
            if (field->speed < 0.1) {
                field->speed = 0.1;
            }
            break;

        case INPUT_CYCLE_EFFECT:
            field->effect_mode = (field->effect_mode + 1) % EFFECT_COUNT;
            break;

        case INPUT_PAN_UP:
        case INPUT_PAN_DOWN:
        case INPUT_PAN_LEFT:
        case INPUT_PAN_RIGHT:
        case INPUT_TOGGLE_ROTATION:
            // These could be implemented for additional control
            break;

        case INPUT_NONE:
            break;
    }
}

static void draw_hud(const Starfield *field) {
    if (!field) {
        return;
    }

    int width, height;
    terminal_get_size(&width, &height);

    // Draw status line at bottom
    attron(A_BOLD);
    mvprintw(height - 2, 2, "Effect: %s", effect_get_name(field->effect_mode));
    mvprintw(height - 2, 25, "Speed: %.1fx", field->speed);
    mvprintw(height - 2, 40, "Zoom: %.0f", field->zoom);
    attroff(A_BOLD);

    // Draw help at bottom
    mvprintw(height - 1, 2, "Q:Quit  SPACE:Effect  +/-:Zoom  []:Speed");

    refresh();
}
