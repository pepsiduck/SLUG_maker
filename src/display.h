#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>

#include "map.h"

typedef struct SLUGmaker_GraphicVariables SLUGmaker_GraphicVariables;
struct SLUGmaker_GraphicVariables
{
    Rectangle display;
    Texture2D mouse_cursor_sprite; 
    Texture2D wall_node_sprite;
};

extern SLUGmaker_GraphicVariables graphic_vars;

int8_t SLUGmaker_GraphicInit();
int8_t SLUGmaker_GraphicUnload();

typedef struct SLUGmaker_camera SLUGmaker_camera;
struct SLUGmaker_camera
{
    Rectangle view_zone;
    Rectangle *display;
    SLUGmaker_map *map;
    float unzoom;
    float ratiox;
    float ratioy;
};

SLUGmaker_camera SLUGmaker_DefaultCamera(SLUGmaker_map *map);
float SLUGmaker_GetMousePosX(SLUGmaker_camera *cam);
float SLUGmaker_GetMousePosY(SLUGmaker_camera *cam);

int8_t SLUGmaker_DisplayUpdate(SLUGmaker_camera *cam);
int8_t SLUGmaker_DisplayMap(SLUGmaker_camera *cam);
int8_t SLUGmaker_DisplayWallNodes(SLUGmaker_camera *cam);
int8_t SLUGmaker_Display(SLUGmaker_camera *cam);

#endif
