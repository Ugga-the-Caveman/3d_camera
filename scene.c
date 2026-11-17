#include "scene.h"
#include "cleanup_stack.h"
#include "assets_path.h"


#include "raylib.h"


static Model model;
static Texture2D texture;

static Vector3 position = { 0.0f, 0.0f, 0.0f }; // model position

static void scene_unload(void)
{
    if (texture.id != 0)
        UnloadTexture(texture);

    if (IsModelValid(model))
        UnloadModel(model);

    texture = (Texture2D){0};
    model = (Model){0};
}

bool scene_init(void)
{
    if (!cleanup_stack_register(scene_unload))
        return false;

    model = LoadModel(assets_path("scenes/church/church.obj"));

    if (!IsModelValid(model))
    {
        TraceLog(LOG_ERROR, "Could not load church.obj");
        return false;
    }

    texture = LoadTexture(assets_path("scenes/church/church_diffuse.png"));

    if (texture.id == 0)
    {
        TraceLog(LOG_ERROR, "Could not load church_diffuse.png");
        UnloadModel(model);
        model = (Model){0};
        return false;
    }

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    return true;
}


void scene_draw(void)
{
    DrawModel(model, position, 0.1f, WHITE);
}