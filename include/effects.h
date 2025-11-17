#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"

// Effect modes
typedef enum {
    EFFECT_LINEAR,      // Straight motion toward viewer
    EFFECT_SPIRAL,      // Spiral motion
    EFFECT_WARP,        // Warp speed effect
    EFFECT_TUNNEL,      // Tunnel effect
    EFFECT_EXPLODE,     // Explosion effect
    EFFECT_WAVE,        // Wave motion
    EFFECT_TORUS,       // Roller coaster loop (3D torus path)
    EFFECT_COUNT        // Total number of effects
} EffectMode;

// Apply linear starfield motion
void effect_linear(Starfield *field, double delta_time);

// Apply spiral motion effect
void effect_spiral(Starfield *field, double delta_time);

// Apply warp speed effect
void effect_warp(Starfield *field, double delta_time);

// Apply tunnel effect
void effect_tunnel(Starfield *field, double delta_time);

// Apply explosion effect
void effect_explode(Starfield *field, double delta_time);

// Apply wave motion effect
void effect_wave(Starfield *field, double delta_time);

// Apply torus roller coaster effect
void effect_torus(Starfield *field, double delta_time);

// Get effect name for display
const char* effect_get_name(EffectMode mode);

#endif // EFFECTS_H
