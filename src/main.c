/*
 * Terminal Stars - Educational Starfield Visualizer
 *
 * A demonstration of 3D starfield effects rendered in the terminal.
 * This is a clean, educational implementation focusing on:
 *
 * - 3D perspective projection (converting 3D positions to 2D screen)
 * - Camera transformations (rotation using Euler angles)
 * - Double-buffered rendering (flicker-free terminal animation)
 * - Delta-time animation (frame-rate independent movement)
 * - Various animation techniques (translation, rotation, waves)
 *
 * For the complete terminal shader program with full flight simulation,
 * see: https://github.com/jcaldwell-labs/atari-style
 *
 * Usage:
 *   ./terminal-stars [options]
 *
 * Options:
 *   -s, --stars N     Number of stars (default: 500)
 *   -p, --speed F     Speed multiplier (default: 1.0)
 *   -e, --effect E    Effect: linear, spiral, warp, tunnel, explode, wave
 *   -z, --zoom F      Zoom/perspective level (default: 50.0)
 *   -h, --help        Show this help message
 *
 * Controls:
 *   Tab, 1-6   Select effect
 *   +/-        Adjust speed
 *   [/]        Adjust zoom
 *   Q, ESC     Quit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <ncurses.h>

#include "starfield.h"
#include "render.h"
#include "terminal.h"
#include "effects.h"

/* Configuration defaults */
#define DEFAULT_STAR_COUNT  500
#define DEFAULT_SPEED       1.0
#define DEFAULT_ZOOM        50.0
#define DEFAULT_EFFECT      EFFECT_LINEAR
#define TARGET_FPS          60
#define FRAME_TIME_US       (1000000 / TARGET_FPS)

/* Command-line option parsing */
static struct option long_options[] = {
    {"stars",  required_argument, 0, 's'},
    {"speed",  required_argument, 0, 'p'},
    {"effect", required_argument, 0, 'e'},
    {"zoom",   required_argument, 0, 'z'},
    {"help",   no_argument,       0, 'h'},
    {0, 0, 0, 0}
};

/*
 * parse_effect - Convert effect name to enum value
 *
 * Accepts both full names and numbers (1-6).
 * Returns -1 if the effect name is not recognized.
 */
static int parse_effect(const char *name) {
    if (!name) return -1;

    /* Check for numeric input first */
    if (name[0] >= '1' && name[0] <= '6' && name[1] == '\0') {
        return name[0] - '1';  /* Convert '1'-'6' to 0-5 */
    }

    /* Check effect names (case-insensitive first character) */
    switch (name[0]) {
        case 'l': case 'L': return EFFECT_LINEAR;
        case 's': case 'S': return EFFECT_SPIRAL;
        case 'w': case 'W':
            /* Distinguish 'warp' from 'wave' */
            if (name[1] == 'a' && name[2] == 'r') return EFFECT_WARP;
            if (name[1] == 'a' && name[2] == 'v') return EFFECT_WAVE;
            return -1;
        case 't': case 'T': return EFFECT_TUNNEL;
        case 'e': case 'E': return EFFECT_EXPLODE;
    }

    return -1;
}

/*
 * print_usage - Display help message
 */
static void print_usage(const char *program_name) {
    printf("Terminal Stars - Educational Starfield Visualizer\n\n");
    printf("Usage: %s [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -s, --stars N     Number of stars (default: %d)\n", DEFAULT_STAR_COUNT);
    printf("  -p, --speed F     Speed multiplier 0.1-5.0 (default: %.1f)\n", DEFAULT_SPEED);
    printf("  -e, --effect E    Effect name or number 1-6 (default: linear)\n");
    printf("  -z, --zoom F      Zoom level 10-200 (default: %.1f)\n", DEFAULT_ZOOM);
    printf("  -h, --help        Show this help message\n\n");
    printf("Effects:\n");
    printf("  1. linear  - Classic forward motion\n");
    printf("  2. spiral  - Rotating spiral pattern\n");
    printf("  3. warp    - Hyperspeed with streaking\n");
    printf("  4. tunnel  - Cylindrical tunnel\n");
    printf("  5. explode - Outward explosion\n");
    printf("  6. wave    - Sinusoidal motion\n\n");
    printf("Keyboard Controls:\n");
    printf("  Tab, 1-6   Select effect\n");
    printf("  +/-        Increase/decrease speed\n");
    printf("  [/]        Decrease/increase zoom\n");
    printf("  Q, ESC     Quit\n\n");
    printf("For the full terminal shader program, see:\n");
    printf("  https://github.com/jcaldwell-labs/atari-style\n");
}

/*
 * draw_hud - Render the heads-up display
 *
 * Shows current effect name, speed, zoom, and star count.
 * Also displays available controls at the bottom of the screen.
 */
static void draw_hud(const Starfield *field, int width, int height) {
    (void)width;  /* Reserved for future HUD elements */
    const char *effect_name = effect_get_name((EffectMode)field->effect_mode);
    char buf[80];

    /* Top-left: Effect name (bold, white) */
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(0, 2, "Effect: %s", effect_name);
    attroff(COLOR_PAIR(1) | A_BOLD);

    /* Second line: Parameters */
    snprintf(buf, sizeof(buf), "Speed: %.1f  Zoom: %.0f  Stars: %zu",
             field->speed, field->zoom, field->star_count);
    mvprintw(1, 2, "%s", buf);

    /* Bottom: Controls (dim) */
    attron(A_DIM);
    mvprintw(height - 1, 2,
             "Tab/1-6:Effect  +/-:Speed  [/]:Zoom  Q:Quit");
    attroff(A_DIM);
}

