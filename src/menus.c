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

int8_t SLUGmaker_ButtonLoad(Rectangle zone, uint16_t guiIcon, bool icon_button, SLUGmaker_Button *button)
{
    if(button == NULL)
        return -1;
    button->zone = zone;
    button->guiIcon = guiIcon;
    button->icon_button = icon_button;
    button->pressed = false;
    button->text[0] = '\0';
    return 0;
}

int8_t SLUGmaker_ButtonSetText(const char * text, SLUGmaker_Button *button)
{
    if(text == NULL || button == NULL)
        return -1;

    strncpy(button->text, text, 256);

    return 0;
}

int8_t SLUGmaker_ButtonResize(float factor_x, float factor_y, SLUGmaker_Button *button)
{
    if(button == NULL)
        return -1;

    RectangleMultiply(&(button->zone),factor_x, factor_y);

    return 0;
}

int8_t SLUGmaker_ButtonPressed(SLUGmaker_Button *button, bool tooltip)
{
    if(button == NULL)
        return -1;

    if(button->icon_button)
    {
        if(tooltip)
            GuiSetTooltip(button->text);

        sprintf(SLUGmaker_IconBuffer,"#%u#",button->guiIcon);
        button->pressed = GuiButton(button->zone, SLUGmaker_IconBuffer);
    }
    else
        button->pressed = GuiButton(button->zone, button->text);
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

//---list view
int8_t SLUGmaker_ListViewLoad(Rectangle zone, char **options, uint16_t *nb, int scrollIndex, int active, int focus, SLUGmaker_ListView *listview)
{
    if(listview == NULL)
        return -1;

    listview->zone = zone;
    printf("%f %f %f %f\n", listview->zone.x, listview->zone.y, listview->zone.width, listview->zone.height);
    listview->options = (const char **) options;
    listview->nb = nb;
    listview->scrollIndex = scrollIndex;
    listview->active = active;
    listview->focus = focus;

    return 0;
}

int8_t SLUGmaker_ListViewResize(float factor_x, float factor_y, SLUGmaker_ListView *listview)
{
    if(listview == NULL)
        return -1;

    RectangleMultiply(&(listview->zone),factor_x, factor_y);

    return 0;
}

int8_t SLUGmaker_ListViewPressed(SLUGmaker_ListView *listview)
{
    if(listview == NULL)
        return -1;
    
    GuiListViewEx(listview->zone, listview->options, *(listview->nb), &(listview->scrollIndex), &(listview->active), &(listview->focus));

    return 0;
}

//! Menus
SLUGmaker_Menu* SLUGmaker_MenuDevLoad(SLUGmaker_MenuType menu_type, Rectangle zone, size_t size)
{
    SLUGmaker_Menu *menu = (SLUGmaker_Menu*) malloc(size);

    menu->menu_type = menu_type;
    menu->zone = zone;

    return menu;
}

int8_t SLUGmaker_MenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    RectangleMultiply(&(menu->zone),factor_x, factor_y);

    return 0;
}

//!!The menus are ordred from bottom to top
//!The order has to be the same on all arrays
SLUGmaker_Menu* (*SLUGmaker_MenuDevLoadFunctions[MENU_NUMBER])(void *ptr) = {
    SLUGmaker_ActionModifMenuDevLoad,
    SLUGmaker_ModifMenuDevLoad,
    SLUGmaker_LogsMenuDevLoad,
    SLUGmaker_InfoMenuDevLoad,
    SLUGmaker_ActionButtonsMenuDevLoad,
    SLUGmaker_ToolBarDevLoad
};

int8_t (*SLUGmaker_MenuFreeFunctions[MENU_NUMBER])(SLUGmaker_Menu *menu) = {
    SLUGmaker_ActionModifMenuFree,
    SLUGmaker_ModifMenuFree,
    SLUGmaker_LogsMenuFree,
    SLUGmaker_InfoMenuFree,
    SLUGmaker_ActionButtonsMenuFree,
    SLUGmaker_ToolBarFree
};

int8_t (*SLUGmaker_MenuResizeFunctions[MENU_NUMBER])(float factor_x, float factor_y, SLUGmaker_Menu *menu) = {
    SLUGmaker_ActionModifMenuResize,
    SLUGmaker_ModifMenuResize,
    SLUGmaker_LogsMenuResize,
    SLUGmaker_InfoMenuResize,
    SLUGmaker_ActionButtonsMenuResize,
    SLUGmaker_ToolBarResize,
};

