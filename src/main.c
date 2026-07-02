#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <time.h>

//#define RAYGUI_IMPLEMENTATION

#include "map.h"
#include "display.h"
#include "action.h"
#include "menus.h"
#include "raygui.h"
#include "defines.h"
#include "tinyfiledialogs.h"

SLUGmaker_map* SLUGmaker_OpenNewMapFileDialog(SLUGmaker_map *map, SLUGmaker_camera *cam)
{
	char map_file_name[256];
	char *tempFolderName = tinyfd_selectFolderDialog("Open map directory" ,NULL);
	if(tempFolderName != NULL)
	{
		SLUGmaker_WriteLog("Open map.\n");
		strncpy(map_file_name,tempFolderName,255);
		SLUGmaker_map *map2 = SLUGmaker_LoadMap(map_file_name);
		if(map2 == NULL)
		{
			SLUGmaker_WriteLog("Could not open map.\n");
			return map;
		}
		else
		{
			SLUGmaker_UnloadMap(map);
			*cam = SLUGmaker_DefaultCamera(map2);
			return map2;
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

int8_t SLUGmaker_Resize(SLUGmaker_camera *cam)
{
	if(GetScreenWidth() != graphic_vars.screen_w || GetScreenHeight() != graphic_vars.screen_h)
	{
		float w = (float) GetScreenWidth();
		float h = (float) GetScreenHeight();

		float factor_x = w / graphic_vars.screen_w;
		float factor_y = h / graphic_vars.screen_h;
			
        int8_t error = SLUGmaker_GeneralMenuResize(factor_x, factor_y);
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
    InitWindow(0, 0, "SLUGmaker");
    SetWindowState(FLAG_VSYNC_HINT|FLAG_WINDOW_RESIZABLE);
    
    int display = GetCurrentMonitor();
    int16_t const screenWidth = GetMonitorWidth(display);
    int16_t const screenHeight = GetMonitorHeight(display);
    
    if(SLUGmaker_GraphicInit() != 0)
    {
        SLUGmaker_WriteLog("Graphic initalization fail.\n");
        return 1;
    }
    
    if(SLUGmaker_MenuInit() != 0)
    {
        SLUGmaker_WriteLog("Menu initalization fail.\n");
        return 1;
    }
        
    InitAudioDevice();

    SLUGmaker_map *map = SLUGmaker_NewMap(3360, 2100);
    if(map == NULL)
    {
        SLUGmaker_WriteLog("Map initialization fail.\n");
        return 1;
    }
    
	SLUGmaker_camera cam = SLUGmaker_DefaultCamera(map);
	
	if(SLUGmaker_GeneralMenuDevLoad((void *) map) == -1)
	{
		SLUGmaker_WriteLog("Error while loading general menu");
		return -1;
	}

    SLUGmaker_ToolBar *toolbar = (SLUGmaker_ToolBar *) general_menu.menus[MENU_TOOLBAR];
	
    HideCursor();
    GuiEnableTooltip();

    int8_t error = 0;
    bool resize = false;
    
    BeginDrawing();
	ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
	EndDrawing();
    
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);

    while (!WindowShouldClose())
    {

		if(!SLUGmaker_ChangeFullscreen(screenWidth, screenHeight))
			resize = SLUGmaker_Resize(&cam);
		
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		error = SLUGmaker_CameraUpdate(&cam,resize);
		if(error == -1)
		    break;
		    
		error = SLUGmaker_Display(&cam);
		if(error == -1)
		    break;        
		       
		EndDrawing();
		    
		if(menu_vars.map_selection_result == 1)
        {
        	SLUGmaker_map *buff = SLUGmaker_OpenNewMapFileDialog(map, &cam);
			if(buff == map)
                SLUGmaker_WriteLog("Load Error.\n");
            else
            	map = buff;  
                
            menu_vars.map_selection_result = -1;
        }
		    
		if(!menu_vars.map_selection_menu) //or all the stuff
		{
			error = SLUGmaker_ChangeGUIStyle();
			if(error == -1)
				break;

			if((IsKeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL)) || toolbar->save.pressed)
			{
				if(SLUGmaker_SaveMap(map) == -1)
				    SLUGmaker_WriteLog("Save failure\n");      
			}

            if((IsKeyPressed(KEY_E) && IsKeyDown(KEY_LEFT_CONTROL)) || toolbar->exprt.pressed)
			{
				if(SLUGmaker_ExportMap(map) == -1)
				    SLUGmaker_WriteLog("Save failure\n");      
			}

			error = SLUGmaker_ChangeActionMode(map);
			if(error == -1)
				break;

			error = SLUGmaker_Action(map,&cam);
			if(error == -1)
				break;
		}
		

    }

    SLUGmaker_UnloadMap(map);
    SLUGmaker_GraphicUnload();
    SLUGmaker_GeneralMenuFree();

    CloseWindow(); // Close window and OpenGL context
    CloseAudioDevice();

    return 0;

}

