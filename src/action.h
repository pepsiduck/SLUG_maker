#ifndef ACTION_H
#define ACTION_H

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <math.h>

#include "map.h"
#include "display.h"

typedef enum {ACTION_MODE_NONE, ACTION_MODE_DELETE, ACTION_MODE_WALL, ACTION_MODE_PLAYER} SLUGmaker_action_mode;
extern SLUGmaker_action_mode previous_action;
extern SLUGmaker_action_mode current_action;

int8_t SLUGmaker_CameraUpdate(SLUGmaker_camera *cam);

//under mouse
int16_t SLUGmaker_WallUnderMouseA(SLUGmaker_map *map, SLUGmaker_camera *cam);
int16_t SLUGmaker_WallUnderMouseB(SLUGmaker_map *map, SLUGmaker_camera *cam);

//action
int8_t SLUGmaker_ChangeActionMode(SLUGmaker_map *map);
int8_t SLUGmaker_Action(SLUGmaker_map *map, SLUGmaker_camera *cam);
int8_t SLUGmaker_WallMode(SLUGmaker_map *map, SLUGmaker_camera *cam);

//delete
int8_t SLUGmaker_MapElementDelete(SLUGmaker_map *map,SLUGmaker_camera *cam);
int8_t SLUGmaker_WallDelete(SLUGmaker_map *map,SLUGmaker_camera *cam);

//place
int8_t SLUGmaker_PlaceNewWall(SLUGmaker_map *map, SLUGmaker_camera *cam);

//move
int8_t SLUGmaker_MoveWall(SLUGmaker_map *map, SLUGmaker_camera *cam);
int8_t SLUGmaker_MovePlayer(SLUGmaker_map *map, SLUGmaker_camera *cam);

//quit
int8_t SLUGmaker_WallModeQuit(SLUGmaker_map *map);

#endif