int8_t (*SLUGmaker_MenuPressedFunctions[MENU_NUMBER])(SLUGmaker_Menu *menu) = {
    SLUGmaker_ActionModifMenuPressed,
    SLUGmaker_ModifMenuPressed,
    SLUGmaker_LogsMenuPressed,
    SLUGmaker_InfoMenuPressed,
    SLUGmaker_ActionButtonsMenuPressed,
    SLUGmaker_ToolBarPressed
};

int8_t (*SLUGmaker_MenuDisplayFunctions[MENU_NUMBER])(SLUGmaker_Menu *menu, void *ptr) = {
    SLUGmaker_ActionModifMenuDisplay,
    SLUGmaker_ModifMenuDisplay,
    SLUGmaker_LogsMenuDisplay,
    SLUGmaker_InfoMenuDisplay,
    SLUGmaker_ActionButtonsMenuDisplay,
    SLUGmaker_ToolBarDisplay
};

//---toolbar
SLUGmaker_Menu* SLUGmaker_ToolBarDevLoad(void *ptr)
{
    SLUGmaker_ToolBar *toolbar = (SLUGmaker_ToolBar *) SLUGmaker_MenuDevLoad(MENU_TOOLBAR, (Rectangle) {.x = 0, .y = 0, .width = GetScreenWidth(), .height = GetScreenHeight() * 0.03f}, sizeof(SLUGmaker_ToolBar));
    
    toolbar->style_zone = (Rectangle) {.x = toolbar->m.zone.x + toolbar->m.zone.width * 0.2f, .y = toolbar->m.zone.y, .width = toolbar->m.zone.width * 0.15f, .height = toolbar->m.zone.height};
    toolbar->style_label_zone = (Rectangle) {.x = toolbar->style_zone.x + toolbar->style_zone.width * 0.05f, .y = toolbar->style_zone.y, .width = toolbar->style_zone.width * 0.25f, .height = toolbar->style_zone.height};
    
    SLUGmaker_ButtonLoad((Rectangle){.x = toolbar->m.zone.width * 0.0025f,
    								 .y = GetScreenHeight() * 0.0025f, 
    								 .width = GetScreenHeight() * 0.025f, 
    								 .height = GetScreenHeight() * 0.025f},
    								 ICON_FILE_SAVE,
                                     true,
    								 &(toolbar->save));

    SLUGmaker_ButtonSetText("Save map (CTRL+S)", &(toolbar->save));
    
    SLUGmaker_ComboBoxLoad((Rectangle){.x = toolbar->style_label_zone.x + toolbar->style_label_zone.width,
    								   .y = GetScreenHeight() * 0.0025f, 
    								   .width = toolbar->style_zone.width * 0.65f, 
    								   .height = GetScreenHeight() * 0.025f},
    								   4, 
    								   "Light;Jungle;Candy;Lavanda;Cyber;Terminal;Ashes;Bluish;Dark;Cherry;Sunny;Enefete;Amber;Genesis",
    								   &(toolbar->styles));

    SLUGmaker_ComboBoxSetToolTip("Choose interface style", &(toolbar->styles));
    
    return (SLUGmaker_Menu*) toolbar;
}

int8_t SLUGmaker_ToolBarFree(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_ToolBar *toolbar = (SLUGmaker_ToolBar *) menu;

    free(toolbar);

    return 0;
}

int8_t SLUGmaker_ToolBarResize(float factor_x, float factor_y, SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_MenuResize(factor_x,factor_y,menu);
    SLUGmaker_ToolBar *toolbar = (SLUGmaker_ToolBar *) menu; 
        
    RectangleMultiply(&(toolbar->style_zone),factor_x, factor_y);
    RectangleMultiply(&(toolbar->style_label_zone),factor_x, factor_y);

    SLUGmaker_ButtonResize(factor_x, factor_y, &(toolbar->save));
    SLUGmaker_ComboBoxResize(factor_x, factor_y, &(toolbar->styles));

    return 0;
}

