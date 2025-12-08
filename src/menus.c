#include "menus.h"
#include "defines.h"
#include "geometry.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "styles/style_jungle.h"            // raygui style: jungle
#include "styles/style_candy.h"             // raygui style: candy
#include "styles/style_lavanda.h"           // raygui style: lavanda
#include "styles/style_cyber.h"             // raygui style: cyber
#include "styles/style_terminal.h"          // raygui style: terminal
#include "styles/style_ashes.h"             // raygui style: ashes
#include "styles/style_bluish.h"            // raygui style: bluish
#include "styles/style_dark.h"              // raygui style: dark
#include "styles/style_cherry.h"            // raygui style: cherry
#include "styles/style_sunny.h"             // raygui style: sunny
#include "styles/style_enefete.h"           // raygui style: enefete
#include "styles/style_amber.h"             // raygui style: amber
#include "styles/style_rltech.h"            // raygui style: rltech
#include "styles/style_genesis.h"           // raygui style: genesis

//--- global variables
SLUGmaker_MenuVariables menu_vars;

int8_t SLUGmaker_MenuInit()
{
	menu_vars.map_selection_menu = true;
	menu_vars.map_selection_result = -1;
	return 0;
}

int8_t SLUGmaker_MenuUnload()
{
	return 0;
}


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

    RectangleMultiply(&(button->zone),factor_x, factor_y);

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

//---combo box
int8_t SLUGmaker_ComboBoxLoad(Rectangle zone, int32_t default_state, const char *options, SLUGmaker_ComboBox *combobox)
{	
	if(combobox == NULL)
		return -1;

	combobox->zone = zone;
	
	strncpy(combobox->options, options, 1023);
	int32_t state_nb = 1;
	for(uint32_t u = 0; u < strlen(options); ++u)
	{
		if(options[u] == ';')
			state_nb++;
	}
	
	if(default_state < 0 || default_state >= state_nb)
		combobox->state = 0;
	else
		combobox->state = default_state;
	
	return 0;
}

int8_t SLUGmaker_ComboBoxResize(float factor_x, float factor_y, SLUGmaker_ComboBox *combobox)
{
	if(combobox == NULL)
		return -1;
		
	RectangleMultiply(&(combobox->zone),factor_x, factor_y);
		
	return 0;
}


int8_t SLUGmaker_ComboBoxPressed(SLUGmaker_ComboBox *combobox)
{
	if(combobox == NULL)
		return -1;
		
	GuiComboBox(combobox->zone, combobox->options, &(combobox->state));
		
	return 0;
}

//---toolbar
SLUGmaker_ToolBar SLUGmaker_ToolBarDevLoad()
{
    SLUGmaker_ToolBar toolbar;
    
    toolbar.zone = (Rectangle) {.x = 0, .y = 0, .width = GetScreenWidth(), .height = GetScreenHeight() * 0.03f};
    
    toolbar.style_zone = (Rectangle) {.x = toolbar.zone.x + toolbar.zone.width * 0.2f, .y = toolbar.zone.y, .width = toolbar.zone.width * 0.15f, .height = toolbar.zone.height};
    toolbar.style_label_zone = (Rectangle) {.x = toolbar.style_zone.x + toolbar.style_zone.width * 0.05f, .y = toolbar.style_zone.y, .width = toolbar.style_zone.width * 0.25f, .height = toolbar.style_zone.height};
    
    SLUGmaker_ButtonLoad((Rectangle){.x = toolbar.zone.width * 0.0025f,
    								 .y = GetScreenHeight() * 0.0025f, 
    								 .width = GetScreenHeight() * 0.025f, 
    								 .height = GetScreenHeight() * 0.025f},
    								 ICON_FILE_SAVE,
    								 &toolbar.save);
    
    SLUGmaker_ComboBoxLoad((Rectangle){.x = toolbar.style_label_zone.x + toolbar.style_label_zone.width,
    								   .y = GetScreenHeight() * 0.0025f, 
    								   .width = toolbar.style_zone.width * 0.65f, 
    								   .height = GetScreenHeight() * 0.025f},
    								   0, 
    								   "Light;Jungle;Candy;Lavanda;Cyber;Terminal;Ashes;Bluish;Dark;Cherry;Sunny;Enefete;Amber;Genesis",
    								   &toolbar.styles);
    
    return toolbar;
}

int8_t SLUGmaker_ToolBarResize(float factor_x, float factor_y, SLUGmaker_ToolBar *toolbar)
{
    if(toolbar == NULL)
        return -1;
        
    RectangleMultiply(&(toolbar->zone),factor_x, factor_y);
    RectangleMultiply(&(toolbar->style_zone),factor_x, factor_y);
    RectangleMultiply(&(toolbar->style_label_zone),factor_x, factor_y);

    SLUGmaker_ButtonResize(factor_x, factor_y, &(toolbar->save));
    SLUGmaker_ComboBoxResize(factor_x, factor_y, &(toolbar->styles));

    return 0;
}

int8_t SLUGmaker_ToolBarButtonPressed(SLUGmaker_ToolBar *toolbar)
{
	if(toolbar == NULL)
		return -1;

	GuiSetTooltip("Save map (CTRL+S)");
    SLUGmaker_ButtonPressed(&(toolbar->save));
    
    GuiSetTooltip("Choose interface style");
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_WIDTH, 50);
    SLUGmaker_ComboBoxPressed(&(toolbar->styles));

    GuiSetTooltip(NULL);
    
    return 0;
}

