/*
 * effects.h - Starfield Visual Effects
 *
 * This module provides 6 different starfield animation effects.
 * Each effect modifies star positions differently each frame.
 *
 * Educational Note: These effects demonstrate various animation techniques:
 * - LINEAR: Basic translation (moving along an axis)
 * - SPIRAL: Polar coordinate transformation
 * - WARP: Variable speed with visual feedback (character changes)
 * - TUNNEL: Constrained motion with radius limits
 * - EXPLODE: Vector-based directional movement
 * - WAVE: Trigonometric oscillation (sine/cosine waves)
 */

#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"

/*
 * EffectMode - Available starfield effects
 *
 * Use these constants with starfield->effect_mode to select the active effect.
 */
typedef enum {
    EFFECT_LINEAR,      // Classic forward motion toward viewer
    EFFECT_SPIRAL,      // Stars spiral while approaching
    EFFECT_WARP,        // High-speed motion with streaking effect
    EFFECT_TUNNEL,      // Cylindrical tunnel pattern
    EFFECT_EXPLODE,     // Stars burst outward from center
    EFFECT_WAVE,        // Sinusoidal undulating motion
    EFFECT_COUNT        // Total number of effects (6)
} EffectMode;

/*
 * Effect Functions - Each modifies star positions based on delta_time
 *
 * All effects follow the same signature:
 * - field: The starfield to animate
 * - delta_time: Time since last frame (in seconds)
 *
 * Using delta_time ensures consistent animation speed regardless of frame rate.
 */

// Stars move straight toward the viewer (classic starfield)
void effect_linear(Starfield *field, double delta_time);

// Stars rotate around the Z-axis while approaching
void effect_spiral(Starfield *field, double delta_time);

// Very fast movement with character changes for motion blur
void effect_warp(Starfield *field, double delta_time);

// Stars move in a cylindrical tunnel pattern
void effect_tunnel(Starfield *field, double delta_time);

// Stars explode outward from a central point
void effect_explode(Starfield *field, double delta_time);

// Stars oscillate in sine wave patterns
void effect_wave(Starfield *field, double delta_time);

// Get human-readable effect name for display
const char* effect_get_name(EffectMode mode);

#endif // EFFECTS_H