int8_t SLUGmaker_ToolBarPressed(SLUGmaker_Menu *menu)
{
	if(menu == NULL)
        return -1;

    SLUGmaker_ToolBar *toolbar = (SLUGmaker_ToolBar *) menu; 
    
    SLUGmaker_ButtonPressed(&(toolbar->save), true);
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_WIDTH, 50);
    SLUGmaker_ComboBoxPressed(&(toolbar->styles), true);

    GuiSetTooltip(NULL);
    
    return 0;
}

int8_t SLUGmaker_ToolBarDisplay(SLUGmaker_Menu *menu, void *ptr)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_ToolBar *toolbar = (SLUGmaker_ToolBar *) menu; 

    GuiPanel(toolbar->m.zone, NULL);
    GuiPanel(toolbar->style_zone, NULL);
    GuiLabel(toolbar->style_label_zone, "Styles : ");

    int8_t err = SLUGmaker_ToolBarPressed(menu);
    if(err < 0)
        return err;

    return 0;
}

//--- action buttons menu
SLUGmaker_Menu* SLUGmaker_ActionButtonsMenuDevLoad(void *ptr)
{
    Rectangle zone = (Rectangle) {
        .x = 0,
		.y = GetScreenHeight() * 0.03f,
		.width = GetScreenWidth() * 0.2f,
		.height = GetScreenHeight() * 0.2f,
    };

	SLUGmaker_ActionButtonsMenu *menu = (SLUGmaker_ActionButtonsMenu *) SLUGmaker_MenuDevLoad(MENU_ACTION, (Rectangle) {.x = zone.x + 20.0f,.y = zone.y + 11.0f,.width = zone.width - 40.0f,.height = zone.height - 22.0f}, sizeof(SLUGmaker_ActionButtonsMenu));

    menu->icons[0] = ICON_CURSOR_CLASSIC;
    menu->icons[1] = ICON_WALL;
    menu->icons[2] = ICON_PLAYER;   
    menu->icons[3] = ICON_DELETE; 
    menu->icons[4] = ICON_SPRITE;
	
    for(uint16_t u = 0; u < ACTION_MENU_NB; ++u)
        SLUGmaker_ButtonLoad((Rectangle){.x = menu->m.zone.x + menu->m.zone.width * 0.015f + (u % 10)*(27 + menu->m.zone.width * 0.015f), .y = menu->m.zone.y + menu->m.zone.height * 0.05f + (u / 10) * (27 + menu->m.zone.height * 0.015f), .width = 27, .height = 27},menu->icons[u],true,&(menu->modes[u]));

    SLUGmaker_ButtonSetText("No editing (N)", &(menu->modes[0]));
    SLUGmaker_ButtonSetText("Place and move walls (W)", &(menu->modes[1]));
    SLUGmaker_ButtonSetText("Move Player spawn point (P)", &(menu->modes[2]));
    SLUGmaker_ButtonSetText("Delete elements (BACKSPACE)", &(menu->modes[3]));
    SLUGmaker_ButtonSetText("Place and load sprites (S)", &(menu->modes[4]));
	
	return (SLUGmaker_Menu*) menu;
}

int8_t SLUGmaker_ActionButtonsMenuFree(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_ActionButtonsMenu *action_menu = (SLUGmaker_ActionButtonsMenu *) menu;

    free(action_menu);

    return 0;
}

int8_t SLUGmaker_ActionButtonsMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_MenuResize(factor_x,factor_y,menu);
    SLUGmaker_ActionButtonsMenu *action_menu = (SLUGmaker_ActionButtonsMenu *) menu;

    for(uint16_t u = 0; u < ACTION_MENU_NB; ++u)
        SLUGmaker_ButtonResize(factor_x, factor_y, &(action_menu->modes[u]));
		
    return 0;
}

int8_t SLUGmaker_ActionButtonsMenuPressed(SLUGmaker_Menu *menu)
{
	if(menu == NULL)
		return -1;

    SLUGmaker_ActionButtonsMenu *action_menu = (SLUGmaker_ActionButtonsMenu *) menu;
		
	for(uint16_t u = 0; u < ACTION_MENU_NB; ++u)
	    SLUGmaker_ButtonPressed(&(action_menu->modes[u]), true);
	
	GuiSetTooltip(NULL);
		
	return 0;
}

