#include "effects.h"
#include <math.h>

void effect_linear(Starfield *field, double delta_time) {
    // TODO: Implement linear starfield effect
    // Move stars straight toward viewer (classic starfield)
    (void)field;
    (void)delta_time;
}

void effect_spiral(Starfield *field, double delta_time) {
    // TODO: Implement spiral effect
    // Rotate stars while moving forward
    (void)field;
    (void)delta_time;
}

void effect_warp(Starfield *field, double delta_time) {
    // TODO: Implement warp speed effect
    // Stretch stars into lines (Star Wars hyperspace)
    (void)field;
    (void)delta_time;
}

void effect_tunnel(Starfield *field, double delta_time) {
    // TODO: Implement tunnel effect
    // Stars move in cylindrical pattern
    (void)field;
    (void)delta_time;
}

void effect_explode(Starfield *field, double delta_time) {
    // TODO: Implement explosion effect
    // Stars radiate outward from center
    (void)field;
    (void)delta_time;
}

void effect_wave(Starfield *field, double delta_time) {
    // TODO: Implement wave effect
    // Sinusoidal motion patterns
    (void)field;
    (void)delta_time;
}

const char* effect_get_name(EffectMode mode) {
    static const char* names[] = {
        "Linear",
        "Spiral",
        "Warp Speed",
        "Tunnel",
        "Explode",
        "Wave"
    };

    if (mode >= 0 && mode < EFFECT_COUNT) {
        return names[mode];
    }
    return "Unknown";
}
