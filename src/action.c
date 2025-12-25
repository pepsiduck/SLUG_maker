#include "action.h"
#include "menus.h"
#include "defines.h"

//---camera
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

SLUGmaker_action_mode previous_action = ACTION_MODE_NONE;
SLUGmaker_action_mode current_action = ACTION_MODE_NONE;

int8_t SLUGmaker_CameraUpdate(SLUGmaker_camera *cam, bool window_changed)
{
    if(cam == NULL)
        return -1;
                
    bool zoom = false;    
        
    if(GetMouseWheelMove() > 0 || IsKeyPressed(KEY_U))
    {
        cam->unzoom /= 1.1;
        zoom = true;
    }
    else if(GetMouseWheelMove() < 0 || IsKeyPressed(KEY_I))
    {
        cam->unzoom *= 1.1;
        zoom = true;
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 v = GetMouseDelta();
        cam->view_zone.x -= v.x * cam->unzoom;
        cam->view_zone.y -= v.y * cam->unzoom;
    }
    
    if(zoom || window_changed)
    {
    	cam->view_zone.width = graphic_vars.screen_w * cam->unzoom;
		cam->view_zone.height = graphic_vars.screen_h * cam->unzoom;
		cam->ratiox = cam->display->width / cam->view_zone.width;
		cam->ratioy = cam->display->height / cam->view_zone.height;
    }

    return 0;
}

//---action handling
int8_t SLUGmaker_ChangeActionMode(SLUGmaker_map *map)
{
    previous_action = current_action;
    if(IsKeyPressed(KEY_N) || general_menus.actionMenu.modes[0].pressed)
        current_action = ACTION_MODE_NONE;
    else if(IsKeyPressed(KEY_BACKSPACE) || general_menus.actionMenu.modes[1].pressed)
        current_action = ACTION_MODE_DELETE;
    else if(IsKeyPressed(KEY_W) || general_menus.actionMenu.modes[2].pressed)
        current_action = ACTION_MODE_WALL;
    else if(IsKeyPressed(KEY_P) || general_menus.actionMenu.modes[3].pressed)
        current_action = ACTION_MODE_PLAYER;
    else if((IsKeyPressed(KEY_S) && !IsKeyDown(KEY_LEFT_CONTROL))|| general_menus.actionMenu.modes[4].pressed)
        current_action = ACTION_MODE_SPRITE;
	
	//quit
    if(previous_action != current_action)
    {
        switch(previous_action)
        {
            case ACTION_MODE_WALL:
                SLUGmaker_WallModeQuit(map);
                break;
            default:
                break;
        }
    }

    return 0;
}

int8_t SLUGmaker_Action(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    if(map == NULL || cam == NULL)
        return -1;
    switch(current_action) //no break statements hihihohohaha
    {
        case ACTION_MODE_NONE:
            return 0;
        case ACTION_MODE_DELETE:
            return SLUGmaker_MapElementDelete(map,cam);
        case ACTION_MODE_WALL :
            return SLUGmaker_WallMode(map,cam);
        case ACTION_MODE_PLAYER :
        	return SLUGmaker_PlayerSpawnPointMove(map,cam);
        default:
            return 0;
    }   
    return 0;
}

//---delete handling

int8_t SLUGmaker_MapElementDelete(SLUGmaker_map *map,SLUGmaker_camera *cam)
{
    if(map == NULL || cam == NULL)
        return -1;
    int8_t r;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        r = SLUGmaker_WallDelete(map,cam);
        if(r == -1)
            return -1;
        else if(r == 1)
            return 0;
    }
    return 0;
}

