#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>
#include <raygui.h>
#include <string.h>

#define ACTION_MENU_NB 5

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
    char tooltip[256];
};

int8_t SLUGmaker_ButtonLoad(Rectangle zone, uint16_t guiIcon, SLUGmaker_button *button);
int8_t SLUGmaker_ButtonSetToolTip(const char * text, SLUGmaker_button *button);
int8_t SLUGmaker_ButtonResize(float factor_x, float factor_y, SLUGmaker_button *button);
int8_t SLUGmaker_ButtonPressed(SLUGmaker_button *button, bool tooltip);

typedef struct SLUGmaker_ComboBox SLUGmaker_ComboBox;
struct SLUGmaker_ComboBox
{
    Rectangle zone;
    int32_t state;
    char options[1024];
    char tooltip[256];
};

int8_t SLUGmaker_ComboBoxLoad(Rectangle zone, int32_t default_state, const char *options, SLUGmaker_ComboBox *combobox);
int8_t SLUGmaker_ComboBoxSetToolTip(const char * text, SLUGmaker_ComboBox *combobox);
int8_t SLUGmaker_ComboBoxResize(float factor_x, float factor_y, SLUGmaker_ComboBox *combobox);
int8_t SLUGmaker_ComboBoxPressed(SLUGmaker_ComboBox *combobox, bool tooltip);

//toobar
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

//Action Menu
typedef struct SLUGmaker_ActionButtonsMenu SLUGmaker_ActionButtonsMenu;
struct SLUGmaker_ActionButtonsMenu
{
    Rectangle zone;
    Rectangle group_zone;
    GuiIconName icons[ACTION_MENU_NB];
    SLUGmaker_button modes[ACTION_MENU_NB];
};

SLUGmaker_ActionButtonsMenu SLUGmaker_ActionButtonsMenuDevLoad();
int8_t SLUGmaker_ActionButtonsMenuResize(float factor_x, float factor_y, SLUGmaker_ActionButtonsMenu *menu);
int8_t SLUGmaker_ActionButtonsMenuPressed(SLUGmaker_ActionButtonsMenu *menu);
int8_t SLUGmaker_ActionButtonsMenuDisplay(SLUGmaker_ActionButtonsMenu *menu);

//Info menu
typedef struct SLUGmaker_InfoMenu SLUGmaker_InfoMenu;
struct SLUGmaker_InfoMenu
{
    Rectangle panel_zone;
    Rectangle text_zone;
}; 

SLUGmaker_InfoMenu SLUGmaker_InfoMenuDevLoad();
int8_t SLUGmaker_InfoMenuResize(float factor_x, float factor_y, SLUGmaker_InfoMenu *info_menu);
int8_t SLUGmaker_InfoMenuDisplay(SLUGmaker_InfoMenu *info_menu, void *ptr);

extern void (*SLUGmaker_InfoMenuPrintFct[ACTION_MENU_NB])(void *,Rectangle);
void SLUGmaker_InfoMenuPrintNone(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintWall(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintPlayerSpawn(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintDelete(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintSprites(void *ptr, Rectangle bounds);

//general menu
typedef struct SLUGmaker_Menu SLUGmaker_Menu;
struct SLUGmaker_Menu
{
    SLUGmaker_ToolBar toolbar;
    SLUGmaker_ActionButtonsMenu actionMenu;
    SLUGmaker_InfoMenu infoMenu;
};

extern SLUGmaker_Menu general_menus;

int8_t SLUGmaker_MenuDevLoad();
int8_t SLUGmaker_MenuResize(float factor_x, float factor_y);
int8_t SLUGmaker_MenuDisplay(void *ptr);

//pop-ups
int8_t SLUGmaker_PopUpNewMap();

//---functions
int8_t SLUGmaker_ChangeGUIStyle();

#endif
