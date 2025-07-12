#include "display.h"
#include "geometry.h"

SLUGmaker_GraphicVariables graphic_vars;

int8_t SLUGmaker_GraphicInit()
{
    graphic_vars.display = (Rectangle) {
        .x = 0,
        .y = 0,
        .width = GetScreenWidth(),
        .height = GetScreenHeight()
    };
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
    return 0;
}

int8_t SLUGmaker_GraphicUnload()
{
    if(graphic_vars.mouse_cursor_sprite.id > 0)
        UnloadTexture(graphic_vars.mouse_cursor_sprite);
    if(graphic_vars.wall_node_sprite.id > 0)
        UnloadTexture(graphic_vars.wall_node_sprite);
    return 0;
}

SLUGmaker_camera SLUGmaker_DefaultCamera(SLUGmaker_map *map)
{
    SLUGmaker_camera cam;
    cam.map = map;
    cam.view_zone = (Rectangle) {
        .x = 0,
        .y = 0,
        .width = 1680,
        .height = 1050
    };
    cam.display = &(graphic_vars.display);
    cam.unzoom = 1.0;
    cam.ratiox = cam.display->width / cam.view_zone.width;
    cam.ratioy = cam.display->height / cam.view_zone.height;
    return cam;
}

float SLUGmaker_GetMousePosX(SLUGmaker_camera *cam)
{
    return (float) (cam->view_zone.x + (GetMouseX() * cam->view_zone.width) / cam->display->width);
}

float SLUGmaker_GetMousePosY(SLUGmaker_camera *cam)
{
    return (float) (cam->view_zone.y + (GetMouseY() * cam->view_zone.height) / cam->display->height);
}

int8_t SLUGmaker_DisplayUpdate(SLUGmaker_camera *cam)
{
    cam->display->width = GetScreenWidth();
    cam->display->height = GetScreenHeight();
    cam->ratiox = cam->display->width / cam->view_zone.width;
    cam->ratioy = cam->display->height / cam->view_zone.height;
    return 0;
}

int8_t SLUGmaker_DisplayMap(SLUGmaker_camera *cam)
{
    if(CheckCollisionRecs(cam->view_zone, cam->map->zone))
    {
        Rectangle d1 = GetCollisionRec(cam->view_zone, cam->map->zone);
        if(d1.x == cam->view_zone.x && d1.y == cam->view_zone.y && d1.width == cam->view_zone.width && d1.height == cam->view_zone.height)
            DrawTexturePro(cam->map->fixed_sprite,cam->view_zone,*(cam->display),(Vector2) {.x = 0, .y = 0},0,WHITE);     
        else
        {
            Rectangle d2 = (Rectangle) {
                .x = cam->display->x + (d1.x - cam->view_zone.x) * cam->ratiox,
                .y = cam->display->y + (d1.y - cam->view_zone.y) * cam->ratioy,
                .width = d1.width * cam->ratiox,
                .height = d1.height * cam->ratioy
            };
            DrawTexturePro(cam->map->fixed_sprite,d1,d2,(Vector2) {.x = 0, .y = 0},0,WHITE);
        }  
    }
    return 0;
}

int8_t SLUGmaker_DisplayWallNodes(SLUGmaker_camera *cam)
{
    Vector2 l1, l2;
    for(int16_t i = 0; i < MAX_WALL_NODES; ++i)
    {
        if(cam->map->wall_nodes[i].exists)
        {
            l1.x = (float) cam->map->wall_nodes[i].x;
            l1.y = (float) cam->map->wall_nodes[i].y;
            if(cam->map->wall_nodes[i].A_side != -1)
            {
                l2.x = (float) cam->map->wall_nodes[cam->map->wall_nodes[i].A_side].x;
                l2.y = (float) cam->map->wall_nodes[cam->map->wall_nodes[i].A_side].y;
                if(CheckCollisionLineRect(l1,l2,cam->view_zone,NULL))
                    DrawLineV(
                        (Vector2) {
                            .x = cam->display->x + (cam->map->wall_nodes[i].x - cam->view_zone.x) * cam->ratiox,
                            .y = cam->display->y + (cam->map->wall_nodes[i].y - cam->view_zone.y) * cam->ratioy
                        },
                        (Vector2) {
                            .x = cam->display->x + (cam->map->wall_nodes[cam->map->wall_nodes[i].A_side].x - cam->view_zone.x) * cam->ratiox,
                            .y = cam->display->y + (cam->map->wall_nodes[cam->map->wall_nodes[i].A_side].y - cam->view_zone.y) * cam->ratioy
                        }, BLUE);
            }

            if(cam->map->wall_nodes[i].B_side != -1)
            {
                l2.x = (float) cam->map->wall_nodes[cam->map->wall_nodes[i].B_side].x;
                l2.y = (float) cam->map->wall_nodes[cam->map->wall_nodes[i].B_side].y;
                if(CheckCollisionLineRect(l1,l2,cam->view_zone,NULL))
                    DrawLineV(
                        (Vector2) {
                            .x = cam->display->x + (cam->map->wall_nodes[i].x - cam->view_zone.x) * cam->ratiox,
                            .y = cam->display->y + (cam->map->wall_nodes[i].y - cam->view_zone.y) * cam->ratioy
                        },
                        (Vector2) {
                            .x = cam->display->x + (cam->map->wall_nodes[cam->map->wall_nodes[i].B_side].x - cam->view_zone.x) * cam->ratiox,
                            .y = cam->display->y + (cam->map->wall_nodes[cam->map->wall_nodes[i].B_side].y - cam->view_zone.y) * cam->ratioy
                        }, BLUE);
            }

            if(CheckCollisionPointRec((Vector2) {.x = cam->map->wall_nodes[i].x, .y = cam->map->wall_nodes[i].y}, cam->view_zone))
                DrawTexture(graphic_vars.wall_node_sprite,(cam->display->x + (cam->map->wall_nodes[i].x - cam->view_zone.x) * cam->ratiox) - graphic_vars.wall_node_sprite.width / 2,(cam->display->y + (cam->map->wall_nodes[i].y - cam->view_zone.y) * cam->ratioy) - graphic_vars.wall_node_sprite.height / 2,WHITE);
        }
    }
    return 0;
}

int8_t SLUGmaker_Display(SLUGmaker_camera *cam)
{
    BeginDrawing();
    ClearBackground(BLACK);
    
    if(SLUGmaker_DisplayMap(cam) == -1)
        return -1;
    
    if(SLUGmaker_DisplayWallNodes(cam) == -1)
        return -1;

    DrawTexture(graphic_vars.mouse_cursor_sprite,GetMouseX(),GetMouseY(),WHITE);
    DrawText(TextFormat("%f ; %f",(float) (cam->view_zone.x + (GetMouseX() / cam->ratiox)),(float) (cam->view_zone.y + (GetMouseY() / cam->ratioy))), 0, 0, 20,WHITE);
    EndDrawing();
    return 0;
}
