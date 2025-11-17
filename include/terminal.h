#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>

// Initialize terminal for rendering
bool terminal_init(void);

// Restore terminal to original state
void terminal_cleanup(void);

// Get current terminal dimensions
void terminal_get_size(int *width, int *height);

// Handle terminal resize events
void terminal_handle_resize(void);

#endif // TERMINAL_H
