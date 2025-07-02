#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>

#include "map.h"

extern Rectangle display;

int8_t SLUGmaker_GraphicInit();

typedef struct SLUG_camera SLUG_camera;
struct SLUG_camera
{
    Rectangle view_zone;
    Rectangle *display;
    SLUG_map *map;
};

SLUG_camera SLUGmaker_DefaultCamera(SLUG_map *map);

int8_t SLUGmaker_DisplayUpdate(Rectangle *rect);
int8_t SLUGmaker_Display(SLUG_camera *cam);

#endif
