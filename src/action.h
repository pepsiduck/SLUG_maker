#ifndef ACTION_H
#define ACTION_H

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <string.h>

#include "map.h"
#include "display.h"

typedef enum {ACTION_NONE, ACTION_WALL} SLUGmaker_action;
extern SLUGmaker_action current_action;

int8_t SLUGmaker_CameraUpdate(SLUGmaker_camera *cam);

int16_t SLUGmaker_NodeUnderMouse(SLUGmaker_map *map, SLUGmaker_camera *cam);

int8_t SLUGmaker_ChangeAction();

int8_t SLUGmaker_Place(SLUGmaker_map *map, SLUGmaker_camera *cam);
int8_t SLUGmaker_PlaceWallNode(SLUGmaker_map *map, SLUGmaker_camera *cam);

#endif
