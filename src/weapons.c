#include "weapons.h"
#include "ship.h"
#include <math.h>
#include <string.h>

void weapons_init(WeaponsSystem *weapons) {
    if (!weapons) {
        return;
    }

    // Clear all missiles
    memset(weapons->missiles, 0, sizeof(weapons->missiles));
    for (int i = 0; i < MAX_MISSILES; i++) {
        weapons->missiles[i].active = false;
    }

    // Clear all explosions
    memset(weapons->explosions, 0, sizeof(weapons->explosions));
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        weapons->explosions[i].active = false;
    }

    // Initialize player weapon stats
    weapons->missiles_remaining[0] = STARTING_MISSILES;
    weapons->missiles_remaining[1] = STARTING_MISSILES;
    weapons->fire_cooldown[0] = 0.0;
    weapons->fire_cooldown[1] = 0.0;
}

void weapons_update(WeaponsSystem *weapons, double delta_time) {
    if (!weapons) {
        return;
    }

    // Update fire cooldowns
    for (int i = 0; i < 2; i++) {
        if (weapons->fire_cooldown[i] > 0.0) {
            weapons->fire_cooldown[i] -= delta_time;
            if (weapons->fire_cooldown[i] < 0.0) {
                weapons->fire_cooldown[i] = 0.0;
            }
        }
    }

    // Update missiles
    for (int i = 0; i < MAX_MISSILES; i++) {
        Missile *m = &weapons->missiles[i];
        if (!m->active) {
            continue;
        }

        // Age the missile
        m->age += delta_time;

        // Check lifetime
        if (m->age >= m->lifetime) {
            // Create explosion at current position
            weapons_create_explosion(weapons, m->x, m->y, m->z);
            m->active = false;
            continue;
        }

        // Update position based on velocity
        m->x += m->velocity_x * delta_time;
        m->y += m->velocity_y * delta_time;
        m->z += m->velocity_z * delta_time;
    }

    // Update explosions
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        Explosion *exp = &weapons->explosions[i];
        if (!exp->active) {
            continue;
        }

        // Age the explosion
        exp->age += delta_time;

        // Check if explosion finished
        if (exp->age >= exp->max_age) {
            exp->active = false;
            continue;
        }

        // Expand radius over time
        double progress = exp->age / exp->max_age;
        exp->radius = exp->max_radius * progress;
    }
}

void weapons_fire(WeaponsSystem *weapons, const Ship3D *ship,
                  double target_x, double target_y, double target_z) {
    if (!weapons || !ship) {
        return;
    }

    int player_id = ship->player_id;

    // Check if can fire
    if (!weapons_can_fire(weapons, player_id)) {
        return;
    }

    // Get ship forward vector for spawn position
    double forward_x, forward_y, forward_z;
    ship_get_forward_vector(ship, &forward_x, &forward_y, &forward_z);

    // Spawn offset in front of ship
    double spawn_distance = 2.0;
    double spawn_x = ship->x + forward_x * spawn_distance;
    double spawn_y = ship->y + forward_y * spawn_distance;
    double spawn_z = ship->z + forward_z * spawn_distance;

    // Calculate direction to target for each missile
    // We'll spawn two missiles slightly offset from center

    // Fire two missiles
    for (int missile_num = 0; missile_num < 2; missile_num++) {
        // Find free missile slot
        int slot = -1;
        for (int i = 0; i < MAX_MISSILES; i++) {
            if (!weapons->missiles[i].active) {
                slot = i;
                break;
            }
        }

        if (slot == -1) {
            continue;  // No free slots
        }

        Missile *m = &weapons->missiles[slot];

        // Offset spawn position left/right for dual missiles
        double offset_x = (missile_num == 0) ? -1.0 : 1.0;
        m->x = spawn_x + offset_x;
        m->y = spawn_y;
        m->z = spawn_z;

        // Calculate direction vector to target
        double dir_x = target_x - m->x;
        double dir_y = target_y - m->y;
        double dir_z = target_z - m->z;

        // Normalize direction
        double length = sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        if (length > 0.001) {
            dir_x /= length;
            dir_y /= length;
            dir_z /= length;
        } else {
            // Default forward if target is too close
            dir_x = forward_x;
            dir_y = forward_y;
            dir_z = forward_z;
        }

        // Set velocity
        m->velocity_x = dir_x * MISSILE_SPEED;
        m->velocity_y = dir_y * MISSILE_SPEED;
        m->velocity_z = dir_z * MISSILE_SPEED;

        // Set target
        m->target_x = target_x;
        m->target_y = target_y;
        m->target_z = target_z;

        // Set properties
        m->speed = MISSILE_SPEED;
        m->lifetime = MISSILE_LIFETIME;
        m->age = 0.0;
        m->owner_id = player_id;
        m->active = true;
        m->trail_char = '=';
        m->color = 1;  // Bright white
    }

    // Consume ammo and set cooldown
    weapons->missiles_remaining[player_id] -= 2;
    weapons->fire_cooldown[player_id] = FIRE_COOLDOWN;
}

void weapons_create_explosion(WeaponsSystem *weapons, double x, double y, double z) {
    if (!weapons) {
        return;
    }

    // Find free explosion slot
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!weapons->explosions[i].active) {
            Explosion *exp = &weapons->explosions[i];
            exp->x = x;
            exp->y = y;
            exp->z = z;
            exp->age = 0.0;
            exp->max_age = EXPLOSION_DURATION;
            exp->radius = 0.0;
            exp->max_radius = EXPLOSION_MAX_RADIUS;
            exp->active = true;
            exp->color = 4;  // Yellow
            return;
        }
    }
}

bool weapons_can_fire(const WeaponsSystem *weapons, int player_id) {
    if (!weapons || player_id < 0 || player_id > 1) {
        return false;
    }

    return weapons->missiles_remaining[player_id] >= 2 &&
           weapons->fire_cooldown[player_id] <= 0.0;
}

int weapons_get_missiles_remaining(const WeaponsSystem *weapons, int player_id) {
    if (!weapons || player_id < 0 || player_id > 1) {
        return 0;
    }

    return weapons->missiles_remaining[player_id];
}
