#ifndef WEAPONS_H
#define WEAPONS_H

#include "types.h"

// Weapons constants
#define MISSILE_SPEED 150.0           // Missile velocity
#define MISSILE_LIFETIME 5.0          // Seconds before auto-destruct
#define FIRE_COOLDOWN 0.3             // Seconds between shots
#define STARTING_MISSILES 20          // Starting missile count per player
#define EXPLOSION_DURATION 0.5        // Explosion animation duration
#define EXPLOSION_MAX_RADIUS 8.0      // Maximum explosion radius

// Initialize weapons system
void weapons_init(WeaponsSystem *weapons);

// Update all missiles and explosions (physics + lifetime)
void weapons_update(WeaponsSystem *weapons, double delta_time);

// Fire two missiles from ship toward target point
// target_x, target_y, target_z is the 3D target point in world space
void weapons_fire(WeaponsSystem *weapons, const Ship3D *ship,
                  double target_x, double target_y, double target_z);

// Create explosion at position
void weapons_create_explosion(WeaponsSystem *weapons, double x, double y, double z);

// Check if player can fire (has missiles and cooldown ready)
bool weapons_can_fire(const WeaponsSystem *weapons, int player_id);

// Get missiles remaining for player
int weapons_get_missiles_remaining(const WeaponsSystem *weapons, int player_id);

#endif // WEAPONS_H
