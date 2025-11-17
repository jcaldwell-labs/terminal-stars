#include "ship.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ship_init(Ship3D *ship, int player_id, double x, double y, double z) {
    memset(ship, 0, sizeof(Ship3D));

    ship->x = x;
    ship->y = y;
    ship->z = z;

    ship->pitch = 0.0;
    ship->yaw = 0.0;
    ship->roll = 0.0;

    ship->max_speed = SHIP_MAX_SPEED;
    ship->acceleration = SHIP_ACCELERATION;
    ship->turn_rate = SHIP_TURN_RATE;
    ship->drag = SHIP_DRAG;

    ship->player_id = player_id;
    ship->health = 3;
    ship->active = true;

    ship->view_mode = VIEW_COCKPIT;  // Start in cockpit view
    ship->chase_distance = SHIP_CHASE_DISTANCE;
}

void ship_update(Ship3D *ship, double delta_time) {
    if (!ship || !ship->active) {
        return;
    }

    // Apply drag to velocity
    ship->velocity_x *= ship->drag;
    ship->velocity_y *= ship->drag;
    ship->velocity_z *= ship->drag;

    // Apply drag to angular velocity (smooth rotation damping)
    ship->angular_pitch *= 0.9;
    ship->angular_yaw *= 0.9;
    ship->angular_roll *= 0.9;

    // Update orientation from angular velocity
    ship->pitch += ship->angular_pitch * delta_time;
    ship->yaw += ship->angular_yaw * delta_time;
    ship->roll += ship->angular_roll * delta_time;

    // Clamp pitch to prevent flipping over
    if (ship->pitch > M_PI / 2.0) ship->pitch = M_PI / 2.0;
    if (ship->pitch < -M_PI / 2.0) ship->pitch = -M_PI / 2.0;

    // Normalize yaw to [0, 2π]
    while (ship->yaw < 0) ship->yaw += 2.0 * M_PI;
    while (ship->yaw >= 2.0 * M_PI) ship->yaw -= 2.0 * M_PI;

    // Update position from velocity
    ship->x += ship->velocity_x * delta_time;
    ship->y += ship->velocity_y * delta_time;
    ship->z += ship->velocity_z * delta_time;

    // Clamp speed
    double speed = sqrt(ship->velocity_x * ship->velocity_x +
                       ship->velocity_y * ship->velocity_y +
                       ship->velocity_z * ship->velocity_z);

    if (speed > ship->max_speed) {
        double scale = ship->max_speed / speed;
        ship->velocity_x *= scale;
        ship->velocity_y *= scale;
        ship->velocity_z *= scale;
    }
}

void ship_get_forward_vector(const Ship3D *ship, double *fx, double *fy, double *fz) {
    // Calculate forward direction from yaw and pitch
    // Z-forward coordinate system (matches rendering system)
    // Forward vector when yaw=0, pitch=0 should be (0, 0, 1) pointing along +Z
    double cos_pitch = cos(ship->pitch);
    *fx = sin(ship->yaw) * cos_pitch;     // Right component
    *fy = sin(ship->pitch);                // Up/down component
    *fz = cos(ship->yaw) * cos_pitch;     // Forward component (Z-axis)
}

void ship_thrust(Ship3D *ship, double amount) {
    if (!ship || !ship->active) {
        return;
    }

    // Get forward vector
    double fx, fy, fz;
    ship_get_forward_vector(ship, &fx, &fy, &fz);

    // Apply thrust in forward direction
    double thrust_force = ship->acceleration * amount;
    ship->velocity_x += fx * thrust_force * 0.016;  // Approximate delta_time
    ship->velocity_y += fy * thrust_force * 0.016;
    ship->velocity_z += fz * thrust_force * 0.016;
}

void ship_pitch_up(Ship3D *ship, double delta_time) {
    (void)delta_time;  // Arcade-style instant turn
    if (!ship || !ship->active) {
        return;
    }
    ship->angular_pitch = ship->turn_rate;
}

void ship_pitch_down(Ship3D *ship, double delta_time) {
    (void)delta_time;
    if (!ship || !ship->active) {
        return;
    }
    ship->angular_pitch = -ship->turn_rate;
}

void ship_yaw_left(Ship3D *ship, double delta_time) {
    (void)delta_time;
    if (!ship || !ship->active) {
        return;
    }
    ship->angular_yaw = ship->turn_rate;
}

void ship_yaw_right(Ship3D *ship, double delta_time) {
    (void)delta_time;
    if (!ship || !ship->active) {
        return;
    }
    ship->angular_yaw = -ship->turn_rate;
}

void ship_roll_left(Ship3D *ship, double delta_time) {
    (void)delta_time;
    if (!ship || !ship->active) {
        return;
    }
    ship->angular_roll = ship->turn_rate;
}

void ship_roll_right(Ship3D *ship, double delta_time) {
    (void)delta_time;
    if (!ship || !ship->active) {
        return;
    }
    ship->angular_roll = -ship->turn_rate;
}

void ship_update_camera(const Ship3D *ship, Camera *camera) {
    if (!ship || !camera) {
        return;
    }

    if (ship->view_mode == VIEW_COCKPIT) {
        // Cockpit view: camera IS the ship
        camera->pos_x = ship->x;
        camera->pos_y = ship->y;
        camera->pos_z = ship->z;

        camera->pitch = ship->pitch;
        camera->yaw = ship->yaw;
        camera->roll = ship->roll;

    } else if (ship->view_mode == VIEW_CHASE) {
        // Chase cam: behind and above the ship
        double fx, fy, fz;
        ship_get_forward_vector(ship, &fx, &fy, &fz);

        // Position camera behind ship
        camera->pos_x = ship->x - fx * ship->chase_distance;
        camera->pos_y = ship->y - fy * ship->chase_distance;
        camera->pos_z = ship->z - fz * ship->chase_distance + 5.0;  // +5 up

        // Look at ship (calculate direction to ship)
        double dx = ship->x - camera->pos_x;
        double dy = ship->y - camera->pos_y;
        double dz = ship->z - camera->pos_z;

        camera->yaw = atan2(dy, dx);
        camera->pitch = atan2(dz, sqrt(dx*dx + dy*dy));
        camera->roll = ship->roll * 0.5;  // Slight roll with ship
    }
}

void ship_toggle_view(Ship3D *ship) {
    if (!ship) {
        return;
    }

    if (ship->view_mode == VIEW_COCKPIT) {
        ship->view_mode = VIEW_CHASE;
    } else {
        ship->view_mode = VIEW_COCKPIT;
    }
}

double ship_get_speed(const Ship3D *ship) {
    if (!ship) {
        return 0.0;
    }

    return sqrt(ship->velocity_x * ship->velocity_x +
                ship->velocity_y * ship->velocity_y +
                ship->velocity_z * ship->velocity_z);
}
