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
        // Quit
        case 'q':
        case 'Q':
        case 27: // ESC
            return INPUT_QUIT;

        // Pan controls (arrow keys)
        case KEY_UP:
        case 'w':
        case 'W':
            return INPUT_PAN_UP;

        case KEY_DOWN:
        case 's':
        case 'S':
            return INPUT_PAN_DOWN;

        case KEY_LEFT:
        case 'a':
        case 'A':
            return INPUT_PAN_LEFT;

        case KEY_RIGHT:
        case 'd':
        case 'D':
            return INPUT_PAN_RIGHT;

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
        case 'e':
        case 'E':
        case '\n':
        case KEY_ENTER:
            return INPUT_CYCLE_EFFECT;

        // Rotation toggle
        case 'r':
        case 'R':
            return INPUT_TOGGLE_ROTATION;

        default:
            return INPUT_NONE;
    }
}

void input_cleanup(void) {
    // Cleanup is handled by terminal_cleanup()
    // This function is here for API completeness
}
