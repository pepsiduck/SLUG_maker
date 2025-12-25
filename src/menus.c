#include "menus.h"
#include "defines.h"
#include "geometry.h"
#include "action.h"
#include "map.h"

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
    button->tooltip[0] = '\0';
    return 0;
}

int8_t SLUGmaker_ButtonSetToolTip(const char * text, SLUGmaker_button *button)
{
    if(text == NULL || button == NULL)
        return -1;

    strncpy(button->tooltip, text, 256);

    return 0;
}

int8_t SLUGmaker_ButtonResize(float factor_x, float factor_y, SLUGmaker_button *button)
{
    if(button == NULL)
        return -1;

    RectangleMultiply(&(button->zone),factor_x, factor_y);

    return 0;
}

int8_t SLUGmaker_ButtonPressed(SLUGmaker_button *button, bool tooltip)
{
    if(button == NULL)
        return -1;

    if(tooltip)
        GuiSetTooltip(button->tooltip);

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

int8_t SLUGmaker_ComboBoxSetToolTip(const char * text, SLUGmaker_ComboBox *combobox)
{
    if(text == NULL || combobox == NULL)
        return -1;

    strncpy(combobox->tooltip, text, 256);

    return 0;
}

int8_t SLUGmaker_ComboBoxResize(float factor_x, float factor_y, SLUGmaker_ComboBox *combobox)
{
	if(combobox == NULL)
		return -1;
		
	RectangleMultiply(&(combobox->zone),factor_x, factor_y);
		
	return 0;
}


int8_t SLUGmaker_ComboBoxPressed(SLUGmaker_ComboBox *combobox, bool tooltip)
{
    if(combobox == NULL)
        return -1;
		
    if(tooltip)
        GuiSetTooltip(combobox->tooltip);

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

    SLUGmaker_ButtonSetToolTip("Save map (CTRL+S)", &toolbar.save);
    
    SLUGmaker_ComboBoxLoad((Rectangle){.x = toolbar.style_label_zone.x + toolbar.style_label_zone.width,
    								   .y = GetScreenHeight() * 0.0025f, 
    								   .width = toolbar.style_zone.width * 0.65f, 
    								   .height = GetScreenHeight() * 0.025f},
    								   0, 
    								   "Light;Jungle;Candy;Lavanda;Cyber;Terminal;Ashes;Bluish;Dark;Cherry;Sunny;Enefete;Amber;Genesis",
    								   &toolbar.styles);

    SLUGmaker_ComboBoxSetToolTip("Choose interface style", &toolbar.styles);
    
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

    SLUGmaker_ButtonPressed(&(toolbar->save), true);
    
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_WIDTH, 50);
    SLUGmaker_ComboBoxPressed(&(toolbar->styles), true);

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

    menu.icons[0] = ICON_CURSOR_CLASSIC;
    menu.icons[1] = ICON_WALL;
    menu.icons[2] = ICON_PLAYER;   
    menu.icons[3] = ICON_DELETE; 
    menu.icons[4] = ICON_SPRITE;
	
    for(uint16_t u = 0; u < ACTION_MENU_NB; ++u)
        SLUGmaker_ButtonLoad((Rectangle){.x = menu.group_zone.x + menu.group_zone.width * 0.015f + (u % 10)*(27 + 0.05f * menu.zone.width), .y = menu.group_zone.y + menu.group_zone.height * 0.05f + (u / 10) * (27 + 0.05f * menu.zone.height), .width = 27, .height = 27},menu.icons[u],&(menu.modes[u]));

    SLUGmaker_ButtonSetToolTip("No editing (N)", &menu.modes[0]);
    SLUGmaker_ButtonSetToolTip("Place and move walls (W)", &menu.modes[1]);
    SLUGmaker_ButtonSetToolTip("Move Player spawn point (P)", &menu.modes[2]);
    SLUGmaker_ButtonSetToolTip("Delete elements (BACKSPACE)", &menu.modes[3]);
    SLUGmaker_ButtonSetToolTip("Place and load sprites (S)", &menu.modes[4]);
	
	return menu;
}

int8_t SLUGmaker_ActionButtonsMenuResize(float factor_x, float factor_y, SLUGmaker_ActionButtonsMenu *menu)
{
    if(menu == NULL)
        return -1;
		
    RectangleMultiply(&(menu->zone),factor_x,factor_y);
    RectangleMultiply(&(menu->group_zone),factor_x,factor_y);

    for(uint16_t u = 0; u < ACTION_MENU_NB; ++u)
        SLUGmaker_ButtonResize(factor_x, factor_y, &(menu->modes[u]));
		
    return 0;
}

