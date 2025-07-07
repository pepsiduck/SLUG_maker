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
    int16_t next_index; //-2 : node doesn't exists
};

typedef struct SLUGmaker_map SLUGmaker_map;
struct SLUGmaker_map
{
    Rectangle zone;
    Texture2D fixed_sprite;
    SLUGmaker_wall_node wall_nodes[MAX_WALL_NODES];
    int16_t current_wall_index;
    int16_t chain_start;
    int16_t wall_node_nb;
    int16_t wall_node_chain_nb;
};

SLUGmaker_map* SLUGmaker_NewMap(const char *filename);
SLUGmaker_map* SLUGmaker_LoadMap(const char *loadMap);
void SLUGmaker_UnloadMap(SLUGmaker_map *map);
int8_t SLUGmaker_WriteMap(SLUGmaker_map *map, const char *filename);

#endif
