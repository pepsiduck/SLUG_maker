#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>
#include <raygui.h>
#include <string.h>
#include <stdarg.h>

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

typedef struct SLUGmaker_Button SLUGmaker_Button;
struct SLUGmaker_Button
{
    Rectangle zone;
    uint16_t guiIcon;
    bool pressed;
    bool icon_button;
    char text[256];
};

int8_t SLUGmaker_ButtonLoad(Rectangle zone, uint16_t guiIcon, bool icon_button, SLUGmaker_Button *button);
int8_t SLUGmaker_ButtonSetText(const char * text, SLUGmaker_Button *button);
int8_t SLUGmaker_ButtonResize(float factor_x, float factor_y, SLUGmaker_Button *button);
int8_t SLUGmaker_ButtonPressed(SLUGmaker_Button *button, bool tooltip);

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

typedef struct SLUGmaker_ListView SLUGmaker_ListView;
struct SLUGmaker_ListView
{
    Rectangle zone;
    const char **options;
    int16_t *nb;
    int scrollIndex;
    int active;
    int focus;
};

int8_t SLUGmaker_ListViewLoad(Rectangle zone, char **options, int16_t *nb, int scrollIndex, int active, int focus, SLUGmaker_ListView *listview);
int8_t SLUGmaker_ListViewResize(float factor_x, float factor_y, SLUGmaker_ListView *listview);
int8_t SLUGmaker_ListViewPressed(SLUGmaker_ListView *listview);

typedef struct SLUGmaker_Spinner SLUGmaker_Spinner;
struct SLUGmaker_Spinner
{
	Rectangle zone;
	const char *text;
	int *value;
	int minValue;
	int maxValue;
	bool editMode;
};

int8_t SLUGmaker_SpinnerLoad(Rectangle zone, const char *text, int *value, int minValue, int maxValue, bool editMode, SLUGmaker_Spinner *spinner);
int8_t SLUGmaker_SpinnerResize(float factor_x, float factor_y, SLUGmaker_Spinner *spinner);
int8_t SLUGmaker_SpinnerPressed(SLUGmaker_Spinner *spinner);

//! Menus
typedef enum {
              MENU_ACTION_MODIF,
              MENU_MODIF,
              MENU_LOGS,
              MENU_INFO, 
              MENU_ACTION,
              MENU_TOOLBAR,
              MENU_NUMBER} SLUGmaker_MenuType;

typedef struct SLUGmaker_Menu SLUGmaker_Menu;
struct SLUGmaker_Menu 
{
    SLUGmaker_MenuType menu_type;
    Rectangle zone;
};

SLUGmaker_Menu* SLUGmaker_MenuDevLoad(SLUGmaker_MenuType menu_type, Rectangle zone, size_t size);
int8_t SLUGmaker_MenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
extern SLUGmaker_Menu* (*SLUGmaker_MenuDevLoadFunctions[MENU_NUMBER])(void *ptr);
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
    
    SLUGmaker_Button save;
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
    SLUGmaker_Button modes[ACTION_MENU_NB];
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

SLUGmaker_Menu* SLUGmaker_InfoMenuDevLoad(void *ptr);
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
typedef struct SLUGmaker_SpriteActionModifMenu SLUGmaker_SpriteActionModifMenu;
struct SLUGmaker_SpriteActionModifMenu
{
    SLUGmaker_Button load_sprite_button;
    SLUGmaker_ListView sprite_list;
    Rectangle sprite_display;
    SLUGmaker_Button place_sprite_button;
    SLUGmaker_Button replace_sprite_button;
    SLUGmaker_Button delete_sprite_button;
};

int8_t SLUGmaker_SpriteActionModifMenuLoad(Rectangle *parent_zone, void *ptr, SLUGmaker_SpriteActionModifMenu *sprite_menu);
int8_t SLUGmaker_SpriteActionModifMenuResize(float factor_x, float factor_y, SLUGmaker_SpriteActionModifMenu *sprite_menu);
int8_t SLUGmaker_SpriteActionModifMenuPressed(SLUGmaker_SpriteActionModifMenu *sprite_menu);
int8_t SLUGmaker_SpriteActionModifMenuDisplay(SLUGmaker_SpriteActionModifMenu *sprite_menu, void *ptr);

