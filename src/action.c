#include "action.h"

SLUGmaker_action current_action = ACTION_NONE;

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

int16_t SLUGmaker_NodeUnderMouse(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    for(int16_t i = 0; i < MAX_WALL_NODES; ++i)
    {
        if(map->wall_nodes[i].next_index != -2)
        {
            if(CheckCollisionPointRec((Vector2) {.x = map->wall_nodes[i].x, .y = map->wall_nodes[i].y}, cam->view_zone))
            {
                float x = cam->display->x + (map->wall_nodes[i].x - cam->view_zone.x) * cam->ratiox;
                float y = cam->display->y + (map->wall_nodes[i].y - cam->view_zone.y) * cam->ratioy;
                if((GetMouseX() >= x - 6) && (GetMouseX() <= x + 6) && (GetMouseY() >= y - 6) && (GetMouseY() <) y + 6))
                    return i;
            }
        }
    }
    return -1;
}

int8_t SLUGmaker_ChangeAction()
{
    if(IsKeyPressed(KEY_N))
    {
        current_action = ACTION_NONE;
        return 0;
    }
    if(IsKeyPressed(KEY_W))
    {
        current_action = ACTION_WALL;
        return 0;
    }
    return 0;
}

int8_t SLUGmaker_Place(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    switch(current_action) //no break statements hihihohohaha
    {
        case ACTION_NONE:
            return 0;
        case ACTION_WALL :
            return SLUGmaker_PlaceWallNode(map,cam);
        default:
            return 0;
    }   
    return 0;
}

int8_t SLUGmaker_PlaceWallNode(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
    if(IsKeyPressed(MOUSE_BUTTON_LEFT))
    {
        if(map->wall_node_nb >= MAX_WALL_NODES)
            printf("Too many wall nodes!\n");
        else
        {
            if((GetMouseX() >= map->wall_nodes[chain_start].x - 6) && (GetMouseX() <= map->wall_nodes[chain_start].x + 6) && (GetMouseY() >= map->wall_nodes[chain_start].y - 6) && ())
            //  else

            

            while(map->wall_nodes[current_index].next_index != -1) //normally it is ok
            {
                current_index++;
                if(current_index >= MAX_WALL_NODES)
                    current_index = 0;
            }
            map->wall_node_nb++;
        }
    }
    return 0;
}
