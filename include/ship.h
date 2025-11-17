#ifndef SHIP_H
#define SHIP_H

#include "types.h"

// Flight constants
#define SHIP_MAX_SPEED 200.0
#define SHIP_ACCELERATION 100.0
#define SHIP_TURN_RATE 2.0         // radians per second
#define SHIP_DRAG 0.98             // Velocity multiplier (0.98 = 2% drag)
#define SHIP_CHASE_DISTANCE 15.0   // Chase camera distance

// Initialize ship at position
void ship_init(Ship3D *ship, int player_id, double x, double y, double z);

// Update ship physics
void ship_update(Ship3D *ship, double delta_time);

// Flight controls
void ship_thrust(Ship3D *ship, double amount);          // Apply thrust (0.0 to 1.0)
void ship_pitch_up(Ship3D *ship, double delta_time);    // Nose up
void ship_pitch_down(Ship3D *ship, double delta_time);  // Nose down
void ship_yaw_left(Ship3D *ship, double delta_time);    // Turn left
void ship_yaw_right(Ship3D *ship, double delta_time);   // Turn right
void ship_roll_left(Ship3D *ship, double delta_time);   // Bank left
void ship_roll_right(Ship3D *ship, double delta_time);  // Bank right

// Camera management
void ship_update_camera(const Ship3D *ship, Camera *camera);
void ship_toggle_view(Ship3D *ship);  // Switch between cockpit/chase

// Get forward vector from ship orientation
void ship_get_forward_vector(const Ship3D *ship, double *fx, double *fy, double *fz);

// Get speed (magnitude of velocity)
double ship_get_speed(const Ship3D *ship);

#endif // SHIP_H
