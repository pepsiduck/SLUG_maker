#include "map.h"

SLUG_map* SLUGmaker_NewMap(const char *filename)
{
    SLUG_map *map = (SLUG_map *) malloc(sizeof(SLUG_map));
    map->fixed_sprite = LoadTexture(filename);
    if(map->fixed_sprite.id <= 0)
    {
        printf("Error while loading new map sprite.\n");
        free(map);
        return NULL;
    }
    SetTextureWrap(map->fixed_sprite, 0);
    map->w = map->fixed_sprite.width;
    map->h = map->fixed_sprite.height;
    return map;
}

SLUG_map* SLUGmaker_LoadMap(const char *loadMap)
{
    return NULL; //TODO
}

void SLUGmaker_UnloadMap(SLUG_map *map)
{
    UnloadTexture(map->fixed_sprite);
    free(map);
}

int8_t SLUGmaker_WriteMap(SLUG_map *map, const char *filename)
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
