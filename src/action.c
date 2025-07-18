#include "action.h"

SLUGmaker_action_mode current_action = ACTION_MODE_NONE;

int8_t SLUGmaker_CameraUpdate(SLUGmaker_camera *cam)
{
    if(GetMouseWheelMove() > 0)
    {
        cam->view_zone.width /= 1.1;
        cam->view_zone.height /= 1.1;
        cam->unzoom /= 1.1;
        cam->ratiox = cam->display->width / cam->view_zone.width;
        cam->ratioy = cam->display->height / cam->view_zone.height;
    }
    else if(GetMouseWheelMove() < 0)
    {
        cam->view_zone.width *= 1.1;
        cam->view_zone.height *= 1.1;
        cam->unzoom *= 1.1;
        cam->ratiox = cam->display->width / cam->view_zone.width;
        cam->ratioy = cam->display->height / cam->view_zone.height;
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 v = GetMouseDelta();
        cam->view_zone.x -= v.x * cam->unzoom;
        cam->view_zone.y -= v.y * cam->unzoom;
    }

    return 0;
}

int16_t SLUGmaker_WallNodeUnderMouse(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    for(int16_t i = 0; i < MAX_WALL_NODES; ++i)
    {
        if(map->wall_nodes[i].exists)
        {
            if(CheckCollisionPointRec((Vector2) {.x = map->wall_nodes[i].x, .y = map->wall_nodes[i].y}, cam->view_zone))
            {
                float x = cam->display->x + (map->wall_nodes[i].x - cam->view_zone.x) * cam->ratiox;
                float y = cam->display->y + (map->wall_nodes[i].y - cam->view_zone.y) * cam->ratioy;
                if((GetMouseX() >= x - graphic_vars.wall_node_sprite.width / 2) && (GetMouseX() <= x + graphic_vars.wall_node_sprite.width / 2) && (GetMouseY() >= y - graphic_vars.wall_node_sprite.height / 2) && (GetMouseY() < y + graphic_vars.wall_node_sprite.height / 2))
                    return i;
            }
        }
    }
    return -1;
}

int8_t SLUGmaker_ChangeActionMode()
{
    if(IsKeyPressed(KEY_N))
    {
        current_action_mode = ACTION_MODE_NONE;
        return 0;
    }
    if(IsKeyPressed(KEY_BACKSPACE))
    {
        current_action_mode = ACTION_MODE_DELETE;
        return 0;
    }
    if(IsKeyPressed(KEY_W))
    {
        current_action_mode = ACTION_MODE_WALL;
        return 0;
    }
    return 0;
}

int8_t SLUGmaker_Action(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    switch(current_action_mode) //no break statements hihihohohaha
    {
        case ACTION_MODE_NONE:
            return 0;
        case ACTION_MODE_DELETE:
            return SLUGmaker_MapElementDelete(map,cam);
        case ACTION_MODE_WALL :
            return SLUGmaker_PlaceWallNode(map,cam);
        default:
            return 0;
    }   
    return 0;
}

int8_t SLUGmaker_MapElementDelete(SLUGmaker_map *map,SLUGmaker_camera *cam)
{
    int8_t r;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        r = SLUGmaker_WallNodeDelete(map,cam);
        if(r == -1)
            return -1;
        else if(r == 1)
            return 0;
    }
    return 0;
}

int8_t SLUGmaker_WallNodeDelete(SLUGmaker_map *map,SLUGmaker_camera *cam)
{
    int16_t w = SLUGmaker_WallNodeUnderMouse(map,cam);
    if(w != -1)
    {

        if(map->wall_nodes[w].A_side != -1)
        {
            if(map->wall_nodes[map->wall_nodes[w].A_side].A_side == w)
                map->wall_nodes[map->wall_nodes[w].A_side].A_side = -1;
            else if(map->wall_nodes[map->wall_nodes[w].A_side].B_side == w)
                map->wall_nodes[map->wall_nodes[w].A_side].B_side = -1;
            else
            {
                printf("This wall positionning should not be possible...\n");
                return -1;
            }
        }

        if(map->wall_nodes[w].B_side != -1)
        {
            if(map->wall_nodes[map->wall_nodes[w].B_side].A_side == w)
                map->wall_nodes[map->wall_nodes[w].B_side].A_side = -1;
            else if(map->wall_nodes[map->wall_nodes[w].B_side].B_side == w)
                map->wall_nodes[map->wall_nodes[w].B_side].B_side = -1;
            else
            {
                printf("This wall positionning should not be possible...\n");
                return -1;
            }
        }

        map->wall_nodes[w].A_side = -1;
        map->wall_nodes[w].B_side = -1; 
        map->wall_nodes[w].x = 0;
        map->wall_nodes[w].y = 0;
        map->wall_nodes[w].exists = false;
        map->current_wall_index = w;
        map->wall_node_nb--;
        return 1;
    }
    return 0;
}

