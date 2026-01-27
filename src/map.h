#ifndef MAP_H
#define MAP_H

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>

#include "geometry.h"

#define MAX_WALLS_NB 8192 
#define MAX_PLACED_SPRITES 1024
#define MAX_SPRITES 128
#define MAX_MAP_CHAR 256
#define SPACE_SOLID -1
#define SPACE_EMPTY -2

typedef struct SLUGmaker_PlacableSprite SLUGmaker_PlacableSprite; //same as in SLUG
struct SLUGmaker_PlacableSprite
{
	int16_t sprite_index;
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
	char map_name[MAX_MAP_CHAR];
	char map_path[MAX_MAP_CHAR];

    Rectangle zone;
    
    Texture2D loaded_sprites[MAX_SPRITES];
    int16_t loaded_sprites_nb;
    char *loaded_sprites_names[MAX_SPRITES];
    SLUGmaker_PlacableSprite map_sprites[MAX_PLACED_SPRITES];
    int16_t sprite_nb;
    int16_t selected_sprite;
    int8_t sprite_move_mode;
    
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

//utils
int32_t SLUGmaker_CountOnes(int8_t *tab, int32_t tab_size);
int8_t SLUGmaker_AllZeros(int8_t *tab, int32_t tab_size);
int32_t SLUGmaker_GetIndexForPosition(int8_t *tab, int32_t tab_size, int32_t position);
int8_t SLUGmaker_RemoveDirRecursive(const char *dir);//TODO:windows

//map load/unload
SLUGmaker_map* SLUGmaker_NewMap(uint32_t width, uint32_t height);
SLUGmaker_map* SLUGmaker_LoadMap(const char *map_dir);
void SLUGmaker_UnloadMap(SLUGmaker_map *map);

//map walls
SLUG_SegmentExtended* SLUGmaker_GetSegments(SLUGmaker_map *map); 
int32_t* SLUGmaker_GetWallsLinks(SLUGmaker_map *map);
bool SLUGmaker_MapWallsTest(SLUG_SegmentExtended *segs, int32_t seg_size);

//BSPTree
int8_t SLUGmaker_BSPTreeBuildRecursive(SLUG_BSPTree *tree,int32_t *node_nb, int8_t *elements_taken);
int8_t SLUGmaker_BSPTreeBuild(SLUG_BSPTree *tree,int32_t *node_nb);
void SLUG_BSPTreeUnload(SLUG_BSPTree *tree);

//map save
bool SLUGmaker_CheckSave(SLUGmaker_map *map);
int8_t SLUGmaker_WriteMap(SLUGmaker_map *map);//TODO:windows

#endif
