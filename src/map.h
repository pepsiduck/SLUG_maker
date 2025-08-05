#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>

#include "geometry.h"

#define MAX_WALLS_NB 8192 
#define SPACE_SOLID -1
#define SPACE_EMPTY -2

typedef struct SLUGmaker_SegmentExtended SLUGmaker_SegmentExtended;
struct SLUGmaker_SegmentExtended
{
    Vector2 A; 
    Vector2 B;
    Vector2 normal; //||norm|| = 1
    Vector2 middlepoint;
    SLUGmaker_SegmentExtended *next;
    bool exists;
};

typedef struct SLUGmaker_map SLUGmaker_map;
struct SLUGmaker_map
{
    Rectangle zone;
    Texture2D fixed_sprite;
    
    SLUGmaker_SegmentExtended walls[MAX_WALLS_NB];
    int16_t current_wall_index;
    int16_t wall_nb;
    int16_t wall_line_origin_index;
    bool wall_line_mode;
    int16_t wall_move_mode;

    Vector2 player_node;
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
    int32_t tab_size;
    SLUG_SegmentExtended *tab;
    int32_t elements_size; 
    SLUG_BSPTreeElement *elements; //tree trunk is always element index 0
};

void SLUG_BSPTreeUnload(SLUG_BSPTree *tree);

int32_t SLUGmaker_CountOnes(int8_t *tab, int32_t tab_size);
int8_t SLUGmaker_AllZeros(int8_t *tab, int32_t tab_size);
int32_t SLUGmaker_GetIndexForPosition(int8_t *tab, int32_t tab_size, int32_t position);

SLUGmaker_map* SLUGmaker_NewMap(const char *filename);
SLUGmaker_map* SLUGmaker_LoadMap(const char *loadMap);
void SLUGmaker_UnloadMap(SLUGmaker_map *map);

SLUG_SegmentExtended* SLUGmaker_GetSegments(SLUGmaker_map *map); 
int32_t* SLUGmaker_GetWallsLinks(SLUGmaker_map *map);
bool SLUGmaker_MapWallsTest(SLUG_SegmentExtended *segs, int32_t seg_size);

int8_t SLUGmaker_BSPTreeBuildRecursive(SLUG_BSPTree *tree,int32_t *node_nb, int8_t *elements_taken);
int8_t SLUGmaker_BSPTreeBuild(SLUG_BSPTree *tree,int32_t *node_nb);

bool SLUGmaker_CheckSave(SLUGmaker_map *map);
int8_t SLUGmaker_WriteMap(SLUGmaker_map *map);

#endif
