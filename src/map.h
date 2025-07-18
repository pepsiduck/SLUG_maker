#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <inttypes.h>

#define MAX_WALL_NODES 8192 // avec int16_t en next_index, max 32767

typedef struct SLUGmaker_wall_node SLUGmaker_wall_node;
struct SLUGmaker_wall_node
{
    int32_t x;
    int32_t y;
    bool exists;
    int16_t A_side;
    int16_t B_side;
};

typedef struct SLUGmaker_map SLUGmaker_map;
struct SLUGmaker_map
{
    Rectangle zone;
    Texture2D fixed_sprite;
    
    SLUGmaker_wall_node wall_nodes[MAX_WALL_NODES];
    int16_t current_wall_index;
    int16_t wall_node_nb;
    bool wall_line_mode;
    int16_t wall_line_origin_index;
    int16_t wall_node_move_mode;
};

SLUGmaker_map* SLUGmaker_NewMap(const char *filename);
SLUGmaker_map* SLUGmaker_LoadMap(const char *loadMap);
void SLUGmaker_UnloadMap(SLUGmaker_map *map);

int16_t SLUGmaker_GetArcsIndex(SLUGmaker_map *map, int16_t *arcs);
int8_t SLUGmaker_CheckMap(SLUGmaker_map *map);
int8_t SLUGmaker_WriteMap(SLUGmaker_map *map);

#endif