/*
 * clamp - Constrain a value to a range
 *
 * Returns min if value < min, max if value > max, else value.
 * This is a common utility for parameter validation.
 */
static double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/*
 * main - Program entry point
 *
 * Main loop structure:
 * 1. Parse command-line arguments
 * 2. Initialize terminal and create starfield
 * 3. Main loop:
 *    a. Calculate delta time
 *    b. Handle keyboard input
 *    c. Update starfield (apply effect)
 *    d. Render to frame buffer
 *    e. Display frame buffer
 *    f. Sleep to maintain target FPS
 * 4. Clean up and exit
 */
int main(int argc, char *argv[]) {
    /* Configuration with defaults */
    size_t star_count = DEFAULT_STAR_COUNT;
    double speed = DEFAULT_SPEED;
    double zoom = DEFAULT_ZOOM;
    int effect = DEFAULT_EFFECT;

    /* Parse command-line arguments */
    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "s:p:e:z:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 's':
                star_count = (size_t)atoi(optarg);
                if (star_count < 10) star_count = 10;
                if (star_count > 5000) star_count = 5000;
                break;
            case 'p':
                speed = atof(optarg);
                speed = clamp(speed, 0.1, 5.0);
                break;
            case 'e':
                effect = parse_effect(optarg);
                if (effect < 0) {
                    fprintf(stderr, "Unknown effect: %s\n", optarg);
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'z':
                zoom = atof(optarg);
                zoom = clamp(zoom, 10.0, 200.0);
                break;
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    /* Initialize terminal (ncurses) */
    if (!terminal_init()) {
        fprintf(stderr, "Failed to initialize terminal\n");
        return EXIT_FAILURE;
    }

    /* Get terminal dimensions */
    int width, height;
    terminal_get_size(&width, &height);

    /* Create starfield with specified parameters */
    Starfield *field = starfield_create(star_count);
    if (!field) {
        terminal_cleanup();
        fprintf(stderr, "Failed to create starfield\n");
        return EXIT_FAILURE;
    }

    /* Apply command-line parameters */
    field->speed = speed;
    field->zoom = zoom;
    field->effect_mode = effect;

    /* Create frame buffer for double-buffered rendering */
    FrameBuffer *fb = framebuffer_create(width, height);
    if (!fb) {
        starfield_destroy(field);
        terminal_cleanup();
        fprintf(stderr, "Failed to create frame buffer\n");
        return EXIT_FAILURE;
    }

    /*
     * Main loop
     *
     * We use a simple fixed-timestep loop with sleep for frame pacing.
     * For more accurate timing, consider using clock_gettime() with
     * CLOCK_MONOTONIC for delta calculation.
     */
    bool running = true;
    struct timespec last_time, current_time;
    clock_gettime(CLOCK_MONOTONIC, &last_time);

    while (running) {
        /* Calculate delta time (time since last frame) */
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        double delta_time = (current_time.tv_sec - last_time.tv_sec) +
                           (current_time.tv_nsec - last_time.tv_nsec) / 1e9;
        last_time = current_time;

        /* Handle terminal resize */
        terminal_handle_resize();
        terminal_get_size(&width, &height);

        /* Recreate frame buffer if size changed */
        if (width != fb->width || height != fb->height) {
            framebuffer_destroy(fb);
            fb = framebuffer_create(width, height);
            if (!fb) {
                running = false;
                continue;
            }
        }

        /*
         * Non-blocking keyboard input
         *
         * timeout(0) makes getch() return immediately with ERR if no key.
         * This allows the animation to continue smoothly.
         */
        timeout(0);
        int key = getch();

        if (key != ERR) {
            switch (key) {
                /* Quit */
                case 'q':
                case 'Q':
                case 27:  /* ESC */
                    running = false;
                    break;

                /* Cycle effects with Tab */
                case '\t':
                    field->effect_mode = (field->effect_mode + 1) % EFFECT_COUNT;
                    break;

                /* Direct effect selection with number keys */
                case '1': field->effect_mode = EFFECT_LINEAR; break;
                case '2': field->effect_mode = EFFECT_SPIRAL; break;
                case '3': field->effect_mode = EFFECT_WARP; break;
                case '4': field->effect_mode = EFFECT_TUNNEL; break;
                case '5': field->effect_mode = EFFECT_EXPLODE; break;
                case '6': field->effect_mode = EFFECT_WAVE; break;

                /* Speed control */
                case '+':
                case '=':  /* Unshifted + on most keyboards */
                    field->speed = clamp(field->speed + 0.1, 0.1, 5.0);
                    break;
                case '-':
                case '_':
                    field->speed = clamp(field->speed - 0.1, 0.1, 5.0);
                    break;

                /* Zoom control */
                case '[':
                    field->zoom = clamp(field->zoom - 5.0, 10.0, 200.0);
                    break;
                case ']':
                    field->zoom = clamp(field->zoom + 5.0, 10.0, 200.0);
                    break;
            }
        }

        /* Update starfield (apply current effect) */
        starfield_update(field, delta_time);

        /* Render to frame buffer */
        framebuffer_clear(fb);
        render_starfield(fb, field);
        framebuffer_display(fb);

        /* Draw HUD overlay (directly to ncurses, on top of frame buffer) */
        draw_hud(field, width, height);
        refresh();

        /* Sleep to maintain target frame rate */
        usleep(FRAME_TIME_US);
    }

    /* Clean up resources */
    framebuffer_destroy(fb);
    starfield_destroy(field);
    terminal_cleanup();

    return EXIT_SUCCESS;
}