int8_t SLUGmaker_ToolBarDisplay(SLUGmaker_ToolBar *toolbar)
{
    if(toolbar == NULL)
        return -1;

    GuiPanel(toolbar->zone, NULL);
    GuiPanel(toolbar->style_zone, NULL);
    GuiLabel(toolbar->style_label_zone, "Styles : ");

    int8_t err = SLUGmaker_ToolBarButtonPressed(toolbar);
    if(err < 0)
        return err;

    return 0;
}

//--- action buttons menu
SLUGmaker_ActionButtonsMenu SLUGmaker_ActionButtonsMenuDevLoad()
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
	
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.group_zone.x + menu.group_zone.width * 0.015f, .y = menu.group_zone.y + menu.group_zone.height * 0.05f, .width = 27, .height = 27},ICON_CURSOR_CLASSIC,&menu.none_mode);
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.none_mode.zone.x + menu.group_zone.width * 0.015f + menu.none_mode.zone.width, .y = menu.group_zone.y + menu.group_zone.height * 0.05f, .width = menu.none_mode.zone.width, .height = menu.none_mode.zone.height},ICON_WALL,&menu.wall_mode);
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.wall_mode.zone.x + menu.group_zone.width * 0.015f + menu.none_mode.zone.width, .y = menu.group_zone.y + menu.group_zone.height * 0.05f, .width = menu.none_mode.zone.width, .height = menu.none_mode.zone.height},ICON_PLAYER,&menu.player_mode);
	SLUGmaker_ButtonLoad((Rectangle){.x = menu.player_mode.zone.x + menu.group_zone.width * 0.015f + menu.none_mode.zone.width, .y = menu.group_zone.y + menu.group_zone.height * 0.05f, .width = menu.none_mode.zone.width, .height = menu.none_mode.zone.height},ICON_DELETE,&menu.delete_mode);
	
	return menu;
}

int8_t SLUGmaker_ActionButtonsMenuResize(float factor_x, float factor_y, SLUGmaker_ActionButtonsMenu *menu)
{
	if(menu == NULL)
		return -1;
		
	RectangleMultiply(&(menu->zone),factor_x,factor_y);
	RectangleMultiply(&(menu->group_zone),factor_x,factor_y);

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

//general menu
SLUGmaker_Menu general_menus;

int8_t SLUGmaker_MenuDevLoad()
{
    general_menus.toolbar = SLUGmaker_ToolBarDevLoad();
    general_menus.actionMenu = SLUGmaker_ActionButtonsMenuDevLoad();
    return 0;
}

int8_t SLUGmaker_MenuResize(float factor_x, float factor_y)
{
	int8_t error = SLUGmaker_ToolBarResize(factor_x, factor_y, &(general_menus.toolbar));
    if(error == -1)
       	return -1;
       	
    error = SLUGmaker_ActionButtonsMenuResize(factor_x, factor_y, &(general_menus.actionMenu));
    if(error == -1)
       	return -1;
       	
    return 0;
}

int8_t SLUGmaker_MenuDisplay()
{
	int8_t error = SLUGmaker_ActionButtonsMenuDisplay(&(general_menus.actionMenu));   
	if(error == -1)
		return -1;

	error = SLUGmaker_ToolBarDisplay(&(general_menus.toolbar));   
	if(error == -1)
		return -1;		        
		
	return 0;
}

//pop-ups
int8_t SLUGmaker_PopUpNewMap()
{
	if(menu_vars.map_selection_menu)
	{
		int32_t res = GuiMessageBox((Rectangle){ (float)GetScreenWidth()/2 - 125, (float)GetScreenHeight()/2 - 50, 250, 100 }, GuiIconText(ICON_EXIT, "Close Window"), "Do you want to open or create a new map ?", "Open;New map");
		menu_vars.map_selection_result = res;
		if(res != -1)
			menu_vars.map_selection_menu = false;
    }
    else
    	menu_vars.map_selection_result = -1;
    return 0;
}

//---functions
int8_t SLUGmaker_ChangeGUIStyle()
{
	if(graphic_vars.style != general_menus.toolbar.styles.state)
	{
		switch (general_menus.toolbar.styles.state)
        {
        	case 0: GuiLoadStyleDefault(); break;
            case 1: GuiLoadStyleJungle(); break;
            case 2: GuiLoadStyleCandy(); break;
            case 3: GuiLoadStyleLavanda(); break;
            case 4: GuiLoadStyleCyber(); break;
            case 5: GuiLoadStyleTerminal(); break;
            case 6: GuiLoadStyleAshes(); break;
            case 7: GuiLoadStyleBluish(); break;
            case 8: GuiLoadStyleDark(); break;
            case 9: GuiLoadStyleCherry(); break;
            case 10: GuiLoadStyleSunny(); break;
            case 11: GuiLoadStyleEnefete(); break;
            case 12: GuiLoadStyleAmber(); break;
            case 13: GuiLoadStyleRltech(); break;
            case 14: GuiLoadStyleGenesis(); break;
            default: break;
        }
		graphic_vars.style = general_menus.toolbar.styles.state;
	}
	return 0;
}
