#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <time.h>

#include "map.h"
#include "display.h"
#include "action.h"
#include "menus.h"
#include "raygui.h"
#include "defines.h"
#include "tinyfiledialogs.h"

//#define RAYGUI_IMPLEMENTATION

//les 4 premiers bytes d'un .slmaker sont tjrs SLUGMAP en binaire

SLUGmaker_map* SLUGmaker_MapInit(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Wrong arguments.\n");
        return NULL;
    }

    SLUGmaker_map *map;

    if(strchr(argv[1], '.') == NULL)
    {
        map = SLUGmaker_LoadMap(argv[1]); //TODO Load map
        if(map == NULL)
        {
            printf("Error while loading map.\n");
            return NULL;
        }
    }
    
    
    return map;
}

int8_t SLUGmaker_ChangeFullscreen(int16_t const screenWidth, int16_t const screenHeight)
{
	if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
 	{
 		int display = GetCurrentMonitor();

        if (IsWindowFullscreen())
            SetWindowSize(screenWidth, screenHeight);
        else
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
 
		ToggleFullscreen();
        return 1;
 	}
    return 0;
}

int8_t SLUGmaker_Resize(SLUGmaker_ToolBar *toolbar, SLUGmaker_ActionButtonsMenu *actionMenu, SLUGmaker_camera *cam)
{
	if(GetScreenWidth() != graphic_vars.screen_w || GetScreenHeight() != graphic_vars.screen_h)
	{
		float w = (float) GetScreenWidth();
		float h = (float) GetScreenHeight();

		float factor_x = w / graphic_vars.screen_w;
		float factor_y = h / graphic_vars.screen_h;
			
        int8_t error = SLUGmaker_ToolBarResize(factor_x, factor_y, toolbar);
        if(error == -1)
           	return -1;
        error = SLUGmaker_ActionButtonsMenuResize(factor_x, factor_y, actionMenu);
        if(error == -1)
           	return -1;
        error = SLUGmaker_DisplayUpdate(factor_x, factor_y, cam);
        if(error == -1)
           	return -1;
        	
        graphic_vars.screen_w = w;
		graphic_vars.screen_h = h;	
        	
        return 1;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    SetWindowState(FLAG_VSYNC_HINT|FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "SLUGmaker");
    
    int display = GetCurrentMonitor();
    int16_t const screenWidth = GetMonitorWidth(display);
    int16_t const screenHeight = GetMonitorHeight(display);
        
    InitAudioDevice();

    SLUGmaker_map *map = SLUGmaker_NewMap(3360, 2100);

    if(map == NULL)
    {
        printf("Map initialization fail.\n");
        return 1;
    }

    if(SLUGmaker_GraphicInit() != 0)
    {
        printf("Graphic initalization fail.\n");
        SLUGmaker_UnloadMap(map);
        return 1;
    }

    HideCursor();

    SLUGmaker_camera cam = SLUGmaker_DefaultCamera(map);
    SLUGmaker_ToolBar toolbar = SLUGmaker_ToolBarDevLoad();
    SLUGmaker_ActionButtonsMenu actionMenu = SLUGmaker_ActionButtonsMenuDevLoad();
    GuiEnableTooltip();

    int8_t error = 0;
    bool resize = false;
    
    BeginDrawing();
	ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
	EndDrawing();
    
    while (!WindowShouldClose())
    {
		if(!SLUGmaker_ChangeFullscreen(screenWidth, screenHeight))
			resize = SLUGmaker_Resize(&toolbar, &actionMenu, &cam);
		
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
		    
		error = SLUGmaker_ToolBarDisplay(&toolbar);   
		if(error == -1)
		    break;
		        
		error = SLUGmaker_ActionButtonsMenuDisplay(&actionMenu);   
		if(error == -1)
		    break;

		error = SLUGmaker_CameraUpdate(&cam,resize);
		if(error == -1)
		    break;
		    
		error = SLUGmaker_Display(&cam);
		if(error == -1)
		    break;
		        
		if(menu_vars.map_selection_menu)
    	{
    		int32_t new_map_result = GuiMessageBox((Rectangle){ (float)GetScreenWidth()/2 - 125, (float)GetScreenHeight()/2 - 50, 250, 100 }, "#159#New map", "Do tou want to open or create a new map ?", "Open;New map");
    		if(new_map_result == 1)
    		{
                char map_file_name[256];
    			char *tempFolderName = tinyfd_selectFolderDialog("Open map directory" ,NULL);
                if(tempFolderName != NULL)
                {
                    strncpy(map_file_name,tempFolderName,255);
                    SLUGmaker_map *map2 = SLUGmaker_LoadMap(map_file_name);
                    if(map2 == NULL)
                        printf("Could not open map.\n");
                    else
                    {
                        SLUGmaker_UnloadMap(map);
                        map = map2;
                        cam = SLUGmaker_DefaultCamera(map);
                    }
                }     
    		}
    		menu_vars.map_selection_menu = false;
    	}
		       
		EndDrawing();
		    
		    
		if(!menu_vars.map_selection_menu)
		{
			error = SLUGmaker_ChangeGUIStyle(&toolbar);
			if(error == -1)
				break;

			if((IsKeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL)) || toolbar.save.pressed)
			{
				if(SLUGmaker_WriteMap(map) == -1)
				    printf("Save failure\n");      
			}

			error = SLUGmaker_ChangeActionMode(&actionMenu, map);
			if(error == -1)
				break;

			error = SLUGmaker_Action(map,&cam);
			if(error == -1)
				break;
		}
		

    }

    SLUGmaker_UnloadMap(map);
    SLUGmaker_GraphicUnload();

    CloseWindow(); // Close window and OpenGL context
    CloseAudioDevice();

    return 0;

}

