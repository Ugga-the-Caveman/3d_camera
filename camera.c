
#include "camera.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>

Camera3D camera;

static Quaternion camera_reference_orientation;

static Quaternion camera_orientation;


static float camera_rotation_yaw;
static float camera_rotation_pitch;
static float camera_rotation_roll;



// -----------------------------------------------------------------------------
// Internal helpers
// -----------------------------------------------------------------------------

static void camera_sync_quaternion(Camera3D *camera)
{
    Vector3 forward = Vector3Normalize(
        Vector3Subtract(camera->target, camera->position)
    );

    Vector3 right = Vector3Normalize(
        Vector3CrossProduct(forward, camera->up)
    );

    Vector3 up = Vector3CrossProduct(right, forward);

    Matrix rotation = MatrixLookAt(
        Vector3Zero(),
        forward,
        up
    );

    camera_orientation = QuaternionFromMatrix(
        MatrixInvert(rotation)
    );
}

static void camera_sync_angles(Camera3D *camera)
{
    Vector3 forward = Vector3Normalize(
        Vector3Subtract(camera->target, camera->position)
    );

    Vector3 right = Vector3Normalize(
        Vector3CrossProduct(forward, camera->up)
    );

    Vector3 up = Vector3CrossProduct(right, forward);

    Matrix rotation = MatrixLookAt(
        Vector3Zero(),
        forward,
        up
    );

    Quaternion current = QuaternionFromMatrix(
        MatrixInvert(rotation)
    );

    Quaternion relative = QuaternionMultiply(
        QuaternionInvert(camera_reference_orientation),
        current
    );

    camera_rotation_yaw = -atan2f(
        2.0f * (relative.w * relative.y - relative.x * relative.z),
        1.0f - 2.0f * (relative.x * relative.x + relative.y * relative.y)
    );

    camera_rotation_pitch = -asinf(
        2.0f * (relative.w * relative.x + relative.y * relative.z)
    );

    camera_rotation_roll = atan2f(
        2.0f * (relative.w * relative.z - relative.x * relative.y),
        1.0f - 2.0f * (relative.x * relative.x + relative.z * relative.z)
    );
}

static void camera_sync_orientation(Camera3D *camera)
{
    Vector3 forward = Vector3RotateByQuaternion(
        (Vector3){ 0, 0, -1 },
        camera_orientation
    );

    camera->up = Vector3RotateByQuaternion(
        (Vector3){ 0, 1, 0 },
        camera_orientation
    );

    camera->target = Vector3Add(
        camera->position,
        Vector3Scale(forward, 1.0f)
    );
}


// -----------------------------------------------------------------------------
// Initialization
// -----------------------------------------------------------------------------

void camera_init(void)
{
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // am anfang soll die camera von oben auf den world punkt [0,0,0] gucken, so das nach rechts +x ist, und nach unten +y
    camera.position = (Vector3){ 1, 10, 1 };
    camera.target   = (Vector3){ 1, 0, 1 };
    camera.up       = (Vector3){ 0, 0, -1 };

    // Orientierung direkt aus der Raylib-Kamera ableiten.
    camera_sync_quaternion(&camera);

    // Ausgangsorientierung festhalten.
    camera_reference_orientation = camera_orientation;

    // winkel aus der tatsächlichen Kamera ableiten.
    camera_sync_angles(&camera);
    printf("x: %f\n", camera_rotation_yaw);
    printf("y: %f\n", camera_rotation_pitch);
    printf("z: %f\n", camera_rotation_roll);
}


// -----------------------------------------------------------------------------
// Translation
// -----------------------------------------------------------------------------

void camera_translate_world(Camera3D *camera, Vector3 delta) {}
void camera_translate_local(Camera3D *camera, Vector3 delta)
{
    Vector3 forward = Vector3Normalize(
        Vector3Subtract(camera->target, camera->position)
    );

    Vector3 right = Vector3Normalize(
        Vector3CrossProduct(forward, camera->up)
    );

    Vector3 up = Vector3CrossProduct(right, forward);

    Vector3 offset =
        Vector3Scale(right, delta.x);

    offset = Vector3Add(offset,
        Vector3Scale(up, delta.y));

    offset = Vector3Subtract(offset,
        Vector3Scale(forward, delta.z));

    camera->position = Vector3Add(camera->position, offset);
    camera->target   = Vector3Add(camera->target, offset);
}

