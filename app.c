// app.c

#include "platform_to_app.h"
#include "app_to_platform.h"
#include "raylib.h"

#include "camera.h"
#include "control_camera_wasd.h"

#include "scene.h"
#include "fps_display.h"

static bool show_grid = false;


bool app_init(void)
{
    camera_init();

    if (!scene_init())
    {
        return false;
    }

    return true;
}


void app_layout(Rectangle parent_rect)
{
    // gui_test_layout(parent_rect);
}

void app_update(void)
{
    if (IsKeyPressed(KEY_R))
    {
        // resets camera position and orientation
        camera_init();
    }

    control_camera_mode_update();
    control_camera_wasd_update();

    if (IsKeyPressed(KEY_G))
        show_grid = !show_grid;

    if (
        IsKeyDown(KEY_LEFT_CONTROL) &&
        IsKeyDown(KEY_LEFT_ALT) &&
        IsKeyPressed(KEY_F)
    ){
        ugga_fps_visible = !ugga_fps_visible;
    }
}


void app_draw(void)
{
    ClearBackground(RAYWHITE);


    BeginMode3D(camera);

    if (show_grid)
        DrawGrid(100, 1.0f);

    scene_draw();
    EndMode3D();

    // gui_test_draw();
}
