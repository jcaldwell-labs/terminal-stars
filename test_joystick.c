#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    printf("SDL2 Joystick Test\n");
    printf("==================\n\n");

    // Get number of joysticks
    int num_joysticks = SDL_NumJoysticks();
    printf("Found %d joystick(s)\n\n", num_joysticks);

    if (num_joysticks == 0) {
        printf("No joysticks detected.\n");
        printf("\nPossible issues:\n");
        printf("  - Joystick not attached via usbipd\n");
        printf("  - Permissions issue (need 'input' group)\n");
        printf("  - Device not supported by SDL2\n");
        SDL_Quit();
        return 0;
    }

    // Open and display info for each joystick
    for (int i = 0; i < num_joysticks; i++) {
        SDL_Joystick *joy = SDL_JoystickOpen(i);
        if (joy) {
            printf("Joystick %d:\n", i);
            printf("  Name: %s\n", SDL_JoystickName(joy));
            printf("  Axes: %d\n", SDL_JoystickNumAxes(joy));
            printf("  Buttons: %d\n", SDL_JoystickNumButtons(joy));
            printf("  Hats: %d\n", SDL_JoystickNumHats(joy));
            printf("  Balls: %d\n", SDL_JoystickNumBalls(joy));

            // Check for haptic support
            if (SDL_JoystickIsHaptic(joy)) {
                printf("  Haptic: Yes (rumble supported)\n");
            } else {
                printf("  Haptic: No\n");
            }
            printf("\n");

            // Test reading some values
            printf("  Testing axes (move your joystick):\n");
            SDL_JoystickUpdate();
            for (int a = 0; a < SDL_JoystickNumAxes(joy) && a < 6; a++) {
                int16_t value = SDL_JoystickGetAxis(joy, a);
                float normalized = value / 32768.0f;
                printf("    Axis %d: %6d (%.2f)\n", a, value, normalized);
            }
            printf("\n");

            printf("  Testing buttons (press some buttons):\n");
            for (int b = 0; b < SDL_JoystickNumButtons(joy) && b < 16; b++) {
                uint8_t pressed = SDL_JoystickGetButton(joy, b);
                if (pressed) {
                    printf("    Button %d: PRESSED\n", b);
                }
            }
            printf("\n");

            SDL_JoystickClose(joy);
        } else {
            printf("Failed to open joystick %d: %s\n", i, SDL_GetError());
        }
    }

    printf("Test complete!\n");
    SDL_Quit();
    return 0;
}