typedef struct SLUGmaker_ActionModifMenu SLUGmaker_ActionModifMenu ;
struct SLUGmaker_ActionModifMenu 
{
    //--- Universal
    SLUGmaker_Menu m;
    //--- 

    Rectangle group_zone; 
    SLUGmaker_SpriteActionModifMenu sprite_menu;
};

SLUGmaker_Menu* SLUGmaker_ActionModifMenuDevLoad(void *ptr);
int8_t SLUGmaker_ActionModifMenuFree(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionModifMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionModifMenuPressed(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ActionModifMenuDisplay(SLUGmaker_Menu *menu, void *ptr);

//Modif Menu
typedef struct SLUGmaker_SpriteModifMenu SLUGmaker_SpriteModifMenu;
struct SLUGmaker_SpriteModifMenu
{
    SLUGmaker_Spinner layer_select;
    int select;
};

int8_t SLUGmaker_SpriteModifMenuLoad(Rectangle *parent_zone, void *ptr, SLUGmaker_SpriteModifMenu *sprite_menu);
int8_t SLUGmaker_SpriteModifMenuResize(float factor_x, float factor_y, SLUGmaker_SpriteModifMenu *sprite_menu);
int8_t SLUGmaker_SpriteModifMenuPressed(SLUGmaker_SpriteModifMenu *sprite_menu);
int8_t SLUGmaker_SpriteModifMenuDisplay(SLUGmaker_SpriteModifMenu *sprite_menu, void *ptr);

typedef struct SLUGmaker_ModifMenu SLUGmaker_ModifMenu ;
struct SLUGmaker_ModifMenu 
{
    //--- Universal
    SLUGmaker_Menu m;
    //--- 

    Rectangle group_zone;
    SLUGmaker_SpriteModifMenu sprite_menu;
};

SLUGmaker_Menu* SLUGmaker_ModifMenuDevLoad(void *ptr);
int8_t SLUGmaker_ModifMenuFree(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ModifMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_ModifMenuPressed(SLUGmaker_Menu *menu);
int8_t SLUGmaker_ModifMenuDisplay(SLUGmaker_Menu *menu, void *ptr);

//Logs Menu
typedef struct SLUGmaker_LogsMenu SLUGmaker_LogsMenu ;
struct SLUGmaker_LogsMenu 
{
    //--- Universal
    SLUGmaker_Menu m;
    //--- 

    Rectangle group_zone; 
};

#define MAX_LOG_STR 256
#define MAX_LOG 31
extern char logs[MAX_LOG][MAX_LOG_STR];
extern uint8_t logs_counter;
int8_t SLUGmaker_WriteLog(const char * log, ...);
int8_t SLUGmaker_FlushLogs();

SLUGmaker_Menu* SLUGmaker_LogsMenuDevLoad(void *ptr);
int8_t SLUGmaker_LogsMenuFree(SLUGmaker_Menu *menu);
int8_t SLUGmaker_LogsMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu);
int8_t SLUGmaker_LogsMenuPressed(SLUGmaker_Menu *menu);
int8_t SLUGmaker_LogsMenuDisplay(SLUGmaker_Menu *menu, void *ptr);

//general menu
typedef struct SLUGmaker_GeneralMenu SLUGmaker_GeneralMenu;
struct SLUGmaker_GeneralMenu
{
    SLUGmaker_Menu* menus[MENU_NUMBER];
};

extern SLUGmaker_GeneralMenu general_menu;

int8_t SLUGmaker_GeneralMenuDevLoad(void *ptr);
int8_t SLUGmaker_GeneralMenuFree();
int8_t SLUGmaker_GeneralMenuResize(float factor_x, float factor_y);
int8_t SLUGmaker_GeneralMenuDisplay(void *ptr);

//pop-ups
int8_t SLUGmaker_PopUpNewMap();

//---functions
int8_t SLUGmaker_ChangeGUIStyle();

#endif
