#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <time.h>

#include "map.h"
#include "display.h"
#include "action.h"
#include "menus.h"
//#include "raygui.h"

//#define RAYGUI_IMPLEMENTATION

//les 4 premiers bytes d'un .slmaker sont tjrs SLUGMAP en binaire

SLUGmaker_map* SLUGmaker_Init(int argc, char *argv[])
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
    else
    {   
        FILE *file = fopen(argv[1], "r");
        if(file == NULL)
        {
            printf("File doesn't exist.\n");
            return NULL;
        }

        unsigned char read[8];

        fread((void *) &read, sizeof(unsigned char), 8, file);
        fclose(file);
        
        unsigned char jpeg[] = {0xFF, 0xD8, 0xFF};
        unsigned char png[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

        if(memcmp((void *) read, (void *) jpeg, 3) == 0 || memcmp((void *) read, (void *) png, 8) == 0)
        {
           
            map = SLUGmaker_NewMap(argv[1]);
            if(map == NULL)
            {
                printf("Error while initializing new map.\n");
                return NULL;
            }
        }
    }
    
    return map;
}

int main(int argc, char *argv[])
{
    /*
    SLUGmaker_map *map = NULL;

    InitWindow(1680, 1050, "SLUGmaker");
    //ToggleFullscreen();
    InitAudioDevice();
    SetWindowState(FLAG_VSYNC_HINT|FLAG_WINDOW_RESIZABLE);
    
    if(SLUGmaker_MenusInit() == -1)
    {
    	printf("Menu initialization fail\n");
    	return 1;
    }

    map = SLUGmaker_Init(argc, argv);

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

    int8_t error = 0;
    while (!WindowShouldClose())
    {
        error = SLUGmaker_DisplayUpdate(&cam);
        if(error == -1)
            break;

        error = SLUGmaker_CameraUpdate(&cam);
        if(error == -1)
            break;
        
        error = SLUGmaker_Display(&cam);
        if(error == -1)
            break;

        if(IsKeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL))
        {
            if(SLUGmaker_WriteMap(map) == -1)
                printf("Save failure\n");      
        }

        error = SLUGmaker_ChangeActionMode(map);
        if(error == -1)
            break;

        error = SLUGmaker_Action(map,&cam);
        if(error == -1)
            break;

    }
    

    SLUGmaker_UnloadMap(map);
    SLUGmaker_MenusUnload();
    SLUGmaker_GraphicUnload();

    CloseWindow(); // Close window and OpenGL context
    CloseAudioDevice();

    return 0;
    */
    return bullshit();
}

