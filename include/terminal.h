/*
 * terminal.h - Terminal Management Interface
 *
 * Provides a simple abstraction over ncurses for terminal initialization,
 * cleanup, and size management. This keeps the main code cleaner and
 * makes it easier to switch terminal libraries if needed.
 *
 * Educational Note: Wrapping ncurses in our own interface demonstrates
 * the principle of abstraction - hiding implementation details behind
 * a simpler API.
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>

/*
 * terminal_init - Initialize the terminal for rendering
 *
 * Sets up ncurses with:
 * - Raw mode (no line buffering)
 * - No echo (keypresses not printed)
 * - Keypad mode (special keys like arrows work)
 * - Colors (if available)
 *
 * Returns true on success, false on failure.
 */
bool terminal_init(void);

/*
 * terminal_cleanup - Restore terminal to original state
 *
 * Always call this before exiting, even on errors.
 * Failing to call this leaves the terminal in a broken state.
 */
void terminal_cleanup(void);

/*
 * terminal_get_size - Get current terminal dimensions
 *
 * Returns the current width (columns) and height (rows) of the terminal.
 * Values are written to the provided pointers.
 */
void terminal_get_size(int *width, int *height);

/*
 * terminal_handle_resize - Check for and handle terminal resize
 *
 * Call this each frame to detect if the terminal was resized.
 * After calling this, use terminal_get_size() to get new dimensions.
 */
void terminal_handle_resize(void);

#endif /* TERMINAL_H */