int8_t SLUGmaker_WallDelete(SLUGmaker_map *map,SLUGmaker_camera *cam)
{
    if(map == NULL || cam == NULL)
        return -1;
    if(map->wall_line_mode)
        return -1;
    int16_t w = SLUGmaker_WallUnderMouseA(map,cam);
    if(w >= -1)
    {
        int16_t count = 1;
        SLUGmaker_SegmentExtended *seg = &(map->walls[w]);
        seg->exists = false;
        seg = seg->next;

        while(seg != &(map->walls[w]))
        {
            if(seg == NULL)
                return -1;
            seg->exists = false;
            count += 1;
            seg = seg->next;
        }

        map->current_wall_index = w;
        map->wall_nb -= count;

        if(map->wall_nb < 0)
        {
            printf("Bruh there is a negative number of walls.");
            return -1;
        }

        return 1;
    }
    return 0;
}


//---wall
int16_t SLUGmaker_WallUnderMouseA(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    if(map == NULL || cam == NULL)
        return -1;
    if(map->wall_nb == 0)
        return -1;
    for(int16_t i = 0; i < MAX_WALLS_NB; ++i)
    {
        if(map->walls[i].exists)
        {
            if(CheckCollisionPointRec(map->walls[i].A, cam->view_zone))
            {
                float x = cam->display->x + (map->walls[i].A.x - cam->view_zone.x) * cam->ratiox;
                float y = cam->display->y + (map->walls[i].A.y - cam->view_zone.y) * cam->ratioy;
                if((GetMouseX() >= x - graphic_vars.wall_node_sprite.width / 2) && (GetMouseX() <= x + graphic_vars.wall_node_sprite.width / 2) && (GetMouseY() >= y - graphic_vars.wall_node_sprite.height / 2) && (GetMouseY() < y + graphic_vars.wall_node_sprite.height / 2))
                    return i;
            }
        }
    }
    return -1;
}

int16_t SLUGmaker_WallUnderMouseB(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    if(map == NULL || cam == NULL)
        return -1;
    if(map->wall_nb == 0)
        return -1;
    for(int16_t i = 0; i < MAX_WALLS_NB; ++i)
    {
        if(map->walls[i].exists)
        {
            if(CheckCollisionPointRec(map->walls[i].B, cam->view_zone))
            {
                float x = cam->display->x + (map->walls[i].B.x - cam->view_zone.x) * cam->ratiox;
                float y = cam->display->y + (map->walls[i].B.y - cam->view_zone.y) * cam->ratioy;
                if((GetMouseX() >= x - graphic_vars.wall_node_sprite.width / 2) && (GetMouseX() <= x + graphic_vars.wall_node_sprite.width / 2) && (GetMouseY() >= y - graphic_vars.wall_node_sprite.height / 2) && (GetMouseY() < y + graphic_vars.wall_node_sprite.height / 2))
                    return i;
            }
        }
    }
    return -1;
}

int8_t SLUGmaker_WallMode(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    if(map == NULL || cam == NULL)
        return -1;

    if(!map->wall_line_mode && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        if(map->wall_move_mode == -1)
            map->wall_move_mode = SLUGmaker_WallUnderMouseB(map, cam);
        if(map->wall_move_mode != -1)
            return SLUGmaker_MoveWall(map, cam);
    }
    else
        map->wall_move_mode = -1;  

    return SLUGmaker_PlaceNewWall(map,cam);
}

