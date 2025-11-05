#include "menus.h"

SLUGmaker_Menu *SLUGmaker_LoadMenu(Rectangle zone, Color color)
{
	SLUGmaker_Menu *newmenu = (SLUGmaker_Menu *) malloc(sizeof(SLUGmaker_Menu));
	if(newmenu == NULL)
		return NULL;
	
	newmenu->zone.x = zone.x * GetScreenWidth();
	newmenu->zone.y = zone.y * GetScreenHeight();
	newmenu->zone.width = zone.width * GetScreenWidth();
	newmenu->zone.height = zone.height * GetScreenHeight();	
	
	newmenu->color = color;
	
	return newmenu;
}

void SLUGmaker_UnloadMenu(SLUGmaker_Menu *menu)
{
	if(menu != NULL)
	{
		free(menu);
		menu = NULL;
	}
}

int8_t SLUGmaker_MenuResize(SLUGmaker_Menu *menu, float factor_x, float factor_y)
{
	if(menu == NULL)
		return -1;
		
	menu->zone.x *= factor_x;
	menu->zone.y *= factor_y;
	menu->zone.width *= factor_x;
	menu->zone.height *= factor_y;
		
	return 0;
}

SLUGmaker_Menu* menus[MENU_NB];

int8_t SLUGmaker_MenusInit()
{
	menus[0] = SLUGmaker_LoadMenu((Rectangle) {.x = 0.0f, .y = 0.0f, .width = 1.0f, .height = 0.03f}, WHITE);
	if(menus[0] == NULL)
		return -1;
	menus[1] = SLUGmaker_LoadMenu((Rectangle) {.x = 0.0f, .y = 0.03f, .width = 0.2f, .height = 0.97f}, DARKGRAY);
	if(menus[1] == NULL)
	{
		SLUGmaker_MenusUnload();
		return -1;
	}
	menus[2] = SLUGmaker_LoadMenu((Rectangle) {.x = 0.2f, .y = 0.83f, .width = 0.8f, .height = 0.17f}, DARKGRAY);
	if(menus[2] == NULL)
	{
		SLUGmaker_MenusUnload();
		return -1;
	}
	return 0;
}

void SLUGmaker_MenusUnload()
{
	for(uint8_t m = 0; m < MENU_NB; m++)
		SLUGmaker_UnloadMenu(menus[m]);
}
