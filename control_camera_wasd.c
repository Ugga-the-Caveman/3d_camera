// control_camera_wasd.c

#include "control_camera_wasd.h"

#include "camera.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

typedef struct {
    void (*update)(float dt);
} CameraControlMode;

static void update_camera_mode_freefly1(float dt);
static void update_camera_mode_freefly2(float dt);
static void update_camera_mode_freefly3(float dt);

static const CameraControlMode camera_modes[] = {
    { .update = update_camera_mode_freefly1 },
    { .update = update_camera_mode_freefly2 },
    { .update = update_camera_mode_freefly3 },
};

#define CAMERA_MODE_COUNT (sizeof(camera_modes) / sizeof(camera_modes[0]))


static unsigned int current_camera_mode = 0;

void control_camera_mode_update(void)
{
    if (IsKeyPressed(KEY_RIGHT))
        {
            current_camera_mode = (current_camera_mode + 1) % CAMERA_MODE_COUNT;
            printf("current camera mode: %d\n", current_camera_mode);
        }

    if (IsKeyPressed(KEY_LEFT))
    {
        current_camera_mode = (current_camera_mode + CAMERA_MODE_COUNT - 1) % CAMERA_MODE_COUNT;
        printf("current camera mode: %d\n", current_camera_mode);
    }

}


void control_camera_wasd_update(void)
{
    float dt = GetFrameTime();
    camera_modes[current_camera_mode].update(dt);
}


static void wasd_translate_camera(float speed)
{
    Vector3 movement = {
        .x = (float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A),
        .y = (float)IsKeyDown(KEY_W) - (float)IsKeyDown(KEY_S),
        .z = (float)IsKeyDown(KEY_E) - (float)IsKeyDown(KEY_Q)
    };

    camera_translate_local(&camera, Vector3Scale(movement, speed));
}

/*
static void wasd_translate_world(float speed)
{
    Vector3 movement = {
        .x = (float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A),
        .z = - (float)IsKeyDown(KEY_W) + (float)IsKeyDown(KEY_S),
        .y = (float)IsKeyDown(KEY_E) - (float)IsKeyDown(KEY_Q)
    };

    camera_translate_world(&camera, Vector3Scale(movement, speed));
}
*/


static void ijkl_rotate_camera_local1(float rotation)
{
    Vector3 rotation2 = {
        .x = ((float)IsKeyDown(KEY_L) - (float)IsKeyDown(KEY_J)) * rotation,
        .y = ((float)IsKeyDown(KEY_K) - (float)IsKeyDown(KEY_I)) * rotation,
        .z = ((float)IsKeyDown(KEY_U) - (float)IsKeyDown(KEY_O)) * rotation,
    };

    camera_rotate_local(&camera, rotation2);
}


static void ijkl_rotate_camera_local2(float rotation)
{
    Vector2 delta = {
        .x = (float)IsKeyDown(KEY_J) - (float)IsKeyDown(KEY_L),
        .y = (float)IsKeyDown(KEY_I) - (float)IsKeyDown(KEY_K)
    };

    delta = Vector2Scale(delta, rotation);

    camera_rotate_fps(&camera, delta);
}



static void update_camera_mode_freefly1(float dt)
{
    float mouse_sensitivity = 0.005f;
    float speed = 4.0f;
    float rotation_speed = 1.0;

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        speed *= 3.0f;
        rotation_speed *= 3.0f;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        speed *= 0.25f;
        rotation_speed *= 0.25f;
    }

    wasd_translate_camera(speed * dt);

    ijkl_rotate_camera_local1(rotation_speed * dt);

/*
    Vector2 mouse_delta = GetMouseDelta();

    if (
        IsMouseButtonDown(MOUSE_BUTTON_LEFT)
    )
    {
        Vector2 mouse_delta = GetMouseDelta();

        Vector3 rotation = {
            .x = -mouse_delta.x * mouse_sensitivity,
            .y = -mouse_delta.y * mouse_sensitivity,
            .z = 0
        };

        camera_rotate_local(&camera, rotation);
    }
*/

}


static void update_camera_mode_freefly2(float dt)
{
    float mouse_sensitivity = 0.005f;
    float speed = 4.0f;
    float rotation_speed = 1.0;

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        speed *= 3.0f;
        rotation_speed *= 3.0f;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        speed *= 0.25f;
        rotation_speed *= 0.25f;
    }

    wasd_translate_camera(speed * dt);

    ijkl_rotate_camera_local2(rotation_speed * dt);
}


static void update_camera_mode_freefly3(float dt)
{
    float mouse_sensitivity = 0.005f;
    float speed = 4.0f;
    float rotation_speed = 1.0;

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        speed *= 3.0f;
        rotation_speed *= 3.0f;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        speed *= 0.25f;
        rotation_speed *= 0.25f;
    }

    wasd_translate_camera(speed * dt);

    Vector3 rotation = {
        .x = ((float)IsKeyDown(KEY_L) - (float)IsKeyDown(KEY_J)) * rotation_speed *dt,
        .y = ((float)IsKeyDown(KEY_K) - (float)IsKeyDown(KEY_I)) * rotation_speed *dt,
        .z = ((float)IsKeyDown(KEY_U) - (float)IsKeyDown(KEY_O)) * rotation_speed *dt,
    };

    camera_rotate_local2(&camera, rotation);
}

