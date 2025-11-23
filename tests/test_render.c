/*
 * Test Suite for Rendering Module
 *
 * Tests frame buffer creation, manipulation, and basic rendering functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/render.h"
#include "../include/types.h"

// Test counter
static int tests_passed = 0;
static int tests_failed = 0;

// Simple test macro
#define TEST(name) \
    printf("Testing %s... ", name); \
    fflush(stdout);

#define PASS() \
    printf("PASS\n"); \
    tests_passed++;

#define FAIL(msg) \
    do { \
        printf("FAIL: %s\n", msg); \
        tests_failed++; \
        exit(1); \
    } while(0)

// Test frame buffer creation
void test_framebuffer_creation() {
    TEST("framebuffer_create");

    FrameBuffer *fb = framebuffer_create(80, 24);

    if (fb == NULL) {
        FAIL("framebuffer_create returned NULL");
    }

    if (fb->width != 80) {
        FAIL("width incorrect");
    }

    if (fb->height != 24) {
        FAIL("height incorrect");
    }

    if (fb->buffer == NULL) {
        FAIL("buffer not allocated");
    }

    if (fb->colors == NULL) {
        FAIL("colors not allocated");
    }

    framebuffer_destroy(fb);
    PASS();
}

// Test frame buffer clearing
void test_framebuffer_clear() {
    TEST("framebuffer_clear");

    FrameBuffer *fb = framebuffer_create(80, 24);
    if (fb == NULL) {
        FAIL("framebuffer_create returned NULL");
    }

    // Fill buffer with junk
    memset(fb->buffer, 'X', fb->width * fb->height);

    // Clear it
    framebuffer_clear(fb);

    // Verify all cells are cleared (should be space ' ')
    for (int i = 0; i < fb->width * fb->height; i++) {
        if (fb->buffer[i] != ' ') {
            framebuffer_destroy(fb);
            FAIL("buffer not properly cleared");
        }
    }

    framebuffer_destroy(fb);
    PASS();
}

// Test text rendering
void test_render_text() {
    TEST("render_text");

    FrameBuffer *fb = framebuffer_create(80, 24);
    if (fb == NULL) {
        FAIL("framebuffer_create returned NULL");
    }

    framebuffer_clear(fb);

    const char *test_string = "Hello";
    render_text(fb, 5, 10, test_string, 1);

    // Verify text was rendered at the correct position
    int offset = 10 * fb->width + 5;
    if (strncmp(&fb->buffer[offset], test_string, strlen(test_string)) != 0) {
        framebuffer_destroy(fb);
        FAIL("text not rendered correctly");
    }

    framebuffer_destroy(fb);
    PASS();
}

// Test text rendering bounds checking
void test_render_text_bounds() {
    TEST("render_text boundary handling");

    FrameBuffer *fb = framebuffer_create(80, 24);
    if (fb == NULL) {
        FAIL("framebuffer_create returned NULL");
    }

    framebuffer_clear(fb);

    // These should not crash (should clip or handle gracefully)
    render_text(fb, -5, 10, "Test", 1);      // Negative X
    render_text(fb, 100, 10, "Test", 1);     // X beyond width
    render_text(fb, 5, -5, "Test", 1);       // Negative Y
    render_text(fb, 5, 100, "Test", 1);      // Y beyond height
    render_text(fb, 78, 10, "LongText", 1);  // Text extends beyond width

    framebuffer_destroy(fb);
    PASS();
}

// Test framebuffer with different sizes
void test_framebuffer_various_sizes() {
    TEST("framebuffer_create with various sizes");

    // Test standard sizes
    int sizes[][2] = {
        {80, 24},   // Standard
        {120, 40},  // Large
        {40, 12},   // Small
        {200, 60},  // Very large
    };

    for (int i = 0; i < 4; i++) {
        int w = sizes[i][0];
        int h = sizes[i][1];

        FrameBuffer *fb = framebuffer_create(w, h);
        if (fb == NULL) {
            FAIL("failed to create framebuffer");
        }

        if (fb->width != w || fb->height != h) {
            framebuffer_destroy(fb);
            FAIL("incorrect dimensions");
        }

        framebuffer_destroy(fb);
    }

    PASS();
}

// Main test runner
int main() {
    printf("\n");
    printf("========================================\n");
    printf("Terminal Stars - Render Module Tests\n");
    printf("========================================\n");
    printf("\n");

    // Run all tests
    test_framebuffer_creation();
    test_framebuffer_clear();
    test_render_text();
    test_render_text_bounds();
    test_framebuffer_various_sizes();

    // Print summary
    printf("\n");
    printf("========================================\n");
    printf("Test Summary\n");
    printf("========================================\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Total:  %d\n", tests_passed + tests_failed);
    printf("========================================\n");
    printf("\n");

    return tests_failed > 0 ? 1 : 0;
}