int8_t SLUGmaker_MoveWall(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    if(map == NULL || cam == NULL)
        return -1;
    if(map->wall_move_mode < 0 || map->wall_move_mode >= MAX_WALLS_NB)
        return -1;
    if(!map->walls[map->wall_move_mode].exists || map->wall_line_mode || map->walls[map->wall_move_mode].next == NULL)
        return -1;
    if(!map->walls[map->wall_move_mode].next->exists)     
        return -1;
        

    
    Vector2 mouse = (Vector2) {.x = (float) GetMouseX(), .y = (float) GetMouseY()};
	
	if(!CheckCollisionPointRec(mouse, *(cam->display)))
		return 0;

    Vector2 p = (Vector2) {
            .x = roundf(cam->view_zone.x + ((mouse.x - cam->display->x) / cam->ratiox)),
            .y = roundf(cam->view_zone.y + ((mouse.y - cam->display->y) / cam->ratioy))
    };

    if(CheckCollisionPointRec(p, map->zone))
    {

        //B segment move
        map->walls[map->wall_move_mode].B = p;
        map->walls[map->wall_move_mode].middlepoint = Vector2Scale(Vector2Add(map->walls[map->wall_move_mode].A,map->walls[map->wall_move_mode].B), 0.50);
        if(map->walls[map->wall_move_mode].B.x == map->walls[map->wall_move_mode].A.x && map->walls[map->wall_move_mode].B.y == map->walls[map->wall_move_mode].A.y)
            map->walls[map->wall_move_mode].normal = (Vector2) {.x = 0, .y = 1 };
        else
        {
            map->walls[map->wall_move_mode].normal = Vector2Normalize( (Vector2) {
                .x = map->walls[map->wall_move_mode].A.y - map->walls[map->wall_move_mode].B.y,
                .y = map->walls[map->wall_move_mode].B.x - map->walls[map->wall_move_mode].A.x
            });
        }
        

        //A segment move
        SLUGmaker_SegmentExtended *seg = map->walls[map->wall_move_mode].next;
        seg->A = p;
        seg->middlepoint = Vector2Scale(Vector2Add(seg->A,seg->B), 0.50);
        if(seg->B.x == seg->A.x && seg->B.y == seg->A.y)
            seg->normal = (Vector2) {.x = 0, .y = 1 };
        else
        {
            seg->normal = Vector2Normalize( (Vector2) {
                .x = seg->A.y - seg->B.y,
                .y = seg->B.x - seg->A.x
            });
        }

    }
    return 0;
}

