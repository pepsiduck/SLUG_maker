#ifndef ACTION_H
#define ACTION_H

#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <math.h>

#include "map.h"

#define ACTIONS_NB 5

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

//camera
SLUGmaker_camera SLUGmaker_DefaultCamera(SLUGmaker_map *map);
float SLUGmaker_GetMousePosX(SLUGmaker_camera *cam);
float SLUGmaker_GetMousePosY(SLUGmaker_camera *cam);
int8_t SLUGmaker_CameraUpdate(SLUGmaker_camera *cam, bool window_changed);

//action types
typedef enum {
	ACTION_MODE_NONE,
	ACTION_MODE_DELETE, 
	ACTION_MODE_WALL,
	ACTION_MODE_PLAYER, 
    ACTION_MODE_SPRITE
	} SLUGmaker_action_mode;
extern SLUGmaker_action_mode previous_action;
extern SLUGmaker_action_mode current_action;

//action handling 
int8_t SLUGmaker_ChangeActionMode(SLUGmaker_map *map);
int8_t SLUGmaker_Action(SLUGmaker_map *map, SLUGmaker_camera *cam);

//delete handling
int8_t SLUGmaker_MapElementDelete(SLUGmaker_map *map,SLUGmaker_camera *cam);
int8_t SLUGmaker_WallDelete(SLUGmaker_map *map,SLUGmaker_camera *cam);

//wall
int16_t SLUGmaker_WallUnderMouseA(SLUGmaker_map *map, SLUGmaker_camera *cam);
int16_t SLUGmaker_WallUnderMouseB(SLUGmaker_map *map, SLUGmaker_camera *cam);
int8_t SLUGmaker_WallMode(SLUGmaker_map *map, SLUGmaker_camera *cam);
int8_t SLUGmaker_MoveWall(SLUGmaker_map *map, SLUGmaker_camera *cam);
int8_t SLUGmaker_PlaceNewWall(SLUGmaker_map *map, SLUGmaker_camera *cam);
int8_t SLUGmaker_WallModeQuit(SLUGmaker_map *map);

//player spawn point
int8_t SLUGmaker_PlayerSpawnPointMove(SLUGmaker_map *map, SLUGmaker_camera *cam);

#endif
