#include "map.h"

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

int8_t SLUGmaker_WriteMap(SLUGmaker_map *map, const char *filename)
{

    FILE *file = fopen(filename, "w");
    if(file == NULL)
    {
        printf("Error while creating output file.\n");
        return -1;
    }

    //TODO

    return 0;
}
