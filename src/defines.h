#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <raylib.h>

typedef struct SLUGmaker_GraphicVariables SLUGmaker_GraphicVariables;
struct SLUGmaker_GraphicVariables
{
    Rectangle display;
    float screen_w;
    float screen_h;
    
    Texture2D mouse_cursor_sprite; 
    Texture2D wall_node_sprite;
    Texture2D player_spawn_point_sprite;
    Texture2D sprite_node_sprite;

    Texture2D missing_texture;
    
    int32_t style;
    
    bool selection_menu;
};

extern SLUGmaker_GraphicVariables graphic_vars;

int8_t SLUGmaker_GraphicInit();
int8_t SLUGmaker_GraphicUnload();

#endif
