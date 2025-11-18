#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <stdbool.h>

// Optional SDL2 support - only include if available
#ifdef HAVE_SDL2
#include <SDL2/SDL.h>
#else
// Stub SDL types when SDL2 not available
typedef void* SDL_Joystick;
typedef void* SDL_Haptic;
#endif

// Maximum number of joysticks supported
#define MAX_JOYSTICKS 2

// Joystick axis indices
typedef enum {
    AXIS_LEFT_X = 0,       // Left stick horizontal
    AXIS_LEFT_Y = 1,       // Left stick vertical
    AXIS_RIGHT_X = 2,      // Right stick horizontal (optional)
    AXIS_RIGHT_Y = 3,      // Right stick vertical (optional)
    AXIS_TRIGGER_LEFT = 4, // Left trigger
    AXIS_TRIGGER_RIGHT = 5 // Right trigger
} JoystickAxis;

// Joystick button indices (standard gamepad layout)
typedef enum {
    BUTTON_A = 0,          // Fire/Select
    BUTTON_B = 1,          // Back/Cancel
    BUTTON_X = 2,          // Secondary action
    BUTTON_Y = 3,          // Tertiary action
    BUTTON_LB = 4,         // Left bumper
    BUTTON_RB = 5,         // Right bumper
    BUTTON_BACK = 6,       // Back/Select
    BUTTON_START = 7,      // Start/Menu
    BUTTON_LS = 8,         // Left stick click
    BUTTON_RS = 9          // Right stick click
} JoystickButton;

// Joystick state for one controller
typedef struct {
    SDL_Joystick *joystick;
    SDL_Haptic *haptic;    // Force feedback (if available)
    int device_index;      // SDL device index
    bool connected;        // Is this joystick connected

    // Axis values (-1.0 to 1.0)
    float axis_left_x;
    float axis_left_y;
    float axis_right_x;
    float axis_right_y;
    float trigger_left;
    float trigger_right;

    // Button states (pressed = true)
    bool buttons[16];

    // Dead zone threshold
    float dead_zone;

    // Name of the controller
    char name[256];
} JoystickState;

// Initialize gamepad system
bool gamepad_init(void);

// Update all connected joysticks
void gamepad_update(void);

// Get joystick state for a specific player (0 or 1)
JoystickState* gamepad_get_state(int player_id);

// Get number of connected joysticks
int gamepad_count(void);

// Rumble/vibration (if supported)
void gamepad_rumble(int player_id, float strength, int duration_ms);

// Clean up gamepad system
void gamepad_cleanup(void);

// Apply dead zone to axis value
float gamepad_apply_deadzone(float value, float dead_zone);

// Convert axis value to direction (-1, 0, 1)
int gamepad_axis_to_direction(float value, float threshold);

#endif // GAMEPAD_H
