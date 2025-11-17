#include "input.h"
#include <ncurses.h>
#include <ctype.h>

bool input_init(void) {
    // Input is initialized by terminal_init() in ncurses
    // This function is here for API completeness
    return true;
}

InputAction input_poll(void) {
    // Non-blocking key read (configured in terminal_init)
    int ch = getch();

    if (ch == ERR) {
        return INPUT_NONE;
    }

    // Map keys to actions
    switch (ch) {
        // Quit (ESC only, Q is now for roll)
        case 27: // ESC
            return INPUT_QUIT;

        // Turn controls (arrow keys = yaw/pitch, Q/E = roll)
        case KEY_UP:
        case 'w':
        case 'W':
            return INPUT_TURN_UP;    // Pitch up

        case KEY_DOWN:
        case 's':
        case 'S':
            return INPUT_TURN_DOWN;  // Pitch down

        case KEY_LEFT:
        case 'a':
        case 'A':
            return INPUT_TURN_LEFT;  // Yaw left

        case KEY_RIGHT:
        case 'd':
        case 'D':
            return INPUT_TURN_RIGHT; // Yaw right

        // Roll controls (banking)
        case 'q':
        case 'Q':
            return INPUT_ROLL_LEFT;  // Bank left

        case 'e':
        case 'E':
            return INPUT_ROLL_RIGHT; // Bank right

        // Zoom controls
        case '+':
        case '=':
        case 'z':
        case 'Z':
            return INPUT_ZOOM_IN;

        case '-':
        case '_':
        case 'x':
        case 'X':
            return INPUT_ZOOM_OUT;

        // Speed controls
        case ']':
        case '}':
            return INPUT_SPEED_UP;

        case '[':
        case '{':
            return INPUT_SPEED_DOWN;

        // Effect cycling
        case ' ':
        case '\n':
        case KEY_ENTER:
        case '\t':  // Tab to cycle effects
            return INPUT_CYCLE_EFFECT;

        // Autopilot toggle (for torus effect)
        case 'r':
        case 'R':
            return INPUT_TOGGLE_AUTOPILOT;

        default:
            return INPUT_NONE;
    }
}

void input_cleanup(void) {
    // Cleanup is handled by terminal_cleanup()
    // This function is here for API completeness
}