int8_t SLUGmaker_ActionButtonsMenuDisplay(SLUGmaker_Menu *menu, void *ptr)
{
	if(menu == NULL)
		return -1;

    SLUGmaker_ActionButtonsMenu *action_menu = (SLUGmaker_ActionButtonsMenu *) menu;
		
	GuiGroupBox(action_menu->m.zone,"Action controls");

    int8_t err = SLUGmaker_ActionButtonsMenuPressed(menu);
    if(err < 0)
        return err;
		
    return 0;
}

//Info menu
SLUGmaker_Menu* SLUGmaker_InfoMenuDevLoad(void *ptr)
{
    
    Rectangle zone = (Rectangle) {
        .x = 0,
		.y = GetScreenHeight() * 0.23f,
		.width = GetScreenWidth() * 0.2f,
		.height = GetScreenHeight() * 0.2f,
    };

    SLUGmaker_InfoMenu *info_menu = (SLUGmaker_InfoMenu *) SLUGmaker_MenuDevLoad(MENU_INFO,(Rectangle) { .x = zone.x + 20.0f,.y = zone.y + 11.0f,.width = zone.width - 40.0f,.height = zone.height - 22.0f}, sizeof(SLUGmaker_InfoMenu));

    info_menu->text_zone = (Rectangle) {
        .x = info_menu->m.zone.x + 20.0f,
		.y = info_menu->m.zone.y + 11.0f,
		.width = info_menu->m.zone.width - 40.0f,
		.height = info_menu->m.zone.height - 22.0f,
    };

    return (SLUGmaker_Menu*) info_menu;
}

int8_t SLUGmaker_InfoMenuFree(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
		return -1; 

    SLUGmaker_InfoMenu *info_menu = (SLUGmaker_InfoMenu *) menu;  

    free(info_menu);

    return 0;
}

int8_t SLUGmaker_InfoMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu)
{
    if(menu == NULL)
		return -1; 

	SLUGmaker_MenuResize(factor_x,factor_y,menu);
    SLUGmaker_InfoMenu *info_menu = (SLUGmaker_InfoMenu *) menu;   

	RectangleMultiply(&(info_menu->text_zone),factor_x,factor_y);

    return 0;
}

int8_t SLUGmaker_InfoMenuPressed(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
		return -1; 

    //SLUGmaker_InfoMenu *info_menu = (SLUGmaker_InfoMenu *) menu;  

    return 0;
}

int8_t SLUGmaker_InfoMenuDisplay(SLUGmaker_Menu *menu, void *ptr)
{
    if(menu == NULL)
		return -1; 

    SLUGmaker_InfoMenu *info_menu = (SLUGmaker_InfoMenu *) menu;  

    GuiGroupBox(info_menu->m.zone,"Action info");

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
    GuiLabel(bounds, "Press right click to move around. You can do that in any mode.");
}

void SLUGmaker_InfoMenuPrintWall(void *ptr, Rectangle bounds)
{
    char msg[256]; //watch out for the size
    SLUGmaker_map *map = (SLUGmaker_map *) ptr;
    sprintf(msg, "Left Click to place a new wall chain.\nShift + Left Click to move wall nodes\nRight click to select a wall node\nNumber of walls nodes : %u/%u .", map->wall_nb, MAX_WALLS_NB);
    GuiLabel(bounds, msg);
}

void SLUGmaker_InfoMenuPrintPlayerSpawn(void *ptr, Rectangle bounds)
{
    char msg[256]; //watch out for the size
    SLUGmaker_map *map = (SLUGmaker_map *) ptr;
    sprintf(msg, "Left Click to place the player's spawn.\nCoordinates of player spawn : %.2f ; %.2f .", map->player_spawn_point.x, map->player_spawn_point.y);
    GuiLabel(bounds, msg);
}

void SLUGmaker_InfoMenuPrintDelete(void *ptr, Rectangle bounds)
{
    GuiLabel(bounds, "Press left click to delete stuff.");
}

void SLUGmaker_InfoMenuPrintSprites(void *ptr, Rectangle bounds)
{
    char msg[256]; //watch out for the size
    SLUGmaker_map *map = (SLUGmaker_map *) ptr;
    sprintf(msg, "Left Click to place a rectangle sprite.\nShift + Left Click to move a sprite\nRight click to select a sprite\nNumber of sprites placed : %u", map->sprite_nb);
    GuiLabel(bounds, msg);
}

//!Action Modif Menu

