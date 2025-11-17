#include "terminal.h"
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>

static volatile sig_atomic_t resize_pending = 0;

static void handle_winch(int sig) {
    (void)sig;
    resize_pending = 1;
}

bool terminal_init(void) {
    // Initialize ncurses
    if (initscr() == NULL) {
        return false;
    }

    // Configure terminal mode
    cbreak();              // Disable line buffering
    noecho();              // Don't echo typed characters
    keypad(stdscr, TRUE);  // Enable special keys
    nodelay(stdscr, TRUE); // Non-blocking input
    curs_set(0);           // Hide cursor

    // Enable colors if available
    if (has_colors()) {
        start_color();
        use_default_colors();

        // Initialize color pairs for different star brightness
        init_pair(1, COLOR_WHITE, -1);   // Bright stars
        init_pair(2, COLOR_CYAN, -1);    // Medium stars
        init_pair(3, COLOR_BLUE, -1);    // Dim stars
        init_pair(4, COLOR_YELLOW, -1);  // Very bright stars
    }

    // Set up resize signal handler
    struct sigaction sa;
    sa.sa_handler = handle_winch;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, NULL);

    return true;
}

void terminal_cleanup(void) {
    // Restore terminal to normal mode
    if (!isendwin()) {
        curs_set(1);  // Show cursor
        endwin();     // End ncurses mode
    }
}

void terminal_get_size(int *width, int *height) {
    if (width && height) {
        getmaxyx(stdscr, *height, *width);
    }
}

void terminal_handle_resize(void) {
    if (resize_pending) {
        resize_pending = 0;
        endwin();
        refresh();
        clear();
    }
}