// -----------------------------------------------------------------------------
// Movement
// -----------------------------------------------------------------------------

void camera_move_world(Camera3D *camera, Vector3 delta) {}
void camera_move_local(Camera3D *camera, Vector3 delta) {}
void camera_move_target(Camera3D *camera, Vector3 delta) {}


// -----------------------------------------------------------------------------
// Rotation
// -----------------------------------------------------------------------------

void camera_rotate_world(Camera3D *camera, Vector3 rotation) {}


void camera_rotate_local(Camera3D *camera, Vector3 rotation)
{
    // Motorwinkel unabhängig aktualisieren.
    camera_rotation_yaw   += rotation.x;
    camera_rotation_pitch += rotation.y;
    camera_rotation_roll  += rotation.z;

    // Rotationen um die ursprünglichen Kameraachsen.
    Quaternion yaw = QuaternionFromAxisAngle(
        (Vector3){ 0, 1, 0 },
        -camera_rotation_yaw
    );

    Quaternion pitch = QuaternionFromAxisAngle(
        (Vector3){ 1, 0, 0 },
        -camera_rotation_pitch
    );

    Quaternion roll = QuaternionFromAxisAngle(
        (Vector3){ 0, 0, 1 },
        camera_rotation_roll
    );

    // Orientierung aus Referenz und Motorstellungen zusammensetzen.
    camera_orientation = QuaternionMultiply(
        camera_reference_orientation,
        QuaternionMultiply(
            roll,
            QuaternionMultiply(pitch, yaw)
        )
    );

    camera_orientation = QuaternionNormalize(camera_orientation);
    camera_sync_orientation(camera);
    // camera_sync_angles(camera); // das müssen wir nicht machen weil die winkel bereits in der funktion hier verändert wurden
}

void camera_rotate_local2(Camera3D *camera, Vector3 rotation)
{
    Quaternion yaw = QuaternionFromAxisAngle(
        (Vector3){ 0, 1, 0 },
        -rotation.x
    );

    Quaternion pitch = QuaternionFromAxisAngle(
        (Vector3){ 1, 0, 0 },
        -rotation.y
    );

    Quaternion roll = QuaternionFromAxisAngle(
        (Vector3){ 0, 0, 1 },
        rotation.z
    );

    camera_orientation = QuaternionMultiply(camera_orientation, yaw);
    camera_orientation = QuaternionMultiply(camera_orientation, pitch);
    camera_orientation = QuaternionMultiply(camera_orientation, roll);

    camera_orientation = QuaternionNormalize(camera_orientation);
    camera_sync_orientation(camera);
    camera_sync_angles(camera);
}

void camera_rotate_fps(Camera3D *camera, Vector2 rotation)
{
    const Vector3 world_up = { 0, 0, -1 };

    // Yaw: Rotation um die feste Welt-Up-Achse.
    Quaternion yaw = QuaternionFromAxisAngle(
        world_up,
        rotation.x
    );

    camera_orientation = QuaternionMultiply(
        yaw,
        camera_orientation
    );

    // Pitch: Rotation um die lokale Rechtsachse.
    Quaternion pitch = QuaternionFromAxisAngle(
        (Vector3){ 1, 0, 0 },
        rotation.y
    );

    camera_orientation = QuaternionMultiply(
        camera_orientation,
        pitch
    );

    camera_orientation = QuaternionNormalize(camera_orientation);
    camera_sync_orientation(camera);
    camera_sync_angles(camera);
}

// -----------------------------------------------------------------------------
// Orbit
// -----------------------------------------------------------------------------

void camera_orbit(Camera3D *camera, Vector2 rotation) {}


// -----------------------------------------------------------------------------
// Orientation
// -----------------------------------------------------------------------------

void camera_look_at(Camera3D *camera, Vector3 target) {}


// -----------------------------------------------------------------------------
// Distance
// -----------------------------------------------------------------------------

void camera_dolly(Camera3D *camera, float distance) {}