//Sprite modif menu
int8_t SLUGmaker_SpriteActionModifMenuLoad(Rectangle *parent_zone, void *ptr, SLUGmaker_SpriteActionModifMenu *sprite_menu)
{
    if(sprite_menu == NULL || parent_zone == NULL || ptr == NULL)
        return -1;

    SLUGmaker_ButtonLoad((Rectangle) {
        .x = parent_zone->x + parent_zone->width * 0.05f,
        .y = parent_zone->y + 11,
        .width = parent_zone->width * 0.9f,
        .height = 30
    }, 0, false, &(sprite_menu->load_sprite_button));

    SLUGmaker_ButtonSetText("Load a Sprite", &(sprite_menu->load_sprite_button));

    SLUGmaker_map *map = (SLUGmaker_map *) ptr;

    SLUGmaker_ListViewLoad((Rectangle) {
        .x = parent_zone->x + parent_zone->width * 0.05f,
        .y = sprite_menu->load_sprite_button.zone.y + sprite_menu->load_sprite_button.zone.height + 11,
        .width = parent_zone->width * 0.9f,
        .height = 215
    },map->loaded_sprites_names, (uint16_t *) &(map->loaded_sprites_nb), 0, -1, -1, &(sprite_menu->sprite_list));

    sprite_menu->sprite_display = (Rectangle) {
        .x = parent_zone->x + parent_zone->width * 0.05f,
        .y = sprite_menu->sprite_list.zone.y + sprite_menu->sprite_list.zone.height + 11,
        .width = parent_zone->width * 0.9f,
        .height = 215
    };

    SLUGmaker_ButtonLoad((Rectangle) {
        .x = parent_zone->x + parent_zone->width * 0.05f,
        .y = sprite_menu->sprite_display.y + sprite_menu->sprite_display.height + 11,
        .width = parent_zone->width * 0.9f,
        .height = 30
    }, 0, false, &(sprite_menu->place_sprite_button));

    SLUGmaker_ButtonSetText("Place Sprite", &(sprite_menu->place_sprite_button));

    SLUGmaker_ButtonLoad((Rectangle) {
        .x = parent_zone->x + parent_zone->width * 0.05f,
        .y = sprite_menu->place_sprite_button.zone.y + sprite_menu->place_sprite_button.zone.height + 11,
        .width = parent_zone->width * 0.9f,
        .height = 30
    }, 0, false, &(sprite_menu->delete_sprite_button));

    SLUGmaker_ButtonSetText("Delete Sprite", &(sprite_menu->delete_sprite_button));

    return 0;
}

int8_t SLUGmaker_SpriteActionModifMenuResize(float factor_x, float factor_y, SLUGmaker_SpriteActionModifMenu *sprite_menu)
{
    if(sprite_menu == NULL)
        return -1;

    SLUGmaker_ButtonResize(factor_x, factor_y, &(sprite_menu->load_sprite_button));
    SLUGmaker_ListViewResize(factor_x, factor_y, &(sprite_menu->sprite_list));
    RectangleMultiply(&(sprite_menu->sprite_display), factor_x, factor_y);
    SLUGmaker_ButtonResize(factor_x, factor_y, &(sprite_menu->place_sprite_button));
    SLUGmaker_ButtonResize(factor_x, factor_y, &(sprite_menu->delete_sprite_button));

    return 0;
}

int8_t SLUGmaker_SpriteActionModifMenuPressed(SLUGmaker_SpriteActionModifMenu *sprite_menu)
{
    if(sprite_menu == NULL)
        return -1;

    SLUGmaker_ButtonPressed(&(sprite_menu->load_sprite_button), false);
    SLUGmaker_ListViewPressed(&(sprite_menu->sprite_list));

    if(sprite_menu->sprite_list.active != -1)
    {
        SLUGmaker_ButtonPressed(&(sprite_menu->place_sprite_button), false);
        SLUGmaker_ButtonPressed(&(sprite_menu->delete_sprite_button), false);
    }

    return 0;
}

