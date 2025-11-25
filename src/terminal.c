/*
 * Terminal management - now using termui shared library
 *
 * This file provides a thin wrapper around termui for backward compatibility
 * with the existing terminal-stars codebase.
 */

#include "terminal.h"
#include <termui.h>

bool terminal_init(void) {
    /* Use termui for initialization with default config */
    if (termui_init(NULL) != TERMUI_OK) {
        return false;
    }
    return true;
}

void terminal_cleanup(void) {
    termui_cleanup();
}

void terminal_get_size(int *width, int *height) {
    termui_get_size(width, height);
}

void terminal_handle_resize(void) {
    termui_check_resize();
}
