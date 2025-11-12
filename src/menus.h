#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>
#include <raygui.h>

#include "string.h"

extern char SLUGmaker_IconBuffer[8];

typedef struct SLUGmaker_button SLUGmaker_button;
struct SLUGmaker_button
{
    Rectangle zone;
    uint16_t guiIcon;
    bool pressed;
};

int8_t SLUGmaker_ButtonLoad(Rectangle zone, uint16_t guiIcon, SLUGmaker_button *button);
int8_t SLUGmaker_ButtonResize(float factor_x, float factor_y, SLUGmaker_button *button);
int8_t SLUGmaker_ButtonPressed(SLUGmaker_button *button);

typedef struct SLUGmaker_Toolbar SLUGmaker_Toolbar;
struct SLUGmaker_Toolbar
{
    Rectangle zone;
    SLUGmaker_button save;
};

SLUGmaker_Toolbar SLUGmaker_ToolBarDevLoad(uint32_t screen_w, uint32_t screen_h);
int8_t SLUGmaker_ToolBarResize(float factor_x, float factor_y, SLUGmaker_Toolbar *toolbar);
int8_t SLUGmaker_ToolBarButtonPressed(SLUGmaker_Toolbar *toolbar);
int8_t SLUGmaker_ToolBar(SLUGmaker_Toolbar *toolbar);

#endif