int8_t SLUGmaker_SpriteActionModifMenuDisplay(SLUGmaker_SpriteActionModifMenu *sprite_menu, void *ptr)
{
    if(sprite_menu == NULL)
        return -1;

    int8_t err = SLUGmaker_SpriteActionModifMenuPressed(sprite_menu);
    if(err < 0)
        return err;

    SLUGmaker_map *map = (SLUGmaker_map *) ptr;

    if(sprite_menu->sprite_list.active != -1)
    {
    
        float scale = (float) map->loaded_sprites[sprite_menu->sprite_list.active].width / (float) map->loaded_sprites[sprite_menu->sprite_list.active].height;

        if(scale >= (sprite_menu->sprite_display.width / sprite_menu->sprite_display.height)) //horizontal
        {
            float h = sprite_menu->sprite_display.width / scale;
            DrawTexturePro(map->loaded_sprites[sprite_menu->sprite_list.active],
                (Rectangle) {
                    .x = 0.0f, 
                    .y = 0.0f, 
                    .width = map->loaded_sprites[sprite_menu->sprite_list.active].width, 
                    .height = map->loaded_sprites[sprite_menu->sprite_list.active].height
                }, 
                (Rectangle) {
                    .x = sprite_menu->sprite_display.x,
                    .y = sprite_menu->sprite_display.y + (sprite_menu->sprite_display.height - h)/2.0f,
                    .width = sprite_menu->sprite_display.width,
                    .height = h, 
                }, 
                (Vector2) {.x = 0, .y = 0}, 0, WHITE);
        }
        else //vertical
        {
            float w = sprite_menu->sprite_display.height * scale;
            DrawTexturePro(map->loaded_sprites[sprite_menu->sprite_list.active],
                (Rectangle) {
                    .x = 0.0f, 
                    .y = 0.0f, 
                    .width = map->loaded_sprites[sprite_menu->sprite_list.active].width, 
                    .height = map->loaded_sprites[sprite_menu->sprite_list.active].height
                }, 
                (Rectangle) {
                    .x = sprite_menu->sprite_display.x + (sprite_menu->sprite_display.width - w)/2.0f,
                    .y = sprite_menu->sprite_display.y,
                    .width = w,
                    .height = sprite_menu->sprite_display.height, 
                }, 
                (Vector2) {.x = 0, .y = 0}, 0, WHITE);
        }

    }
        

    return 0;
}

//Action modif menu proper
SLUGmaker_Menu* SLUGmaker_ActionModifMenuDevLoad(void *ptr)
{

    Rectangle zone = (Rectangle) {
        .x = 0,
		.y = GetScreenHeight() * 0.43f,
		.width = GetScreenWidth() * 0.2f,
		.height = GetScreenHeight() * 0.57f,
    };

    SLUGmaker_ActionModifMenu *action_modif_menu = (SLUGmaker_ActionModifMenu *) SLUGmaker_MenuDevLoad(MENU_ACTION_MODIF,(Rectangle) { .x = zone.x + 20.0f,.y = zone.y + 11.0f,.width = zone.width - 40.0f,.height = zone.height - 22.0f}, sizeof(SLUGmaker_ActionModifMenu));

    action_modif_menu->group_zone = (Rectangle) {
        .x = action_modif_menu->m.zone.x + 20.0f,
		.y = action_modif_menu->m.zone.y + 11.0f,
		.width = action_modif_menu->m.zone.width - 40.0f,
		.height = action_modif_menu->m.zone.height - 22.0f,
    };

    SLUGmaker_SpriteActionModifMenuLoad(&(action_modif_menu->m.zone), ptr, &(action_modif_menu->sprite_menu));

    return (SLUGmaker_Menu*) action_modif_menu;   
}

int8_t SLUGmaker_ActionModifMenuFree(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_ActionModifMenu *action_modif_menu = (SLUGmaker_ActionModifMenu *) menu;

    free(action_modif_menu);

    return 0;
}

int8_t SLUGmaker_ActionModifMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_MenuResize(factor_x,factor_y,menu);
    SLUGmaker_ActionModifMenu *action_modif_menu = (SLUGmaker_ActionModifMenu *) menu;

	RectangleMultiply(&(action_modif_menu->group_zone),factor_x,factor_y);
    SLUGmaker_SpriteActionModifMenuResize(factor_x, factor_y, &(action_modif_menu->sprite_menu));

    return 0;
}

int8_t SLUGmaker_ActionModifMenuPressed(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    //SLUGmaker_ActionModifMenu *action_modif_menu = (SLUGmaker_ActionModifMenu *) menu;

    return 0;
}

