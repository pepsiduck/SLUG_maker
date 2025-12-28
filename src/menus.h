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


//! Menus
typedef enum {
              MENU_TOOLBAR, 
              MENU_ACTION, 
              MENU_INFO, 
              MENU_ACTION_MODIF,
              MENU_NUMBER} SLUGmaker_MenuType;

typedef struct SLUGmaker_Menu SLUGmaker_Menu;
struct SLUGmaker_Menu 
{
    SLUGmaker_MenuType menu_type;
    Rectangle zone;
};

SLUGmaker_Menu* SLUGmaker_MenuDevLoad(SLUGmaker_MenuType menu_type, Rectangle zone, size_t size);
int8_t SLUGmaker_MenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
extern SLUGmaker_Menu* (*SLUGmaker_MenuDevLoadFunctions[MENU_NUMBER])(void);
extern int8_t (*SLUGmaker_MenuFreeFunctions[MENU_NUMBER])(SLUGmaker_Menu *menu);
extern int8_t (*SLUGmaker_MenuResizeFunctions[MENU_NUMBER])(float factor_x, float factor_y, SLUGmaker_Menu *menu);
extern int8_t (*SLUGmaker_MenuPressedFunctions[MENU_NUMBER])(SLUGmaker_Menu *menu);
extern int8_t (*SLUGmaker_MenuDisplayFunctions[MENU_NUMBER])(SLUGmaker_Menu *menu, void *ptr);

//toobar
typedef struct SLUGmaker_ToolBar SLUGmaker_ToolBar;
struct SLUGmaker_ToolBar
{
    //--- Universal
    SLUGmaker_Menu m;
    //---  
    
    //styles
    Rectangle style_zone;
    Rectangle style_label_zone;
    
    SLUGmaker_button save;
    SLUGmaker_ComboBox styles;
};

SLUGmaker_Menu* SLUGmaker_ToolBarDevLoad();
int8_t SLUGmaker_ToolBarFree(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ToolBarResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_ToolBarPressed(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ToolBarDisplay(SLUGmaker_Menu *menu, void *ptr);

//Action Menu
typedef struct SLUGmaker_ActionButtonsMenu SLUGmaker_ActionButtonsMenu;
struct SLUGmaker_ActionButtonsMenu
{
    //--- Universal
    SLUGmaker_Menu m;
    //--- 

    GuiIconName icons[ACTION_MENU_NB];
    SLUGmaker_button modes[ACTION_MENU_NB];
};

SLUGmaker_Menu* SLUGmaker_ActionButtonsMenuDevLoad();
int8_t SLUGmaker_ActionButtonsMenuFree(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionButtonsMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionButtonsMenuPressed(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionButtonsMenuDisplay(SLUGmaker_Menu *menu, void *ptr);

//Info menu
typedef struct SLUGmaker_InfoMenu SLUGmaker_InfoMenu;
struct SLUGmaker_InfoMenu
{
    //--- Universal
    SLUGmaker_Menu m;
    //--- 

    Rectangle text_zone;
}; 

SLUGmaker_Menu* SLUGmaker_InfoMenuDevLoad();
int8_t SLUGmaker_InfoMenuFree(SLUGmaker_Menu *menu);
int8_t SLUGmaker_InfoMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_InfoMenuPressed(SLUGmaker_Menu *menu);
int8_t SLUGmaker_InfoMenuDisplay(SLUGmaker_Menu *menu, void *ptr);

extern void (*SLUGmaker_InfoMenuPrintFct[ACTION_MENU_NB])(void *,Rectangle);
void SLUGmaker_InfoMenuPrintNone(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintWall(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintPlayerSpawn(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintDelete(void *ptr, Rectangle bounds);
void SLUGmaker_InfoMenuPrintSprites(void *ptr, Rectangle bounds);

//Action modif Menu
typedef struct SLUGmaker_ActionModifMenu SLUGmaker_ActionModifMenu ;
struct SLUGmaker_ActionModifMenu 
{
    //--- Universal
    SLUGmaker_Menu m;
    //--- 

    Rectangle group_zone; 
};

SLUGmaker_Menu* SLUGmaker_ActionModifMenuDevLoad();
int8_t SLUGmaker_ActionModifMenuFree(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionModifMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionModifMenuPressed(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionModifMenuDisplay(SLUGmaker_Menu *menu, void *ptr);

//general menu
typedef struct SLUGmaker_GeneralMenu SLUGmaker_GeneralMenu;
struct SLUGmaker_GeneralMenu
{
    SLUGmaker_Menu* menus[MENU_NUMBER];
};

extern SLUGmaker_GeneralMenu general_menu;

int8_t SLUGmaker_GeneralMenuDevLoad();
int8_t SLUGmaker_GeneralMenuFree();
int8_t SLUGmaker_GeneralMenuResize(float factor_x, float factor_y);
int8_t SLUGmaker_GeneralMenuDisplay(void *ptr);

//pop-ups
int8_t SLUGmaker_PopUpNewMap();

//---functions
int8_t SLUGmaker_ChangeGUIStyle();

#endif
