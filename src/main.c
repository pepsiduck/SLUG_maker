#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <string.h>

#include "map.h"
#include "display.h"
#include "action.h"

//les 4 premiers bytes d'un .slmaker sont tjrs SLUGMAP en binaire

SLUGmaker_map* SLUGmaker_Init(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Wrong arguments.\n");
        return NULL;
    }
   
    FILE *file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("File doesn't exist.\n");
        return NULL;
    }

    unsigned char *read = (unsigned char *) malloc(8*sizeof(unsigned char));
    if(read == NULL)
    {
        printf("Error in SLUGmaker_Init\n");
        return NULL;
    }



    fread(read, sizeof(read), 1, file);
    fclose(file);
    
    unsigned char jpeg[] = {0xFF, 0xD8, 0xFF};
    unsigned char png[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    unsigned char slug[] = {0x53, 0x4C, 0x55, 0x47, 0x4D, 0x41, 0x50};

    SLUGmaker_map *map; 

    if(memcmp((void *) read, (void *) jpeg, 3) == 0 || memcmp((void *) read, (void *) png, 8) == 0)
    {
       
        map = SLUGmaker_NewMap(argv[1]);
        if(map == NULL)
        {
            printf("Error while initializing new map.\n");
            return NULL;
        }
    }
    else if(memcmp((void *) read, (void *) slug, 7) == 0)
    {
        map = SLUGmaker_LoadMap(argv[1]); //TODO Load map
        if(map == NULL)
        {
            printf("Error while loading map.\n");
            return NULL;
        }
    }
    free(read);
    return map;
}

int main(int argc, char *argv[])
{
    SLUGmaker_map *map = NULL;

    InitWindow(1680, 1050, "SLUGmaker");
    //ToggleFullscreen();
    InitAudioDevice();
    SetWindowState(FLAG_VSYNC_HINT|FLAG_WINDOW_RESIZABLE);

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
            error = SLUGmaker_WriteMap(map);
            if(error == -1)
                break;
        }

        error = SLUGmaker_ChangeAction();
        if(error == -1)
            break;

        error = SLUGmaker_Action(map,&cam);
        if(error == -1)
            break;
    }
    

    SLUGmaker_UnloadMap(map);

    SLUGmaker_GraphicUnload();

    CloseWindow(); // Close window and OpenGL context
    CloseAudioDevice();

    return 0;
}

