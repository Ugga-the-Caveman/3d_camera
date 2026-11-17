
#pragma once

#include "raylib.h"

extern Camera3D camera;

void camera_init(void);


// -----------------------------------------------------------------------------
// Translation
// -----------------------------------------------------------------------------

void camera_translate_world(Camera3D *camera, Vector3 delta);
void camera_translate_local(Camera3D *camera, Vector3 delta);


// -----------------------------------------------------------------------------
// Movement
// -----------------------------------------------------------------------------

void camera_move_world(Camera3D *camera, Vector3 delta);
void camera_move_local(Camera3D *camera, Vector3 delta);
void camera_move_target(Camera3D *camera, Vector3 delta);


// -----------------------------------------------------------------------------
// Rotation
// -----------------------------------------------------------------------------

void camera_rotate_world(Camera3D *camera, Vector3 rotation);
void camera_rotate_local(Camera3D *camera, Vector3 rotation);
void camera_rotate_fps(Camera3D *camera, Vector2 rotation);
void camera_rotate_local2(Camera3D *camera, Vector3 rotation);

// -----------------------------------------------------------------------------
// Orbit
// -----------------------------------------------------------------------------

void camera_orbit(Camera3D *camera, Vector2 rotation);


// -----------------------------------------------------------------------------
// Orientation
// -----------------------------------------------------------------------------

void camera_look_at(Camera3D *camera, Vector3 target);


// -----------------------------------------------------------------------------
// Distance
// -----------------------------------------------------------------------------

void camera_dolly(Camera3D *camera, float distance);
