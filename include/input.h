#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

// Input action types
typedef enum {
    INPUT_QUIT,
    INPUT_TURN_UP,          // Pitch up (look up)
    INPUT_TURN_DOWN,        // Pitch down (look down)
    INPUT_TURN_LEFT,        // Yaw left (turn left)
    INPUT_TURN_RIGHT,       // Yaw right (turn right)
    INPUT_ROLL_LEFT,        // Roll left (bank left)
    INPUT_ROLL_RIGHT,       // Roll right (bank right)
    INPUT_ZOOM_IN,
    INPUT_ZOOM_OUT,
    INPUT_SPEED_UP,
    INPUT_SPEED_DOWN,
    INPUT_CYCLE_EFFECT,
    INPUT_TOGGLE_AUTOPILOT, // Toggle auto-path following
    INPUT_NONE
} InputAction;

// Initialize input system (non-blocking keyboard)
bool input_init(void);

// Poll for input (non-blocking)
InputAction input_poll(void);

// Clean up input system
void input_cleanup(void);

#endif // INPUT_H
