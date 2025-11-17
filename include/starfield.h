#ifndef STARFIELD_H
#define STARFIELD_H

#include "types.h"

// Initialize starfield with given number of stars
Starfield* starfield_create(size_t star_count);

// Update starfield state (motion, effects)
void starfield_update(Starfield *field, double delta_time);

// Apply effect-specific transformations
void starfield_apply_effect(Starfield *field, int effect_mode);

// Clean up starfield resources
void starfield_destroy(Starfield *field);

#endif // STARFIELD_H