int8_t SLUGmaker_ActionButtonsMenuPressed(SLUGmaker_ActionButtonsMenu *menu)
{
	if(menu == NULL)
		return -1;
		
	for(uint16_t u = 0; u < ACTION_MENU_NB; ++u)
	    SLUGmaker_ButtonPressed(&(menu->modes[u]), true);
	
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

//Info menu
SLUGmaker_InfoMenu SLUGmaker_InfoMenuDevLoad()
{
    SLUGmaker_InfoMenu info_menu;


    info_menu.panel_zone = (Rectangle) {
        .x = 0,
		.y = GetScreenHeight() * 0.23f,
		.width = GetScreenWidth() * 0.2f,
		.height = GetScreenHeight() * 0.2f,
    };

    info_menu.text_zone = (Rectangle) {
        .x = info_menu.panel_zone.x + 0.05f * info_menu.panel_zone.width,
		.y = info_menu.panel_zone.y + 0.05f * info_menu.panel_zone.height,
		.width = 0.9f * info_menu.panel_zone.width,
		.height = 0.9f * info_menu.panel_zone.height,
    };

    return info_menu;
}

int8_t SLUGmaker_InfoMenuResize(float factor_x, float factor_y, SLUGmaker_InfoMenu *info_menu)
{
    if(info_menu == NULL)
		return -1;
		
	RectangleMultiply(&(info_menu->panel_zone),factor_x,factor_y);
	RectangleMultiply(&(info_menu->text_zone),factor_x,factor_y);

    return 0;
}

int8_t SLUGmaker_InfoMenuDisplay(SLUGmaker_InfoMenu *info_menu, void *ptr)
{
    if(info_menu == NULL)
		return -1;

    GuiGroupBox(info_menu->panel_zone,"Action info");

    SLUGmaker_InfoMenuPrintFct[current_action](ptr, info_menu->text_zone);

    return 0;
}

void (*SLUGmaker_InfoMenuPrintFct[ACTION_MENU_NB])(void *,Rectangle) = {
    SLUGmaker_InfoMenuPrintNone,
    SLUGmaker_InfoMenuPrintWall,
    SLUGmaker_InfoMenuPrintPlayerSpawn,
    SLUGmaker_InfoMenuPrintDelete,
    SLUGmaker_InfoMenuPrintSprites
};

void SLUGmaker_InfoMenuPrintNone(void *ptr, Rectangle bounds)
{
    GuiLabelButton(bounds, "Press right click to move around. You can do that in any mode.");
}

void SLUGmaker_InfoMenuPrintWall(void *ptr, Rectangle bounds)
{
    char msg[256]; //watch out for the size
    SLUGmaker_map *map = (SLUGmaker_map *) ptr;
    sprintf(msg, "Left Click to place a new wall chain.\nShift + Left Click to move wall nodes\nRight click to select a wall node\nNumber of walls nodes : %u/%u .", map->wall_nb, MAX_WALLS_NB);
    GuiLabelButton(bounds, msg);
}

void SLUGmaker_InfoMenuPrintPlayerSpawn(void *ptr, Rectangle bounds)
{
    char msg[256]; //watch out for the size
    SLUGmaker_map *map = (SLUGmaker_map *) ptr;
    sprintf(msg, "Left Click to place the player's spawn.\nCoordinates of player spawn : %.2f ; %.2f .", map->player_spawn_point.x, map->player_spawn_point.y);
    GuiLabelButton(bounds, msg);
}

void SLUGmaker_InfoMenuPrintDelete(void *ptr, Rectangle bounds)
{
    GuiLabelButton(bounds, "Press leftt click to delete stuff.");
}

void SLUGmaker_InfoMenuPrintSprites(void *ptr, Rectangle bounds)
{
    char msg[256]; //watch out for the size
    SLUGmaker_map *map = (SLUGmaker_map *) ptr;
    sprintf(msg, "Left Click to place a rectangle sprite.\nShift + Left Click to move a sprite\nRight click to select a sprite\nNumber of sprites placed : %u", map->sprite_nb);
    GuiLabelButton(bounds, msg);
}

//general menu
SLUGmaker_Menu general_menus;

int8_t SLUGmaker_MenuDevLoad()
{
    general_menus.toolbar = SLUGmaker_ToolBarDevLoad();
    general_menus.actionMenu = SLUGmaker_ActionButtonsMenuDevLoad();
    general_menus.infoMenu = SLUGmaker_InfoMenuDevLoad();

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

    error = SLUGmaker_InfoMenuResize(factor_x, factor_y, &(general_menus.infoMenu));
    if(error == -1)
       	return -1;
       	
    return 0;
}

int8_t SLUGmaker_MenuDisplay(void *ptr)
{
	int8_t error = SLUGmaker_ActionButtonsMenuDisplay(&(general_menus.actionMenu));   
	if(error == -1)
		return -1;

	error = SLUGmaker_ToolBarDisplay(&(general_menus.toolbar));   
	if(error == -1)
		return -1;	

    error = SLUGmaker_InfoMenuDisplay(&(general_menus.infoMenu), ptr) ; 
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
