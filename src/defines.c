#include "defines.h"

SLUGmaker_GraphicVariables graphic_vars;

int8_t SLUGmaker_GraphicInit()
{
    graphic_vars.display = (Rectangle) {
        .x = GetScreenWidth() * 0.20f,
        .y = GetScreenHeight() * 0.03f,
        .width = GetScreenWidth() * 0.80f,
        .height = GetScreenHeight() * 0.80f
    };
    
    graphic_vars.screen_w = GetScreenWidth();
    graphic_vars.screen_h = GetScreenHeight();
    graphic_vars.mouse_cursor_sprite = LoadTexture("assets/sprites/mouse_cursor.png");
    if(graphic_vars.mouse_cursor_sprite.id <= 0)
    {
        printf("Error while loading mouse cursor sprite.\n");
        return -1;
    }
    graphic_vars.wall_node_sprite = LoadTexture("assets/sprites/wall_node.png");
    if(graphic_vars.wall_node_sprite.id <= 0)
    {
        printf("Error while loading wall node sprite.\n");
        return -1;
    }
    graphic_vars.player_spawn_point_sprite = LoadTexture("assets/sprites/player_node.png");
    if(graphic_vars.player_spawn_point_sprite.id <= 0)
    {
        printf("Error while loading wall node sprite.\n");
        return -1;
    }
    
    graphic_vars.style = 0;
    
    return 0;
}

int8_t SLUGmaker_GraphicUnload()
{
    if(graphic_vars.mouse_cursor_sprite.id > 0)
        UnloadTexture(graphic_vars.mouse_cursor_sprite);
    if(graphic_vars.wall_node_sprite.id > 0)
        UnloadTexture(graphic_vars.wall_node_sprite);
    if(graphic_vars.player_spawn_point_sprite.id > 0)
        UnloadTexture(graphic_vars.player_spawn_point_sprite);
    return 0;
}
