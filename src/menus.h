#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>
#include <raygui.h>

typedef enum {
                NONE,
                BUTTON_SAVE,
                BUTTON_WALL, 
                BUTTON_DELETE, 
                BUTTON_NONE
              }  SLUGmaker_ButtonType;

typedef struct SLUGmaker_Button SLUGmaker_Button;
struct SLUGmaker_Button
{
    Rectangle zone;
    Color hover;
    GuiIconName icon;
    SLUGmaker_ButtonType type;
    bool active;
};

SLUGmaker_Button *SLUGmaker_LoadButton(Rectangle zone, Rectangle parent_menu, Color color, Color hover, SLUGmaker_ButtonType type);
void SLUGmaker_UnloadButton(SLUGmaker_Button button);

SLUGmaker_ButtonType SLUGmaker_ButtonMouseHover(Vector2 mouse);

typedef struct SLUGmaker_Menu SLUGmaker_Menu;
struct SLUGmaker_Menu
{
	Rectangle zone; //Relative to screen_w and screen_h
	Color color;
    bool active;

    uint8_t buttons_nb;
    SLUGmaker_Button** button_list;
};

SLUGmaker_Menu *SLUGmaker_LoadMenu(Rectangle zone, Color color);
void SLUGmaker_UnloadMenu(SLUGmaker_Menu *menu);

int8_t SLUGmaker_MenuResize(SLUGmaker_Menu *menu, float factor_x, float factor_y);

#define MENU_NB 3
extern SLUGmaker_Menu* menus[MENU_NB];
int8_t SLUGmaker_MenusInit();
void SLUGmaker_MenusUnload();

int8_t bullshit();

#endif
