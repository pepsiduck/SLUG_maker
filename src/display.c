#include "display.h"
#include "geometry.h"

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
    return (float) (cam->view_zone.x + ((GetMouseX() - cam->display->x) * cam->view_zone.width) / cam->display->width);
}

float SLUGmaker_GetMousePosY(SLUGmaker_camera *cam)
{
    return (float) (cam->view_zone.y + ((GetMouseY() - cam->display->y) * cam->view_zone.height) / cam->display->height);
}

int8_t SLUGmaker_DisplayUpdate(float factor_x, float factor_y, SLUGmaker_camera *cam)
{
		
	cam->display->x *= factor_x;
	cam->display->y *= factor_y;
	cam->display->width *= factor_x;
	cam->display->height *= factor_y;

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

int8_t SLUGmaker_DisplayWalls(SLUGmaker_camera *cam)
{
    for(int16_t i = 0; i < MAX_WALLS_NB; ++i)
    {
        if(cam->map->walls[i].exists)
        {
            if(CheckCollisionLineRect(cam->map->walls[i].A,cam->map->walls[i].B,cam->view_zone,NULL))
            {
                DrawLineV(
                    (Vector2) {
                        .x = cam->display->x + (cam->map->walls[i].A.x - cam->view_zone.x) * cam->ratiox,
                        .y = cam->display->y + (cam->map->walls[i].A.y - cam->view_zone.y) * cam->ratioy
                    },
                    (Vector2) {
                        .x = cam->display->x + (cam->map->walls[i].B.x - cam->view_zone.x) * cam->ratiox,
                        .y = cam->display->y + (cam->map->walls[i].B.y - cam->view_zone.y) * cam->ratioy
                    }, BLUE);
            }
            if(CheckCollisionPointRec(cam->map->walls[i].middlepoint, cam->view_zone))
            {
                DrawLineV(
                    (Vector2) {
                        .x = cam->display->x + (cam->map->walls[i].middlepoint.x - cam->view_zone.x) * cam->ratiox,
                        .y = cam->display->y + (cam->map->walls[i].middlepoint.y - cam->view_zone.y) * cam->ratioy
                    },
                    (Vector2) {
                        .x = cam->display->x + (cam->map->walls[i].middlepoint.x - cam->view_zone.x) * cam->ratiox + 20 * cam->map->walls[i].normal.x,
                        .y = cam->display->y + (cam->map->walls[i].middlepoint.y - cam->view_zone.y) * cam->ratioy + 20 * cam->map->walls[i].normal.y
                    }, BLUE);
            }
            
            if(cam->map->wall_line_mode && i == cam->map->current_wall_index)
            {
                if(CheckCollisionPointRec(cam->map->walls[i].B, cam->view_zone))
                {
                    DrawTexture(graphic_vars.wall_node_sprite,(cam->display->x + (cam->map->walls[i].B.x - cam->view_zone.x) * cam->ratiox) - graphic_vars.wall_node_sprite.width / 2,(cam->display->y + (cam->map->walls[i].B.y - cam->view_zone.y) * cam->ratioy) - graphic_vars.wall_node_sprite.height / 2,WHITE);
                }    
            }
            if(CheckCollisionPointRec(cam->map->walls[i].A, cam->view_zone))
            {
                DrawTexture(graphic_vars.wall_node_sprite,(cam->display->x + (cam->map->walls[i].A.x - cam->view_zone.x) * cam->ratiox) - graphic_vars.wall_node_sprite.width / 2,(cam->display->y + (cam->map->walls[i].A.y - cam->view_zone.y) * cam->ratioy) - graphic_vars.wall_node_sprite.height / 2,WHITE);
            }
        }
    }
    return 0;
}

int8_t SLUGmaker_Display(SLUGmaker_camera *cam)
{
    
    if(SLUGmaker_DisplayMap(cam) == -1)
        return -1;
    
    if(SLUGmaker_DisplayWalls(cam) == -1)
        return -1;

    DrawTexture(graphic_vars.mouse_cursor_sprite,GetMouseX(),GetMouseY(),WHITE);
    
    Vector2 mouse = {.x = GetMouseX(), .y = GetMouseY()};
    
    
    if(CheckCollisionPointRec(mouse, *(cam->display)))
    	DrawText(TextFormat("%f ; %f",SLUGmaker_GetMousePosX(cam),SLUGmaker_GetMousePosY(cam)), 0, (float) (graphic_vars.screen_h) * 0.04f, 20,GREEN);
    
   
    return 0;
}
