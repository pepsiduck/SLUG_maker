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
#define MAX_PLACED_SPRITES 1024
#define MAX_SPRITES 128
#define SPACE_SOLID -1
#define SPACE_EMPTY -2

typedef struct SLUGmaker_PlacableSprite SLUGmaker_PlacableSprite; //same as in SLUG
struct SLUGmaker_PlacableSprite
{
	uint16_t sprite_index;
	Rectangle zone;
};

//Même pas besoin de layer : on affiche, on load et on exporte dans l'ordre de map_sprites. 
//C'est ok puisque sprite_index réfère à loaded_sprites, qui n'a rien à voir avec le système de layering.
//Lorsqu'on veut changer de layer, on échange deux éléments de la liste map_sprites. C'est ok puisque c'est un struct.
//Attention : on peut pas laisser de vide : lorsqu'on supprime, on décale de 1 tous ceux qui sont après l'élément supprimé
//O(n) mais ok vu qu'on le fait pas toutes les frames.

typedef struct SLUGmaker_map SLUGmaker_map;
struct SLUGmaker_map
{
    Rectangle zone;
    
    Texture2D loaded_sprites[MAX_SPRITES];
    char loaded_sprites_names[MAX_SPRITES][256];
    SLUGmaker_PlacableSprite map_sprites[MAX_PLACED_SPRITES];
    int16_t sprite_nb;
    int16_t selected_sprite;
    
    SLUGmaker_SegmentExtended walls[MAX_WALLS_NB];
    int16_t current_wall_index;
    int16_t wall_nb;
    int16_t wall_line_origin_index;
    bool wall_line_mode;
    int16_t wall_move_mode;
    
    Vector2 player_spawn_point;
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

SLUGmaker_map* SLUGmaker_NewMap(uint32_t width, uint32_t height);
SLUGmaker_map* SLUGmaker_LoadMap(const char *loadMap);
void SLUGmaker_UnloadMap(SLUGmaker_map *map);

void SLUG_BSPTreeUnload(SLUG_BSPTree *tree);

int32_t SLUGmaker_CountOnes(int8_t *tab, int32_t tab_size);
int8_t SLUGmaker_AllZeros(int8_t *tab, int32_t tab_size);
int32_t SLUGmaker_GetIndexForPosition(int8_t *tab, int32_t tab_size, int32_t position);

SLUG_SegmentExtended* SLUGmaker_GetSegments(SLUGmaker_map *map); 
int32_t* SLUGmaker_GetWallsLinks(SLUGmaker_map *map);
bool SLUGmaker_MapWallsTest(SLUG_SegmentExtended *segs, int32_t seg_size);

int8_t SLUGmaker_BSPTreeBuildRecursive(SLUG_BSPTree *tree,int32_t *node_nb, int8_t *elements_taken);
int8_t SLUGmaker_BSPTreeBuild(SLUG_BSPTree *tree,int32_t *node_nb);

bool SLUGmaker_CheckSave(SLUGmaker_map *map);
int8_t SLUGmaker_WriteMap(SLUGmaker_map *map);

#endif
