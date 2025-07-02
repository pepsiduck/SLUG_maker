#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <inttypes.h>

typedef struct SLUG_map SLUG_map;
struct SLUG_map
{
    uint32_t w;
    uint32_t h;
    Texture2D fixed_sprite;
};

SLUG_map* SLUGmaker_NewMap(const char *filename);
SLUG_map* SLUGmaker_LoadMap(const char *loadMap);
void SLUGmaker_UnloadMap(SLUG_map *map);
int8_t SLUGmaker_WriteMap(SLUG_map *map, const char *filename);

#endif
