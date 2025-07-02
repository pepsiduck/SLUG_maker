#include "display.h"

Rectangle display;

int8_t SLUGmaker_GraphicInit()
{
    display = (Rectangle) {
        .x = 0,
        .y = 0,
        .width = GetScreenWidth(),
        .height = GetScreenHeight()
    };
    return 0;
}

SLUG_camera SLUGmaker_DefaultCamera(SLUG_map *map)
{
    SLUG_camera cam;
    cam.map = map;
    cam.view_zone = (Rectangle) {
        .x = 0,
        .y = 0,
        .width = 1680,
        .height = 1050
    };
    cam.display = &display;
    return cam;
}

int8_t SLUGmaker_DisplayUpdate(Rectangle *rect)
{
    rect->width = GetScreenWidth();
    rect->height = GetScreenHeight();
    return 0;
}

int8_t SLUGmaker_Display(SLUG_camera *cam)
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(cam->map->fixed_sprite,cam->view_zone,*(cam->display),(Vector2) {.x = 0, .y = 0},0,WHITE);
    EndDrawing();
    return 0;
}