int8_t SLUGmaker_ActionModifMenuDisplay(SLUGmaker_Menu *menu, void *ptr)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_ActionModifMenu *action_modif_menu = (SLUGmaker_ActionModifMenu *) menu;

    GuiGroupBox(action_modif_menu->m.zone,"Action");

    int8_t err = 0;
    switch(current_action)
    {
        case ACTION_MODE_SPRITE:
            err = SLUGmaker_SpriteActionModifMenuDisplay(&(action_modif_menu->sprite_menu), ptr);
            break;
        default:
            break;
    }

    if(err < 0)
        return err;
    
    return 0;
}

//Modif Menu
SLUGmaker_Menu* SLUGmaker_ModifMenuDevLoad(void *ptr)
{
    Rectangle zone = (Rectangle) {
        .x = GetScreenWidth() * 0.2f,
		.y = GetScreenHeight() * 0.83f,
		.width = GetScreenWidth() * 0.6f,
		.height = GetScreenHeight() * 0.17f,
    };

    SLUGmaker_ModifMenu *modif_menu = (SLUGmaker_ModifMenu *) SLUGmaker_MenuDevLoad(MENU_MODIF,(Rectangle) { .x = zone.x + 20.0f,.y = zone.y + 11.0f,.width = zone.width - 40.0f,.height = zone.height - 22.0f}, sizeof(SLUGmaker_ModifMenu));

    modif_menu->group_zone = (Rectangle) {
        .x = modif_menu->m.zone.x + 20.0f,
		.y = modif_menu->m.zone.y + 11.0f,
		.width = modif_menu->m.zone.width - 40.0f,
		.height = modif_menu->m.zone.height - 22.0f,
    };

    return (SLUGmaker_Menu*) modif_menu; 
}

int8_t SLUGmaker_ModifMenuFree(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_ModifMenu *modif_menu = (SLUGmaker_ModifMenu *) menu;

    free(modif_menu);

    return 0;
}

int8_t SLUGmaker_ModifMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_MenuResize(factor_x,factor_y,menu);
    SLUGmaker_ModifMenu *modif_menu = (SLUGmaker_ModifMenu *) menu;

    RectangleMultiply(&(modif_menu->group_zone),factor_x,factor_y);

    return 0;
}

int8_t SLUGmaker_ModifMenuPressed(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    //SLUGmaker_ModifMenu *modif_menu = (SLUGmaker_ModifMenu *) menu;

    return 0;
}

int8_t SLUGmaker_ModifMenuDisplay(SLUGmaker_Menu *menu, void *ptr)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_ModifMenu *modif_menu = (SLUGmaker_ModifMenu *) menu;

    GuiGroupBox(modif_menu->m.zone,"Modifications");
    
    return 0;
}

//Logs Menu
char logs[MAX_LOG][MAX_LOG_STR];
uint8_t logs_counter = 0;

int8_t SLUGmaker_WriteLog(const char * log, ...)
{
    if(log == NULL)
        return -1;

    char buffer[MAX_LOG_STR];
    va_list args;
    va_start(args, log);
    vsnprintf(buffer, MAX_LOG_STR, log, args);
    vprintf(buffer, args);
    va_end(args);

    if(logs_counter >= MAX_LOG)
    {
        for(uint8_t u = 0; u < MAX_LOG - 1; ++u)
            strncpy(logs[u], logs[u + 1], MAX_LOG_STR);
        strncpy(logs[MAX_LOG - 1], buffer, MAX_LOG_STR);
    }
    else
    {   
        strncpy(logs[logs_counter], buffer, MAX_LOG_STR);
        logs_counter++;
    }

    return 0;
}

int8_t SLUGmaker_FlushLogs()
{
    for(uint8_t u = 0; u < MAX_LOG; ++u)
        strncpy(logs[u], "\n", MAX_LOG_STR);
    return 0;
}

SLUGmaker_Menu* SLUGmaker_LogsMenuDevLoad(void *ptr)
{
    Rectangle zone = (Rectangle) {
        .x = GetScreenWidth() * 0.8f,
		.y = GetScreenHeight() * 0.83f,
		.width = GetScreenWidth() * 0.2f,
		.height = GetScreenHeight() * 0.17f,
    };

    SLUGmaker_LogsMenu *logs_menu = (SLUGmaker_LogsMenu *) SLUGmaker_MenuDevLoad(MENU_LOGS,(Rectangle) { .x = zone.x + 20.0f,.y = zone.y + 11.0f,.width = zone.width - 40.0f,.height = zone.height - 22.0f}, sizeof(SLUGmaker_LogsMenu));

    logs_menu->group_zone = (Rectangle) {
        .x = logs_menu->m.zone.x + 11.0f,
		.y = logs_menu->m.zone.y + 11.0f,
		.width = logs_menu->m.zone.width - 22.0f,
		.height = logs_menu->m.zone.height - 22.0f,
    };

    return (SLUGmaker_Menu*) logs_menu; 
}

