/*
 * render.h - Frame Buffer Rendering Interface
 *
 * This module provides double-buffered rendering for smooth terminal animation.
 * Instead of writing directly to the screen (which causes flicker), we:
 * 1. Build the entire frame in a buffer
 * 2. Display the complete buffer at once
 *
 * This is the standard technique for flicker-free terminal graphics.
 */

#ifndef RENDER_H
#define RENDER_H

#include "types.h"

/*
 * Frame buffer lifecycle
 */

// Create a new frame buffer of the given dimensions
// Returns NULL on allocation failure
FrameBuffer* framebuffer_create(int width, int height);

// Clear the frame buffer (fill with spaces)
// Call this at the start of each frame
void framebuffer_clear(FrameBuffer *fb);

// Display the frame buffer contents to the terminal
// Call this after rendering is complete
void framebuffer_display(const FrameBuffer *fb);

// Free frame buffer memory
void framebuffer_destroy(FrameBuffer *fb);

/*
 * Rendering functions
 */

// Render all stars in the starfield to the frame buffer
// Handles 3D-to-2D projection with camera transformation
void render_starfield(FrameBuffer *fb, const Starfield *field);

// Render a text string at the given position with color
// Useful for HUD elements (effect name, controls, etc.)
void render_text(FrameBuffer *fb, int x, int y, const char *text, int color);

#endif // RENDER_H
