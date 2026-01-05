/*
 * Performance Benchmark Suite for Terminal Stars
 *
 * Measures rendering performance for the educational starfield visualizer.
 * Tests: starfield creation, updates, frame buffer operations, and rendering.
 *
 * Run with: make benchmark
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "../include/starfield.h"
#include "../include/render.h"
#include "../include/types.h"

/*
 * get_time - Get current time with microsecond precision
 *
 * Returns the current time as a double (seconds.microseconds).
 * This is more precise than time() for benchmarking short operations.
 */
double get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

/*
 * benchmark_starfield_creation - Measure starfield allocation time
 *
 * Tests how long it takes to create and destroy starfields of various sizes.
 * Important for understanding startup overhead.
 */
void benchmark_starfield_creation(void) {
    printf("Benchmarking starfield creation...\n");

    int star_counts[] = {100, 500, 1000, 5000, 10000};
    int iterations = 100;

    for (int i = 0; i < 5; i++) {
        int count = star_counts[i];
        double start = get_time();

        for (int j = 0; j < iterations; j++) {
            Starfield *field = starfield_create(count);
            starfield_destroy(field);
        }

        double end = get_time();
        double avg_time = ((end - start) / iterations) * 1000.0;

        printf("  %5d stars: %7.3f ms/creation\n", count, avg_time);
    }
    printf("\n");
}

/*
 * benchmark_starfield_update - Measure effect update performance
 *
 * Tests how fast the starfield can be updated (applying effects).
 * This is called once per frame in the main loop.
 */
void benchmark_starfield_update(void) {
    printf("Benchmarking starfield updates...\n");

    int star_counts[] = {100, 500, 1000, 5000, 10000};
    int iterations = 1000;
    double delta_time = 0.016;  /* 60 FPS simulation */

    for (int i = 0; i < 5; i++) {
        int count = star_counts[i];
        Starfield *field = starfield_create(count);

        double start = get_time();

        for (int j = 0; j < iterations; j++) {
            starfield_update(field, delta_time);
        }

        double end = get_time();
        double avg_time = ((end - start) / iterations) * 1000.0;
        double fps = 1000.0 / avg_time;

        printf("  %5d stars: %7.3f ms/update (%6.1f fps theoretical)\n",
               count, avg_time, fps);

        starfield_destroy(field);
    }
    printf("\n");
}

/*
 * benchmark_framebuffer - Measure frame buffer clear performance
 *
 * Tests how fast we can clear the frame buffer.
 * This is called once per frame before rendering.
 */
void benchmark_framebuffer(void) {
    printf("Benchmarking frame buffer operations...\n");

    int sizes[][2] = {
        {80, 24},    /* Standard terminal */
        {120, 40},   /* Medium terminal */
        {160, 50},   /* Large terminal */
        {200, 60},   /* Very large terminal */
    };
    int iterations = 1000;

    for (int i = 0; i < 4; i++) {
        int w = sizes[i][0];
        int h = sizes[i][1];

        FrameBuffer *fb = framebuffer_create(w, h);

        double start = get_time();

        for (int j = 0; j < iterations; j++) {
            framebuffer_clear(fb);
        }

        double end = get_time();
        double avg_time = ((end - start) / iterations) * 1000.0;

        printf("  %dx%d: %7.3f ms/clear\n", w, h, avg_time);

        framebuffer_destroy(fb);
    }
    printf("\n");
}

/*
 * benchmark_render_starfield - Measure starfield rendering performance
 *
 * Tests the complete render pipeline: clear + project all stars to buffer.
 * This is the most expensive per-frame operation.
 */
