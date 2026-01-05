/*
 * terminal.c - Terminal Management Implementation
 *
 * This module wraps ncurses to provide a simple interface for terminal
 * initialization, cleanup, and size management.
 *
 * Key ncurses concepts demonstrated:
 * - initscr(): Start curses mode
 * - cbreak(): Disable line buffering (react to keys immediately)
 * - noecho(): Don't print typed characters
 * - curs_set(0): Hide the cursor
 * - keypad(): Enable special key detection
 * - start_color(): Enable color support
 *
 * Color pairs are numbered 1-4:
 *   1 = White (bright stars)
 *   2 = Cyan (medium stars)
 *   3 = Blue (dim stars)
 *   4 = Yellow (very bright stars)
 */

#include "terminal.h"
#include <ncurses.h>
#include <signal.h>

/* Track resize events */
static volatile sig_atomic_t resize_flag = 0;

/*
 * resize_handler - Signal handler for SIGWINCH (window resize)
 *
 * This is called by the OS when the terminal is resized.
 * We just set a flag here; the actual resize handling happens
 * in terminal_handle_resize() to avoid doing complex work in a signal handler.
 */
static void resize_handler(int sig) {
    (void)sig;  /* Unused parameter */
    resize_flag = 1;
}

/*
 * terminal_init - Initialize ncurses for rendering
 *
 * Sets up all the ncurses options we need for smooth animation:
 * - Raw input mode for responsive controls
 * - Hidden cursor to not distract from the starfield
 * - Color support for varied star brightness
 */
bool terminal_init(void) {
    /* Start ncurses mode */
    if (initscr() == NULL) {
        return false;
    }

    /* Raw mode: characters are passed directly without buffering */
    cbreak();

    /* Don't echo typed characters to the screen */
    noecho();

    /* Hide the cursor */
    curs_set(0);

    /* Enable special keys (function keys, arrow keys, etc.) */
    keypad(stdscr, TRUE);

    /* Initialize color support if available */
    if (has_colors()) {
        start_color();

        /* Use default terminal background */
        use_default_colors();

        /* Define color pairs for different star brightnesses
         * init_pair(pair_number, foreground, background)
         * -1 means use default terminal background */
        init_pair(1, COLOR_WHITE, -1);   /* Bright stars */
        init_pair(2, COLOR_CYAN, -1);    /* Medium stars */
        init_pair(3, COLOR_BLUE, -1);    /* Dim stars */
        init_pair(4, COLOR_YELLOW, -1);  /* Very bright stars */
    }

    /* Set up resize signal handler */
    struct sigaction sa;
    sa.sa_handler = resize_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, NULL);

    return true;
}

/*
 * terminal_cleanup - Restore terminal to original state
 *
 * This must be called before the program exits to restore
 * the terminal to its normal state.
 */
void terminal_cleanup(void) {
    /* Show cursor again */
    curs_set(1);

    /* End ncurses mode - restores terminal */
    endwin();
}

/*
 * terminal_get_size - Get current terminal dimensions
 *
 * Uses ncurses COLS and LINES globals which are updated
 * when we call endwin()/refresh() after a resize.
 */
void terminal_get_size(int *width, int *height) {
    if (width) {
        *width = COLS;
    }
    if (height) {
        *height = LINES;
    }
}

/*
 * terminal_handle_resize - Process terminal resize events
 *
 * When the terminal is resized, ncurses needs to be notified.
 * We do this by calling endwin() and refresh(), which updates
 * the LINES and COLS values.
 */
void terminal_handle_resize(void) {
    if (resize_flag) {
        resize_flag = 0;

        /* Tell ncurses to recalculate dimensions */
        endwin();
        refresh();
    }
}