int8_t SLUGmaker_PlaceNewWall(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
	if(map == NULL || cam == NULL)
		return -1;
	if(map->wall_move_mode >= 0)
		return -1;

	Vector2 mouse = (Vector2) {.x = (float) GetMouseX(), .y = (float) GetMouseY()};
	
	if(!CheckCollisionPointRec(mouse, *(cam->display)))
		return 0;

    Vector2 p = (Vector2) {
            .x = roundf(cam->view_zone.x + ((mouse.x - cam->display->x) / cam->ratiox)),
            .y = roundf(cam->view_zone.y + ((mouse.y - cam->display->y) / cam->ratioy))
    };
    if(!CheckCollisionPointRec(p, map->zone))
        return 0;

    if(map->wall_line_mode)
    {

        if(!map->walls[map->current_wall_index].exists)
        {
            printf("Currently placed wall doesn't exist ???\n");
            return -1;
        }

        
        map->walls[map->current_wall_index].B = p;
        map->walls[map->current_wall_index].middlepoint = Vector2Scale(Vector2Add(map->walls[map->current_wall_index].A,map->walls[map->current_wall_index].B), 0.50);
        if(map->walls[map->current_wall_index].B.x == map->walls[map->current_wall_index].A.x && map->walls[map->current_wall_index].B.y == map->walls[map->current_wall_index].A.y)
            map->walls[map->current_wall_index].normal = (Vector2) {.x = 0, .y = 1 };
        else
        {
            map->walls[map->current_wall_index].normal = Vector2Normalize( (Vector2) {
                .x = map->walls[map->current_wall_index].A.y - map->walls[map->current_wall_index].B.y,
                .y = map->walls[map->current_wall_index].B.x - map->walls[map->current_wall_index].A.x
            });
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if(SLUGmaker_WallUnderMouseA(map,cam) == map->wall_line_origin_index)
            {
                if(&(map->walls[map->current_wall_index]) != map->walls[map->wall_line_origin_index].next)
                {
                    map->walls[map->current_wall_index].B = map->walls[map->wall_line_origin_index].A;
                    map->walls[map->current_wall_index].middlepoint = Vector2Scale(Vector2Add(map->walls[map->current_wall_index].A,map->walls[map->current_wall_index].B), 0.50);
                    if(map->walls[map->current_wall_index].B.x == map->walls[map->current_wall_index].A.x && map->walls[map->current_wall_index].B.y == map->walls[map->current_wall_index].A.y)
                        map->walls[map->current_wall_index].normal = (Vector2) {.x = 0, .y = 1 };
                    else
                    {
                        map->walls[map->current_wall_index].normal = Vector2Normalize( (Vector2) {
                            .x = map->walls[map->current_wall_index].A.y - map->walls[map->current_wall_index].B.y,
                            .y = map->walls[map->current_wall_index].B.x - map->walls[map->current_wall_index].A.x
                        });
                    }
                    map->walls[map->current_wall_index].next = &(map->walls[map->wall_line_origin_index]);

                    if(map->wall_nb >= MAX_WALLS_NB)
                        map->current_wall_index = -1;
                    else
                    {
                        while(map->walls[map->current_wall_index].exists)
                        {
                            map->current_wall_index += 1;
                            if(map->current_wall_index >= MAX_WALLS_NB)
                                map->current_wall_index = 0;
                        }
                    }

                    map->wall_line_mode = false;
                }
            }
            else if(map->wall_nb < MAX_WALLS_NB)
            {
                int16_t w = map->current_wall_index;
                while(map->walls[w].exists)
                {
                    w += 1;
                    if(w >= MAX_WALLS_NB)
                        w = 0;
                }

                map->walls[map->current_wall_index].next = &(map->walls[w]);

                map->walls[w].A = map->walls[map->current_wall_index].B;
                map->walls[w].B = map->walls[w].A;
                map->walls[w].normal = (Vector2) {.x = 0, .y = 1};
                map->walls[w].middlepoint = map->walls[w].A;
                map->walls[w].next = NULL;
                map->walls[w].exists = true;

                map->current_wall_index = w;
                map->wall_nb += 1;
            }
            else
                printf("Too many walls\n");
        }

    }
    else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if(map->wall_nb < MAX_WALLS_NB - 2)
        {
            if(SLUGmaker_WallUnderMouseA(map,cam) < 0)
            {
                if(map->walls[map->current_wall_index].exists)  
                {
                    printf("Error : Trying to overwrite existing wall.\n");
                    return -1;
                }   

                map->walls[map->current_wall_index].A = p;
                map->walls[map->current_wall_index].B = p;
                map->walls[map->current_wall_index].normal = (Vector2) {.x = 0, .y = 1};
                map->walls[map->current_wall_index].middlepoint = p;
                map->walls[map->current_wall_index].next = NULL;
                map->walls[map->current_wall_index].exists = true;   

                map->wall_line_origin_index = map->current_wall_index;
                map->wall_line_mode = true;
                map->wall_nb += 1;
            }
        }
        else
            printf("Too many walls\n");
    }

    

    return 0;   
}

int8_t SLUGmaker_WallModeQuit(SLUGmaker_map *map)
{
    if(map == NULL)
        return -1;
    if(map->wall_line_mode)
    {
        SLUGmaker_SegmentExtended *seg = &(map->walls[map->wall_line_origin_index]);
        seg->exists = false;
        seg = seg->next;

        int16_t count = 1;

        while(seg != NULL && seg != &(map->walls[map->wall_line_origin_index]))
        {
            seg->exists = false;
            seg = seg->next;
            count += 1;
        }
        
        map->wall_nb -= count;
        map->current_wall_index = map->wall_line_origin_index;
        map->wall_line_mode = false;

    }
    
    map->wall_move_mode = -1;
    return 0;
}

//--- player
int8_t SLUGmaker_PlayerSpawnPointMove(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
	if(map == NULL || cam == NULL)
		return -1;
		
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 msb = (Vector2) {.x = (float) GetMouseX(), .y = (float) GetMouseY()};
		if(CheckCollisionPointRec(msb,graphic_vars.display))
		{
			msb = (Vector2) {.x = SLUGmaker_GetMousePosX(cam), .y = SLUGmaker_GetMousePosY(cam)};
			if(CheckCollisionPointRec(msb,map->zone))
				map->player_spawn_point = msb;
		}
	}	
		
	return 0;
}
