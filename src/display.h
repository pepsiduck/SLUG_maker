#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>

#include "menus.h"
#include "action.h"

int8_t SLUGmaker_DisplayUpdate(float factor_x, float factor_y, SLUGmaker_camera *cam);

int8_t SLUGmaker_DisplayWalls(SLUGmaker_camera *cam);
int8_t SLUGmaker_DisplayPlayerSpawnPoint(SLUGmaker_camera *cam);
int8_t SLUGmaker_DisplayMap(SLUGmaker_camera *cam);

int8_t SLUGmaker_Display(SLUGmaker_camera *cam);

#endif
