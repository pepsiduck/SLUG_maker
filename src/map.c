#include "map.h"
#include "geometry.h"

SLUGmaker_map* SLUGmaker_NewMap(const char *filename)
{
    SLUGmaker_map *map = (SLUGmaker_map *) malloc(sizeof(SLUGmaker_map));
    if(map == NULL)
    {
        printf("Malloc Error in map allocation.\n");
        return NULL;
    }
    map->fixed_sprite = LoadTexture(filename);
    if(map->fixed_sprite.id <= 0)
    {
        printf("Error while loading new map sprite.\n");
        free(map);
        return NULL;
    }
    SetTextureWrap(map->fixed_sprite, 1);
    map->zone.width = map->fixed_sprite.width;
    map->zone.height = map->fixed_sprite.height;
    map->zone.x = 0;
    map->zone.y = 0;
    for(int16_t i = 0; i < MAX_WALL_NODES; ++ i)
        map->wall_nodes[i] = (SLUGmaker_wall_node) {
            .x = 0, 
            .y = 0, 
            .exists = false,
            .A_side = -1,
            .B_side = -1
        };
    map->current_wall_index = 0;
    map->wall_node_nb = 0;
    map->wall_line_mode = false;
    map->wall_line_origin_index = -1;
    map->wall_node_move_mode = -1;
    return map;
}

SLUGmaker_map* SLUGmaker_LoadMap(const char *loadMap)
{
    return NULL; //TODO
}

void SLUGmaker_UnloadMap(SLUGmaker_map *map)
{
    if(map != NULL)
    {
        if(map->fixed_sprite.id > 0)
            UnloadTexture(map->fixed_sprite);
        free(map);
    }
}


/*All checks must be made before*/
void SLUGmaker_TranslateArcs(SLUGmaker_map *map, SLUG_SegmentExtended *arcs, int16_t arcs_size, int16_t w, int16_t *segcount)
{
    
}

/*
    Returns -1 if invalid (NULL ptr or map->wall_node_nb < 3)
    Returns 0 if at least one of the arcs are not cyclic 
    Returns the number of arcs if everything is well
*/
int16_t SLUGmaker_GetArcs(SLUGmaker_map *map, SLUG_SegmentExtended *arcs, int16_t arcs_size)
{
    if(arcs == NULL || map->wall_node_nb < 3)
        return -1;
    int16_t arcs_check[MAX_WALL_NODES];
    for(uint16_t i = 0; i < MAX_WALL_NODES; ++i)
        arcs_check[i] = 0;
    int16_t count = 0;
    int16_t segcount = 0;
    for(uint16_t i = 0; i < MAX_WALL_NODES; ++i)
    {
        if(map->wall_nodes[i].exists && arcs_check[i] == 0)
        {
            //check if valid + convex
            int32_t cross;
            int16_t w;
            int16_t w_prev = i; 
            if(map->wall_nodes[i].A_side != -1 && map->wall_nodes[i].B_side != -1 && map->wall_nodes[i].A_side != i && map->wall_nodes[i].B_side != i)
            {
                cross = Vector2CrossProduct(map->wall_nodes[map->wall_nodes[i].A_side].x - map->wall_nodes[i].x, map->wall_nodes[map->wall_nodes[i].A_side].y - map->wall_nodes[i].y, map->wall_nodes[map->wall_nodes[i].B_side].x - map->wall_nodes[i].x, map->wall_nodes[map->wall_nodes[i].B_side].y - map->wall_nodes[i].y);
                w = map->wall_nodes[i].A_side;
            }
            else
                return 0;

            if(segcount >= arcs_size)
                return -1;

            
            
            segcount++;

            count++;
            arcs_check[i] = count;
            while(w != i)
            {
                if(map->wall_nodes[w].A_side == -1 || map->wall_nodes[w].B_side == -1)
                    return 0;
                arcs_check[w] = count;
                if(map->wall_nodes[w].A_side != w_prev)
                {
                    int32_t curr = Vector2CrossProduct(map->wall_nodes[map->wall_nodes[w].A_side].x - map->wall_nodes[w].x, map->wall_nodes[map->wall_nodes[w].A_side].y - map->wall_nodes[w].y, map->wall_nodes[w_prev].x - map->wall_nodes[w].x, map->wall_nodes[w_prev].y - map->wall_nodes[w].y);
                    if(curr * cross < 0)
                        return 0;

                    cross = curr;

                    w_prev = w;
                    w = map->wall_nodes[w].A_side;
                }
                else if(map->wall_nodes[w].B_side != w_prev)
                {
                    int32_t curr = Vector2CrossProduct(map->wall_nodes[map->wall_nodes[w].B_side].x - map->wall_nodes[w].x, map->wall_nodes[map->wall_nodes[w].B_side].y - map->wall_nodes[w].y, map->wall_nodes[w_prev].x - map->wall_nodes[w].x, map->wall_nodes[w_prev].y - map->wall_nodes[w].y);
                    if(curr * cross < 0)
                        return 0;

                    cross = curr;

                    w_prev = w;
                    w = map->wall_nodes[w].B_side;
                }
                else
                    return 0;
    
            }
        }
    }
    return count;
}

int8_t SLUGmaker_CheckMap(SLUGmaker_map *map)
{
    //no unfished arcs
    
    return 1;
}

int8_t SLUGmaker_WriteMap(SLUGmaker_map *map)
{
    if(SLUGmaker_CheckMap(map))
    {
        //proceed
    }   
    else
        printf("Map is not suitable to be exported.\n");

    return 0;
}
