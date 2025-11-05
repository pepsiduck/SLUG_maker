#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>

typedef struct SLUGmaker_Menu SLUGmaker_Menu;
struct SLUGmaker_Menu
{
	Rectangle zone; //Relative to screen_w and screen_h
	Color color;
};

SLUGmaker_Menu *SLUGmaker_LoadMenu(Rectangle zone, Color color);
void SLUGmaker_UnloadMenu(SLUGmaker_Menu *menu);

int8_t SLUGmaker_MenuResize(SLUGmaker_Menu *menu, float factor_x, float factor_y);

#define MENU_NB 3
extern SLUGmaker_Menu* menus[MENU_NB];
int8_t SLUGmaker_MenusInit();
void SLUGmaker_MenusUnload();

#endif
