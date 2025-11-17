#ifndef RENDER_H
#define RENDER_H

#include "types.h"

// Create frame buffer for rendering
FrameBuffer* framebuffer_create(int width, int height);

// Clear frame buffer
void framebuffer_clear(FrameBuffer *fb);

// Render starfield to frame buffer
void render_starfield(FrameBuffer *fb, const Starfield *field);

// Render a ship in 3D space (transformed by camera)
void render_ship_3d(FrameBuffer *fb, const Ship3D *ship, const Camera *camera, double zoom);

// Display frame buffer to terminal
void framebuffer_display(const FrameBuffer *fb);

// Clean up frame buffer
void framebuffer_destroy(FrameBuffer *fb);

#endif // RENDER_H
