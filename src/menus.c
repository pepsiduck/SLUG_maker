#include "menus.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

//---buttons
char SLUGmaker_IconBuffer[8];

int8_t SLUGmaker_ButtonLoad(Rectangle zone, uint16_t guiIcon, SLUGmaker_button *button)
{
    if(button == NULL)
        return -1;
    button->zone = zone;
    button->guiIcon = guiIcon;
    button->pressed = false;
    return 0;
}

int8_t SLUGmaker_ButtonResize(float factor_x, float factor_y, SLUGmaker_button *button)
{
    if(button == NULL)
        return -1;

    button->zone.x *= factor_x;
    button->zone.y *= factor_y;
    button->zone.width *= factor_x;
    button->zone.height *= factor_y;

    return 0;
}

int8_t SLUGmaker_ButtonPressed(SLUGmaker_button *button)
{
    if(button == NULL)
        return -1;
    sprintf(SLUGmaker_IconBuffer,"#%u#",button->guiIcon);
    button->pressed = GuiButton(button->zone, SLUGmaker_IconBuffer);
    return 0;
}

//---toolbar
SLUGmaker_ToolBar SLUGmaker_ToolBarDevLoad(uint32_t screen_w, uint32_t screen_h)
{
    SLUGmaker_ToolBar toolbar;
    toolbar.zone = (Rectangle) {.x = 0, .y = 0, .width = GetScreenWidth(), .height = GetScreenHeight() * 0.03f};
    SLUGmaker_ButtonLoad((Rectangle){.x = toolbar.zone.width * 0.0025f, .y = GetScreenHeight() * 0.0025f, .width = GetScreenHeight() * 0.025f, .height = GetScreenHeight() * 0.025f},ICON_FILE_SAVE,&toolbar.save);
    return toolbar;
}

int8_t SLUGmaker_ToolBarResize(float factor_x, float factor_y, SLUGmaker_ToolBar *toolbar)
{
    if(toolbar == NULL)
        return -1;

    toolbar->zone.x *= factor_x;
    toolbar->zone.y *= factor_y;
    toolbar->zone.width *= factor_x;
    toolbar->zone.height *= factor_y;

    SLUGmaker_ButtonResize(factor_x, factor_y, &(toolbar->save));

    return 0;
}

int8_t SLUGmaker_ToolBarButtonPressed(SLUGmaker_ToolBar *toolbar)
{
	if(toolbar == NULL)
		return -1;

	GuiSetTooltip("Save map (CTRL+S)");
    SLUGmaker_ButtonPressed(&(toolbar->save));

    GuiSetTooltip(NULL);
    
    return 0;
}

int8_t SLUGmaker_ToolBarDisplay(SLUGmaker_ToolBar *toolbar)
{
    if(toolbar == NULL)
        return -1;

    GuiPanel(toolbar->zone, NULL);

    int8_t err = SLUGmaker_ToolBarButtonPressed(toolbar);
    if(err < 0)
        return err;

    return 0;
}

//--- action buttons menu
SLUGmaker_ActionButtonsMenu SLUGmaker_ActionButtonsMenuDevLoad(uint32_t screen_w, uint32_t screen_h)
{
	SLUGmaker_ActionButtonsMenu menu;
	
	menu.zone = (Rectangle) {
		.x = 0,
		.y = GetScreenHeight() * 0.03f,
		.width = GetScreenWidth() * 0.2f,
		.height = GetScreenHeight() * 0.2f,
	};
	
	menu.group_zone = (Rectangle) {
		.x = menu.zone.x + 0.05f * menu.zone.width,
		.y = menu.zone.y + 0.05f * menu.zone.height,
		.width = 0.9f * menu.zone.width,
		.height = 0.9f * menu.zone.height,
	};
	
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.group_zone.x + menu.group_zone.width * 0.015f, .y = menu.group_zone.y + menu.group_zone.height * 0.026f, .width = 27, .height = 27},ICON_CURSOR_CLASSIC,&menu.none_mode);
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.none_mode.zone.x + menu.group_zone.width * 0.015f + menu.none_mode.zone.width, .y = menu.group_zone.y + menu.group_zone.height * 0.026f, .width = menu.none_mode.zone.width, .height = menu.none_mode.zone.height},ICON_WALL,&menu.wall_mode);
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.wall_mode.zone.x + menu.group_zone.width * 0.015f + menu.none_mode.zone.width, .y = menu.group_zone.y + menu.group_zone.height * 0.026f, .width = menu.none_mode.zone.width, .height = menu.none_mode.zone.height},ICON_PLAYER,&menu.player_mode);
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.player_mode.zone.x + menu.group_zone.width * 0.015f + menu.none_mode.zone.width, .y = menu.group_zone.y + menu.group_zone.height * 0.026f, .width = menu.none_mode.zone.width, .height = menu.none_mode.zone.height},ICON_DELETE,&menu.delete_mode);
	
	return menu;
}

int8_t SLUGmaker_ActionButtonsMenuResize(float factor_x, float factor_y, SLUGmaker_ActionButtonsMenu *menu)
{
	if(menu == NULL)
		return -1;
		
	menu->zone.x *= factor_x;
    menu->zone.y *= factor_y;
    menu->zone.width *= factor_x;
    menu->zone.height *= factor_y;
    
    menu->group_zone.x *= factor_x;
    menu->group_zone.y *= factor_y;
    menu->group_zone.width *= factor_x;
    menu->group_zone.height *= factor_y; 

    SLUGmaker_ButtonResize(factor_x, factor_y, &(menu->none_mode));
    SLUGmaker_ButtonResize(factor_x, factor_y, &(menu->wall_mode));
    SLUGmaker_ButtonResize(factor_x, factor_y, &(menu->player_mode));
    SLUGmaker_ButtonResize(factor_x, factor_y, &(menu->delete_mode));
		
	return 0;
}

int8_t SLUGmaker_ActionButtonsMenuPressed(SLUGmaker_ActionButtonsMenu *menu)
{
	if(menu == NULL)
		return -1;
		
	GuiSetTooltip("No editing (N)");
	SLUGmaker_ButtonPressed(&(menu->none_mode));
	GuiSetTooltip("Place and move walls (W)");
	SLUGmaker_ButtonPressed(&(menu->wall_mode));
	GuiSetTooltip("Move Player spawn point (P)");
	SLUGmaker_ButtonPressed(&(menu->player_mode));
	GuiSetTooltip("Delete elements (BACKSPACE)");
	SLUGmaker_ButtonPressed(&(menu->delete_mode));
	
	GuiSetTooltip(NULL);
		
	return 0;
}

int8_t SLUGmaker_ActionButtonsMenuDisplay(SLUGmaker_ActionButtonsMenu *menu)
{
	if(menu == NULL)
		return -1;
		
	GuiGroupBox(menu->group_zone,"Action controls");

    int8_t err = SLUGmaker_ActionButtonsMenuPressed(menu);
    if(err < 0)
        return err;
		
    return 0;
}