int8_t SLUGmaker_LogsMenuFree(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_LogsMenu *logs_menu = (SLUGmaker_LogsMenu *) menu;

    free(logs_menu);

    return 0;
}

int8_t SLUGmaker_LogsMenuResize(float factor_x, float factor_y, SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_MenuResize(factor_x,factor_y,menu);
    SLUGmaker_LogsMenu *logs_menu = (SLUGmaker_LogsMenu *) menu;

    RectangleMultiply(&(logs_menu->group_zone),factor_x,factor_y);

    return 0;
}

int8_t SLUGmaker_LogsMenuPressed(SLUGmaker_Menu *menu)
{
    if(menu == NULL)
        return -1;

    //SLUGmaker_LogsMenu *logs_menu = (SLUGmaker_LogsMenu *) menu;

    return 0;
}

int8_t SLUGmaker_LogsMenuDisplay(SLUGmaker_Menu *menu, void *ptr)
{
    if(menu == NULL)
        return -1;

    SLUGmaker_LogsMenu *logs_menu = (SLUGmaker_LogsMenu *) menu;

    GuiGroupBox(logs_menu->m.zone,"Logs");
    
    uint8_t lines = ((uint8_t) logs_menu->group_zone.height) / 20 - 1;
    char log_buffer[MAX_LOG_STR * lines];

    uint8_t counter = 0;
    int16_t u;
    for(u = MAX_LOG - 1; u >= 0; --u)
    {
        if(strncmp(logs[u], "\n", MAX_LOG) != 0)
            counter++;
        if(counter == lines)
            break;
    }
    u = fmax(u, 0);

    if(counter > 0)
    {
        size_t str_start = 0;
        for(uint8_t i = 0; i < fmin(lines, counter); ++i)
        {
            strncpy(log_buffer + str_start, logs[u + i], MAX_LOG_STR);
            str_start += strlen(logs[u + i]);
        }

        GuiTextBox(logs_menu->group_zone, log_buffer, 20, false);
    }
    
    return 0;
}

//general menu
SLUGmaker_GeneralMenu general_menu;

int8_t SLUGmaker_GeneralMenuDevLoad(void *ptr)
{
    for(uint8_t u = 0; u < MENU_NUMBER; ++u)
        general_menu.menus[u] = SLUGmaker_MenuDevLoadFunctions[u](ptr);

    for(uint8_t u = 0; u < MAX_LOG; ++u)
        strncpy(logs[u], "\n", MAX_LOG_STR);

    return 0;
}

int8_t SLUGmaker_GeneralMenuFree()
{
    for(uint8_t u = 0; u < MENU_NUMBER; ++u)
        SLUGmaker_MenuFreeFunctions[u]((SLUGmaker_Menu *) general_menu.menus[u]);

    return 0;
}

int8_t SLUGmaker_GeneralMenuResize(float factor_x, float factor_y)
{
	int8_t error;
    for(uint8_t u = 0; u < MENU_NUMBER; ++u)
    {
        error = SLUGmaker_MenuResizeFunctions[u](factor_x, factor_y, general_menu.menus[u]);
        if(error < 0)
            return error;
    }
       	
    return 0;
}

int8_t SLUGmaker_GeneralMenuDisplay(void *ptr)
{
	int8_t error;
    for(uint8_t u = 0; u < MENU_NUMBER; ++u)
    {
        error = SLUGmaker_MenuDisplayFunctions[u](general_menu.menus[u], ptr);
        if(error < 0)
            return error;
    }

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
    SLUGmaker_ToolBar *toolbar = (SLUGmaker_ToolBar *) general_menu.menus[MENU_TOOLBAR];
    
	if(graphic_vars.style != toolbar->styles.state)
	{
		switch (toolbar->styles.state)
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
		graphic_vars.style = toolbar->styles.state;
	}
	return 0;
}
