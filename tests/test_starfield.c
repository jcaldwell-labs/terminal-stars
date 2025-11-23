/*
 * Test Suite for Starfield Module
 *
 * Tests starfield initialization, star management, and basic operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../include/starfield.h"
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
    printf("FAIL: %s\n", msg); \
    tests_failed++; \
    return;

// Test starfield initialization
void test_starfield_init() {
    TEST("starfield_create");

    Starfield *field = starfield_create(1000);

    if (field == NULL) {
        FAIL("starfield_create returned NULL");
    }

    if (field->stars == NULL) {
        FAIL("stars array not allocated");
    }

    if (field->star_count != 1000) {
        FAIL("incorrect star count");
    }

    if (field->speed <= 0.0) {
        FAIL("speed not initialized properly");
    }

    starfield_destroy(field);
    PASS();
}

// Test star positions are initialized
void test_star_positions() {
    TEST("star position initialization");

    Starfield *field = starfield_create(100);

    if (field == NULL) {
        FAIL("starfield_create returned NULL");
    }

    int stars_with_valid_positions = 0;

    for (size_t i = 0; i < field->star_count; i++) {
        Star *s = &field->stars[i];

        // Stars should have some position
        // At minimum, z should be positive (in front of camera)
        if (s->z > 0.0) {
            stars_with_valid_positions++;
        }
    }

    if (stars_with_valid_positions == 0) {
        starfield_destroy(field);
        FAIL("no stars have valid positions");
    }

    starfield_destroy(field);
    PASS();
}

// Test starfield update doesn't crash
void test_starfield_update() {
    TEST("starfield_update");

    Starfield *field = starfield_create(500);

    if (field == NULL) {
        FAIL("starfield_create returned NULL");
    }

    // Update several times with different delta times
    double delta_times[] = {0.016, 0.033, 0.001, 0.1};

    for (int i = 0; i < 4; i++) {
        starfield_update(field, delta_times[i]);

        // Verify stars still exist
        if (field->stars == NULL) {
            starfield_destroy(field);
            FAIL("stars became NULL after update");
        }
    }

    starfield_destroy(field);
    PASS();
}

// Test starfield with different star counts
void test_starfield_various_counts() {
    TEST("starfield with various star counts");

    int counts[] = {10, 100, 500, 1000, 5000};

    for (int i = 0; i < 5; i++) {
        Starfield *field = starfield_create(counts[i]);

        if (field == NULL) {
            FAIL("starfield_create returned NULL");
        }

        if (field->star_count != (size_t)counts[i]) {
            starfield_destroy(field);
            FAIL("incorrect star count");
        }

        if (field->stars == NULL) {
            starfield_destroy(field);
            FAIL("stars not allocated");
        }

        starfield_destroy(field);
    }

    PASS();
}

// Test camera initialization
void test_camera_init() {
    TEST("camera initialization");

    Starfield *field = starfield_create(100);

    if (field == NULL) {
        FAIL("starfield_create returned NULL");
    }

    // Camera should be initialized
    Camera *cam = &field->camera;

    // Initial position should be reasonable (likely at origin or nearby)
    if (isnan(cam->pos_x) || isnan(cam->pos_y) || isnan(cam->pos_z)) {
        starfield_destroy(field);
        FAIL("camera position contains NaN");
    }

    // Orientation angles should be reasonable
    if (isnan(cam->pitch) || isnan(cam->yaw) || isnan(cam->roll)) {
        starfield_destroy(field);
        FAIL("camera orientation contains NaN");
    }

    starfield_destroy(field);
    PASS();
}

// Main test runner
int main() {
    printf("\n");
    printf("========================================\n");
    printf("Terminal Stars - Starfield Module Tests\n");
    printf("========================================\n");
    printf("\n");

    // Run all tests
    test_starfield_init();
    test_star_positions();
    test_starfield_update();
    test_starfield_various_counts();
    test_camera_init();

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
