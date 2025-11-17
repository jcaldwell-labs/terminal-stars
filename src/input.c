#include "input.h"
#include <stdio.h>

bool input_init(void) {
    // TODO: Implement input initialization
    // Set up non-blocking keyboard input
    return false;
}

InputAction input_poll(void) {
    // TODO: Implement input polling
    // Check for keypresses and return appropriate action
    // Map keys: arrows/WASD for pan, +/- for zoom, etc.
    return INPUT_NONE;
}

void input_cleanup(void) {
    // TODO: Implement input cleanup
    // Restore terminal input settings
}
