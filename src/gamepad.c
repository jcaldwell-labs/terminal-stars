#include "gamepad.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Global joystick states
static JoystickState joysticks[MAX_JOYSTICKS];
static bool initialized = false;

#ifdef HAVE_SDL2
// ============================================================================
// SDL2 IMPLEMENTATION - Full gamepad support
// ============================================================================

// Check if running in WSL
static bool is_wsl(void) {
    FILE *f = fopen("/proc/version", "r");
    if (!f) return false;

    char buffer[256];
    bool wsl = false;
    if (fgets(buffer, sizeof(buffer), f)) {
        wsl = (strstr(buffer, "microsoft") != NULL || strstr(buffer, "WSL") != NULL);
    }
    fclose(f);
    return wsl;
}

bool gamepad_init(void) {
    if (initialized) {
        return true;
    }

    // Check for environment variable to disable gamepad
    const char *disable = getenv("TERMINAL_STARS_NO_GAMEPAD");
    if (disable && disable[0] == '1') {
        initialized = true;
        return false;
    }

    // Check if running in WSL - SDL often hangs in WSL2
    if (is_wsl()) {
        // Skip gamepad in WSL to avoid SDL hanging
        initialized = true;
        return false;
    }

    // Set SDL hint to avoid hanging on WSL/environments without proper joystick support
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SDL_SetHint("SDL_JOYSTICK_LINUX_CLASSIC", "0");

    // Initialize SDL joystick subsystem only (skip haptic to avoid WSL issues)
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "Failed to initialize SDL joystick: %s\n", SDL_GetError());
        return false;
    }

    // Initialize joystick states
    for (int i = 0; i < MAX_JOYSTICKS; i++) {
        joysticks[i].joystick = NULL;
        joysticks[i].haptic = NULL;
        joysticks[i].device_index = -1;
        joysticks[i].connected = false;
        joysticks[i].dead_zone = 0.15f; // 15% dead zone
        memset(joysticks[i].buttons, 0, sizeof(joysticks[i].buttons));
        joysticks[i].axis_left_x = 0.0f;
        joysticks[i].axis_left_y = 0.0f;
        joysticks[i].axis_right_x = 0.0f;
        joysticks[i].axis_right_y = 0.0f;
        joysticks[i].trigger_left = 0.0f;
        joysticks[i].trigger_right = 0.0f;
    }

    // Open available joysticks (up to MAX_JOYSTICKS)
    int num_joysticks = SDL_NumJoysticks();

    for (int i = 0; i < num_joysticks && i < MAX_JOYSTICKS; i++) {
        SDL_Joystick *joy = SDL_JoystickOpen(i);
        if (joy) {
            joysticks[i].joystick = joy;
            joysticks[i].device_index = i;
            joysticks[i].connected = true;

            // Get joystick name
            const char *name = SDL_JoystickName(joy);
            if (name) {
                strncpy(joysticks[i].name, name, sizeof(joysticks[i].name) - 1);
                joysticks[i].name[sizeof(joysticks[i].name) - 1] = '\0';
            }

            // Try to open haptic device for force feedback
            if (SDL_JoystickIsHaptic(joy)) {
                joysticks[i].haptic = SDL_HapticOpenFromJoystick(joy);
                if (joysticks[i].haptic) {
                    SDL_HapticRumbleInit(joysticks[i].haptic);
                }
            }

            printf("Joystick %d: %s (Axes: %d, Buttons: %d)\n",
                   i, joysticks[i].name,
                   SDL_JoystickNumAxes(joy),
                   SDL_JoystickNumButtons(joy));
        }
    }

    initialized = true;
    return true;
}

void gamepad_update(void) {
    if (!initialized) {
        return;
    }

    // Update SDL events to get latest joystick state
    SDL_JoystickUpdate();

    for (int i = 0; i < MAX_JOYSTICKS; i++) {
        if (!joysticks[i].connected || !joysticks[i].joystick) {
            continue;
        }

        SDL_Joystick *joy = joysticks[i].joystick;

        // Update axes (convert from -32768..32767 to -1.0..1.0)
        int num_axes = SDL_JoystickNumAxes(joy);

        if (num_axes > AXIS_LEFT_X) {
            float raw = SDL_JoystickGetAxis(joy, AXIS_LEFT_X) / 32768.0f;
            joysticks[i].axis_left_x = gamepad_apply_deadzone(raw, joysticks[i].dead_zone);
        }

        if (num_axes > AXIS_LEFT_Y) {
            float raw = SDL_JoystickGetAxis(joy, AXIS_LEFT_Y) / 32768.0f;
            joysticks[i].axis_left_y = gamepad_apply_deadzone(raw, joysticks[i].dead_zone);
        }

        if (num_axes > AXIS_RIGHT_X) {
            float raw = SDL_JoystickGetAxis(joy, AXIS_RIGHT_X) / 32768.0f;
            joysticks[i].axis_right_x = gamepad_apply_deadzone(raw, joysticks[i].dead_zone);
        }

        if (num_axes > AXIS_RIGHT_Y) {
            float raw = SDL_JoystickGetAxis(joy, AXIS_RIGHT_Y) / 32768.0f;
            joysticks[i].axis_right_y = gamepad_apply_deadzone(raw, joysticks[i].dead_zone);
        }

        if (num_axes > AXIS_TRIGGER_LEFT) {
            float raw = SDL_JoystickGetAxis(joy, AXIS_TRIGGER_LEFT) / 32768.0f;
            joysticks[i].trigger_left = (raw + 1.0f) / 2.0f; // Convert to 0.0..1.0
        }

        if (num_axes > AXIS_TRIGGER_RIGHT) {
            float raw = SDL_JoystickGetAxis(joy, AXIS_TRIGGER_RIGHT) / 32768.0f;
            joysticks[i].trigger_right = (raw + 1.0f) / 2.0f; // Convert to 0.0..1.0
        }

        // Update buttons
        int num_buttons = SDL_JoystickNumButtons(joy);
        for (int b = 0; b < num_buttons && b < 16; b++) {
            joysticks[i].buttons[b] = SDL_JoystickGetButton(joy, b) != 0;
        }
    }
}