void benchmark_render_starfield(void) {
    printf("Benchmarking starfield rendering...\n");

    int star_counts[] = {100, 500, 1000, 5000};
    int iterations = 100;
    FrameBuffer *fb = framebuffer_create(120, 40);

    for (int i = 0; i < 4; i++) {
        int count = star_counts[i];
        Starfield *field = starfield_create(count);

        double start = get_time();

        for (int j = 0; j < iterations; j++) {
            framebuffer_clear(fb);
            render_starfield(fb, field);
        }

        double end = get_time();
        double avg_time = ((end - start) / iterations) * 1000.0;
        double fps = 1000.0 / avg_time;

        printf("  %5d stars on 120x40: %7.3f ms/frame (%6.1f fps)\n",
               count, avg_time, fps);

        starfield_destroy(field);
    }

    framebuffer_destroy(fb);
    printf("\n");
}

/*
 * benchmark_text_rendering - Measure text overlay performance
 *
 * Tests HUD text rendering speed for various string lengths.
 */
void benchmark_text_rendering(void) {
    printf("Benchmarking text rendering...\n");

    FrameBuffer *fb = framebuffer_create(120, 40);
    int iterations = 10000;

    const char *test_strings[] = {
        "Short",
        "Medium length text",
        "This is a much longer string for testing rendering performance",
    };

    for (int i = 0; i < 3; i++) {
        const char *str = test_strings[i];

        double start = get_time();

        for (int j = 0; j < iterations; j++) {
            render_text(fb, 10, 10, str, 1);
        }

        double end = get_time();
        double avg_time = ((end - start) / iterations) * 1000.0;

        printf("  \"%s\": %7.3f ms/render\n", str, avg_time);
    }

    framebuffer_destroy(fb);
    printf("\n");
}

/*
 * benchmark_full_frame - Estimate maximum achievable FPS
 *
 * Simulates a complete frame cycle without terminal I/O.
 * Note: Actual FPS will be lower due to terminal display time.
 */
void benchmark_full_frame(void) {
    printf("Benchmarking full frame render cycle...\n");

    FrameBuffer *fb = framebuffer_create(120, 40);
    Starfield *field = starfield_create(2000);
    int iterations = 100;
    double delta_time = 0.016;

    double start = get_time();

    for (int i = 0; i < iterations; i++) {
        /* Full frame cycle (minus terminal display) */
        starfield_update(field, delta_time);
        framebuffer_clear(fb);
        render_starfield(fb, field);
    }

    double end = get_time();
    double total_time = end - start;
    double avg_frame_time = (total_time / iterations) * 1000.0;
    double theoretical_fps = 1000.0 / avg_frame_time;

    printf("  Frame update + clear + render:\n");
    printf("    Average time: %7.3f ms/frame\n", avg_frame_time);
    printf("    Theoretical FPS: %6.1f\n", theoretical_fps);
    printf("    (Note: Terminal I/O not included - actual FPS will be lower)\n");

    starfield_destroy(field);
    framebuffer_destroy(fb);
    printf("\n");
}

/*
 * main - Benchmark suite entry point
 *
 * Runs all benchmarks and reports system information.
 */
int main(void) {
    printf("\n");
    printf("========================================\n");
    printf("Terminal Stars - Performance Benchmarks\n");
    printf("========================================\n");
    printf("\n");

    /* Report system info */
    printf("System: ");
    #ifdef __linux__
    printf("Linux\n");
    #elif __APPLE__
    printf("macOS\n");
    #elif _WIN32
    printf("Windows\n");
    #else
    printf("Unknown\n");
    #endif

    printf("Compiler: ");
    #ifdef __clang__
        printf("Clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
    #elif defined(__GNUC__)
        printf("GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #else
        printf("Unknown\n");
    #endif
    printf("\n");

    /* Run all benchmarks */
    benchmark_starfield_creation();
    benchmark_starfield_update();
    benchmark_framebuffer();
    benchmark_render_starfield();
    benchmark_text_rendering();
    benchmark_full_frame();

    printf("========================================\n");
    printf("Benchmark complete!\n");
    printf("========================================\n");
    printf("\n");

    return 0;
}
