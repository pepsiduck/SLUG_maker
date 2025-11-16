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

typedef struct SLUGmaker_ToolBar SLUGmaker_ToolBar;
struct SLUGmaker_ToolBar
{
    Rectangle zone;
    SLUGmaker_button save;
};

SLUGmaker_ToolBar SLUGmaker_ToolBarDevLoad();
int8_t SLUGmaker_ToolBarResize(float factor_x, float factor_y, SLUGmaker_ToolBar *toolbar);
int8_t SLUGmaker_ToolBarButtonPressed(SLUGmaker_ToolBar *toolbar);
int8_t SLUGmaker_ToolBarDisplay(SLUGmaker_ToolBar *toolbar);

typedef struct SLUGmaker_ActionButtonsMenu SLUGmaker_ActionButtonsMenu;
struct SLUGmaker_ActionButtonsMenu
{
    Rectangle zone;
    Rectangle group_zone;
    SLUGmaker_button none_mode;
    SLUGmaker_button wall_mode;
    SLUGmaker_button player_mode;
    SLUGmaker_button delete_mode;
};

SLUGmaker_ActionButtonsMenu SLUGmaker_ActionButtonsMenuDevLoad();
int8_t SLUGmaker_ActionButtonsMenuResize(float factor_x, float factor_y, SLUGmaker_ActionButtonsMenu *menu);
int8_t SLUGmaker_ActionButtonsMenuPressed(SLUGmaker_ActionButtonsMenu *menu);
int8_t SLUGmaker_ActionButtonsMenuDisplay(SLUGmaker_ActionButtonsMenu *menu);

#endif
