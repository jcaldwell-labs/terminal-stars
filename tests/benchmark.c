/*
 * Performance Benchmark Suite for Terminal Stars
 *
 * Measures rendering performance, physics calculations, and frame rates
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "../include/starfield.h"
#include "../include/render.h"
#include "../include/ship.h"
#include "../include/types.h"

// Get current time in seconds with microsecond precision
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Benchmark starfield creation
void benchmark_starfield_creation() {
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
        double avg_time = ((end - start) / iterations) * 1000.0; // Convert to ms

        printf("  %5d stars: %7.3f ms/creation\n", count, avg_time);
    }
    printf("\n");
}

// Benchmark starfield updates
void benchmark_starfield_update() {
    printf("Benchmarking starfield updates...\n");

    int star_counts[] = {100, 500, 1000, 5000, 10000};
    int iterations = 1000;
    double delta_time = 0.016; // 60 FPS

    for (int i = 0; i < 5; i++) {
        int count = star_counts[i];
        Starfield *field = starfield_create(count);

        double start = get_time();

        for (int j = 0; j < iterations; j++) {
            starfield_update(field, delta_time);
        }

        double end = get_time();
        double avg_time = ((end - start) / iterations) * 1000.0; // Convert to ms
        double fps = 1000.0 / avg_time;

        printf("  %5d stars: %7.3f ms/update (%6.1f fps theoretical)\n",
               count, avg_time, fps);

        starfield_destroy(field);
    }
    printf("\n");
}

// Benchmark frame buffer operations
void benchmark_framebuffer() {
    printf("Benchmarking frame buffer operations...\n");

    int sizes[][2] = {
        {80, 24},    // Standard
        {120, 40},   // Medium
        {160, 50},   // Large
        {200, 60},   // Very large
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

// Benchmark rendering starfield to frame buffer
void benchmark_render_starfield() {
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

// Benchmark text rendering
void benchmark_text_rendering() {
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

// Estimate maximum achievable FPS
void benchmark_full_frame() {
    printf("Benchmarking full frame render cycle...\n");

    FrameBuffer *fb = framebuffer_create(120, 40);
    Starfield *field = starfield_create(2000);
    int iterations = 100;
    double delta_time = 0.016;

    double start = get_time();

    for (int i = 0; i < iterations; i++) {
        // Full frame cycle
        starfield_update(field, delta_time);
        framebuffer_clear(fb);
        render_starfield(fb, field);
        // Note: not actually displaying to terminal (would be much slower)
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

// Main benchmark runner
int main() {
    printf("\n");
    printf("========================================\n");
    printf("Terminal Stars - Performance Benchmarks\n");
    printf("========================================\n");
    printf("\n");

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

    printf("Compiler: GCC %s\n", __VERSION__);
    printf("\n");

    // Run all benchmarks
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
