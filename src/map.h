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

typedef struct SLUG_SegmentExtended SLUG_SegmentExtended;
struct SLUG_SegmentExtended
{
    Vector2 A; 
    Vector2 B;
    Vector2 normal; //norm = 1
    float dist; //distance de la droite représentée par le segment ar rapport à 0, 0
};

typedef struct SLUG_BSPTreeElement SLUG_BSPTreeElement;
struct SLUG_BSPTreeElement
{
    int32_t segment;
    int32_t children[2];
};

typedef struct SLUG_BSPTree SLUG_BSPTree;
struct SLUG_BSPTree
{
    SLUG_SegmentExtended *tab; 
    SLUG_BSPTreeElement *elements; //tree trunk is always element index 0
    int32_t tab_size;
    int32_t elements_size;
};

SLUGmaker_map* SLUGmaker_NewMap(const char *filename);
SLUGmaker_map* SLUGmaker_LoadMap(const char *loadMap);
void SLUGmaker_UnloadMap(SLUGmaker_map *map);

int16_t SLUGmaker_GetArcs(SLUGmaker_map *map, SLUG_SegmentExtended *arcs,  int16_t arcs_size);
int8_t SLUGmaker_CheckMap(SLUGmaker_map *map);
int8_t SLUGmaker_WriteMap(SLUGmaker_map *map);

#endif
