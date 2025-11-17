#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

// Input action types
typedef enum {
    INPUT_QUIT,
    INPUT_PAN_UP,
    INPUT_PAN_DOWN,
    INPUT_PAN_LEFT,
    INPUT_PAN_RIGHT,
    INPUT_ZOOM_IN,
    INPUT_ZOOM_OUT,
    INPUT_SPEED_UP,
    INPUT_SPEED_DOWN,
    INPUT_CYCLE_EFFECT,
    INPUT_TOGGLE_ROTATION,
    INPUT_NONE
} InputAction;

// Initialize input system (non-blocking keyboard)
bool input_init(void);

// Poll for input (non-blocking)
InputAction input_poll(void);

// Clean up input system
void input_cleanup(void);

#endif // INPUT_H
