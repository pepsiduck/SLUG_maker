#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>
#include <raygui.h>
#include <string.h>

//---global variables
typedef struct SLUGmaker_MenuVariables SLUGmaker_MenuVariables;
struct SLUGmaker_MenuVariables
{
	bool map_selection_menu;
	int32_t map_selection_result;
};

extern SLUGmaker_MenuVariables menu_vars;

int8_t SLUGmaker_MenuInit();
int8_t SLUGmaker_MenuUnload();

//---buttons
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

typedef struct SLUGmaker_ComboBox SLUGmaker_ComboBox;
struct SLUGmaker_ComboBox
{
    Rectangle zone;
    int32_t state;
    char options[1024];
};

int8_t SLUGmaker_ComboBoxLoad(Rectangle zone, int32_t default_state, const char *options, SLUGmaker_ComboBox *combobox);
int8_t SLUGmaker_ComboBoxResize(float factor_x, float factor_y, SLUGmaker_ComboBox *combobox);
int8_t SLUGmaker_ComboBoxPressed(SLUGmaker_ComboBox *combobox);

typedef struct SLUGmaker_ToolBar SLUGmaker_ToolBar;
struct SLUGmaker_ToolBar
{
    Rectangle zone;
    
    //styles
    Rectangle style_zone;
    Rectangle style_label_zone;
    
    SLUGmaker_button save;
    SLUGmaker_ComboBox styles;
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


//pop-ups
int8_t SLUGmaker_PopUpNewMap();

//---functions
int8_t SLUGmaker_ChangeGUIStyle(SLUGmaker_ToolBar *toolbar);

//general menu
typedef struct SLUGmaker_Menu SLUGmaker_Menu;
struct SLUGmaker_Menu
{
    SLUGmaker_ToolBar toolbar;
    SLUGmaker_ActionButtonsMenu actionMenu;
};

SLUGmaker_Menu SLUGmaker_MenuDevLoad();
int8_t SLUGmaker_MenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_MenuDisplay(SLUGmaker_Menu *menu);


#endif