int8_t SLUGmaker_PlaceWallNode(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_SHIFT))
    {
        if(!map->wall_line_mode)
        {
            if(map->wall_node_nb >= MAX_WALL_NODES)
                printf("Too many wall nodes!\n");
            else
            {
                int16_t w = SLUGmaker_WallNodeUnderMouse(map, cam);
                if(w == -1)
                {
                    Vector2 p = (Vector2) {
                        .x = cam->view_zone.x + (GetMouseX() / cam->ratiox),
                        .y = cam->view_zone.y + (GetMouseY() / cam->ratioy)
                    };
                    if(CheckCollisionPointRec(p, map->zone))
                    {
                        map->wall_nodes[map->current_wall_index].exists = true;  
                        map->wall_nodes[map->current_wall_index].x = roundf(p.x);
                        map->wall_nodes[map->current_wall_index].y = roundf(p.y);

                        while(map->wall_nodes[map->current_wall_index].exists) //normally ok
                        {
                            map->current_wall_index++;
                            if(map->current_wall_index >= MAX_WALL_NODES)
                                map->current_wall_index = 0;
                        }
                        map->wall_node_nb++; 
                    }    
                }
                else if(map->wall_nodes[w].A_side == -1 || map->wall_nodes[w].B_side == -1)
                {
                    printf("Selected node %d\n",w);
                    map->wall_line_mode = true;
                    map->wall_line_origin_index = w;    
                }
                else
                    printf("A wall node can't be bound by more than 2 others.\n");
            }
            
        }
        else
        {
            int16_t w = SLUGmaker_WallNodeUnderMouse(map, cam);
            if(w != -1 && w != map->wall_line_origin_index) 
            {
                printf("Selected node %d\n",w);
                if(map->wall_nodes[w].A_side == -1 || map->wall_nodes[w].B_side == -1)
                {
                    if(map->wall_nodes[w].A_side != map->wall_line_origin_index && map->wall_nodes[w].B_side != map->wall_line_origin_index)
                    {
                        if(map->wall_nodes[w].A_side == -1)
                            map->wall_nodes[w].A_side = map->wall_line_origin_index;
                        else if(map->wall_nodes[w].B_side == -1)
                            map->wall_nodes[w].B_side = map->wall_line_origin_index;

                        if(map->wall_nodes[map->wall_line_origin_index].A_side == -1)
                            map->wall_nodes[map->wall_line_origin_index].A_side = w;
                        else if(map->wall_nodes[map->wall_line_origin_index].B_side == -1)
                            map->wall_nodes[map->wall_line_origin_index].B_side = w;
                    }
                    else
                        printf("Cannot do double wall bind.\n"); 
                }
                else
                    printf("A wall node can't be bound by more than 2 others.\n");  
                
            }

            map->wall_line_mode = false;
            map->wall_line_origin_index = -1;

        }
        
    }
    else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT))
    {
        if(map->wall_node_move_mode == -1)
        {
            int16_t w = SLUGmaker_WallNodeUnderMouse(map, cam);
            map->wall_node_move_mode = w;
            printf("Selected node %d\n",w);
        }
        if(map->wall_node_move_mode != -1)
        {
            Vector2 p = (Vector2) {
                .x = cam->view_zone.x + (GetMouseX() / cam->ratiox),
                .y = cam->view_zone.y + (GetMouseY() / cam->ratioy)
            };
            if(CheckCollisionPointRec(p, map->zone))
            {
                map->wall_nodes[map->wall_node_move_mode].x = roundf(p.x);
                map->wall_nodes[map->wall_node_move_mode].y = roundf(p.y);
            }
        }
    }
    else
        map->wall_node_move_mode = -1;
    return 0;
}
