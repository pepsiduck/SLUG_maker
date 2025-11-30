#include "display.h"
#include "geometry.h"
#include "defines.h"

int8_t SLUGmaker_DisplayUpdate(float factor_x, float factor_y, SLUGmaker_camera *cam)
{	
	RectangleMultiply(cam->display,factor_x,factor_y);

	cam->ratiox = cam->display->width / cam->view_zone.width;
	cam->ratioy = cam->display->height / cam->view_zone.height;
		
    return 0;
}

int8_t SLUGmaker_DisplaySprite(SLUGmaker_camera *cam, int16_t index)
{
    if(cam == NULL)
        return -1;

    if(index >= cam->map->sprite_nb)
    {
        printf("Sprite index out of range.\n");
        return -1;
    }

    if(cam->map->loaded_sprites[cam->map->map_sprites[index].sprite_index].id <= 0)
    {
        printf("Trying to display a sprite that doesn't exist");
        return -1;
    }

    Rectangle d1 = GetCollisionRec(cam->view_zone, cam->map->map_sprites[index].zone);
    float w = (float) cam->map->loaded_sprites[cam->map->map_sprites[index].sprite_index].width;
    float h = (float) cam->map->loaded_sprites[cam->map->map_sprites[index].sprite_index].height;

    if(d1.x == cam->view_zone.x && d1.y == cam->view_zone.y && d1.width == cam->view_zone.width && d1.height == cam->view_zone.height)
    {
        Rectangle d2 = (Rectangle) {
            .x = ((cam->map->map_sprites[index].zone.x - d1.x) * w)/cam->map->map_sprites[index].zone.width,
            .y = ((cam->map->map_sprites[index].zone.y - d1.y) * h)/cam->map->map_sprites[index].zone.height,
            .width = (d1.width * w)/cam->map->map_sprites[index].zone.width,
            .height = (d1.height * h)/cam->map->map_sprites[index].zone.height
        };
        DrawTexturePro(cam->map->loaded_sprites[cam->map->map_sprites[index].sprite_index], d2,*(cam->display), (Vector2) {.x = 0, .y = 0}, 0, WHITE);  
    }
    else
    {
        Rectangle src = (Rectangle) {
            .x = 0.0,
            .y = 0.0,
            .width = w,
            .height = h
        };
        Rectangle dest = (Rectangle) {
            .x = cam->display->x + (d1.x - cam->view_zone.x) * cam->ratiox,
            .y = cam->display->y + (d1.y - cam->view_zone.y) * cam->ratioy,
            .width = d1.width * cam->ratiox,
            .height = d1.height * cam->ratioy
        };
        DrawTexturePro(cam->map->loaded_sprites[cam->map->map_sprites[index].sprite_index],src,dest,(Vector2) {.x = 0, .y = 0},0,WHITE);
    }  

    return 0;
}

int8_t SLUGmaker_DisplaySprites(SLUGmaker_camera *cam)
{
    if(cam == NULL)
        return -1;

    for(int16_t i = 0; i < cam->map->sprite_nb; ++i)
    {
        if(SLUGmaker_DisplaySprite(cam, i) == -1)
            return -1;
    }

    return 0;
}

int8_t SLUGmaker_DisplayWalls(SLUGmaker_camera *cam)
{
	if(cam == NULL)
		return -1;
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

int8_t SLUGmaker_DisplayPlayerSpawnPoint(SLUGmaker_camera *cam)
{
	if(cam == NULL)
		return -1;
	if(CheckCollisionPointRec(cam->map->player_spawn_point,cam->view_zone))
	{
		DrawTexture(graphic_vars.player_spawn_point_sprite,(cam->display->x + (cam->map->player_spawn_point.x - cam->view_zone.x) * cam->ratiox) - graphic_vars.player_spawn_point_sprite.width / 2,(cam->display->y + (cam->map->player_spawn_point.y - cam->view_zone.y) * cam->ratioy) - graphic_vars.player_spawn_point_sprite.height / 2,WHITE);
	}
	return 0;
}

int8_t SLUGmaker_DisplayMap(SLUGmaker_camera *cam)
{
	if(cam == NULL)
		return -1;
    if(CheckCollisionRecs(cam->view_zone, cam->map->zone))
    {
        Rectangle d1 = GetCollisionRec(cam->view_zone, cam->map->zone);
        if(d1.x == cam->view_zone.x && d1.y == cam->view_zone.y && d1.width == cam->view_zone.width && d1.height == cam->view_zone.height)
            DrawRectanglePro(*(cam->display), (Vector2) {.x = 0, .y = 0}, 0, WHITE);     
        else
        {
            Rectangle d2 = (Rectangle) {
                .x = cam->display->x + (d1.x - cam->view_zone.x) * cam->ratiox,
                .y = cam->display->y + (d1.y - cam->view_zone.y) * cam->ratioy,
                .width = d1.width * cam->ratiox,
                .height = d1.height * cam->ratioy
            };
            DrawRectanglePro(d2, (Vector2) {.x = 0, .y = 0}, 0, WHITE);
        }  
    }
    return 0;
}

int8_t SLUGmaker_Display(SLUGmaker_camera *cam)
{
	if(cam == NULL)
		return -1;
    
    if(SLUGmaker_DisplayMap(cam) == -1)
        return -1;
    
    if(SLUGmaker_DisplaySprites(cam) == -1)
        return -1;

    if(SLUGmaker_DisplayWalls(cam) == -1)
        return -1;
        
    if(SLUGmaker_DisplayPlayerSpawnPoint(cam) == -1)
    	return -1;

    DrawTexture(graphic_vars.mouse_cursor_sprite,GetMouseX(),GetMouseY(),WHITE);
    
    Vector2 mouse = {.x = GetMouseX(), .y = GetMouseY()};
    
    
    if(CheckCollisionPointRec(mouse, *(cam->display)))
    	DrawText(TextFormat("%f ; %f",SLUGmaker_GetMousePosX(cam),SLUGmaker_GetMousePosY(cam)), 0, (float) (graphic_vars.screen_h) * 0.30f, 20,GREEN);
    
   
    return 0;
}