void gamepad_rumble(int player_id, float strength, int duration_ms) {
    if (player_id < 0 || player_id >= MAX_JOYSTICKS) {
        return;
    }

    if (!joysticks[player_id].connected || !joysticks[player_id].haptic) {
        return;
    }

    // Clamp strength to 0.0..1.0
    if (strength < 0.0f) strength = 0.0f;
    if (strength > 1.0f) strength = 1.0f;

    SDL_HapticRumblePlay(joysticks[player_id].haptic, strength, duration_ms);
}

void gamepad_cleanup(void) {
    if (!initialized) {
        return;
    }

    for (int i = 0; i < MAX_JOYSTICKS; i++) {
        if (joysticks[i].haptic) {
            SDL_HapticClose(joysticks[i].haptic);
            joysticks[i].haptic = NULL;
        }
        if (joysticks[i].joystick) {
            SDL_JoystickClose(joysticks[i].joystick);
            joysticks[i].joystick = NULL;
        }
        joysticks[i].connected = false;
    }

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    initialized = false;
}

#else
// ============================================================================
// STUB IMPLEMENTATION - No SDL2 available
// ============================================================================

bool gamepad_init(void) {
    // Initialize joystick states (all disconnected)
    for (int i = 0; i < MAX_JOYSTICKS; i++) {
        joysticks[i].joystick = NULL;
        joysticks[i].haptic = NULL;
        joysticks[i].device_index = -1;
        joysticks[i].connected = false;
        joysticks[i].dead_zone = 0.15f;
        memset(joysticks[i].buttons, 0, sizeof(joysticks[i].buttons));
        joysticks[i].axis_left_x = 0.0f;
        joysticks[i].axis_left_y = 0.0f;
        joysticks[i].axis_right_x = 0.0f;
        joysticks[i].axis_right_y = 0.0f;
        joysticks[i].trigger_left = 0.0f;
        joysticks[i].trigger_right = 0.0f;
        strncpy(joysticks[i].name, "No SDL2 Support", sizeof(joysticks[i].name) - 1);
        joysticks[i].name[sizeof(joysticks[i].name) - 1] = '\0';
    }

    initialized = true;
    return false;  // Return false to indicate no gamepad support
}

void gamepad_update(void) {
    // No-op
}

void gamepad_rumble(int player_id, float strength, int duration_ms) {
    (void)player_id;
    (void)strength;
    (void)duration_ms;
    // No-op
}

void gamepad_cleanup(void) {
    initialized = false;
}

#endif  // HAVE_SDL2

// ============================================================================
// COMMON IMPLEMENTATION - Works with or without SDL2
// ============================================================================

JoystickState* gamepad_get_state(int player_id) {
    if (player_id < 0 || player_id >= MAX_JOYSTICKS) {
        return NULL;
    }

    if (!joysticks[player_id].connected) {
        return NULL;
    }

    return &joysticks[player_id];
}

int gamepad_count(void) {
    int count = 0;
    for (int i = 0; i < MAX_JOYSTICKS; i++) {
        if (joysticks[i].connected) {
            count++;
        }
    }
    return count;
}

float gamepad_apply_deadzone(float value, float dead_zone) {
    if (fabs(value) < dead_zone) {
        return 0.0f;
    }

    // Scale the value so it ranges from 0 to 1 outside the dead zone
    float sign = (value < 0) ? -1.0f : 1.0f;
    float magnitude = fabs(value);
    float scaled = (magnitude - dead_zone) / (1.0f - dead_zone);

    return sign * scaled;
}

int gamepad_axis_to_direction(float value, float threshold) {
    if (value < -threshold) {
        return -1;
    } else if (value > threshold) {
        return 1;
    }
    return 0;
}
