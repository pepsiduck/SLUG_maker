#include "menus.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

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
    GuiSetTooltip("Save map (CTRL+S)");
    button->pressed = GuiButton(button->zone, SLUGmaker_IconBuffer);
    return 0;
}

SLUGmaker_Toolbar SLUGmaker_ToolBarDevLoad(uint32_t screen_w, uint32_t screen_h)
{
    SLUGmaker_Toolbar toolbar;
    toolbar.zone = (Rectangle) {.x = 0, .y = 0, .width = (float) screen_w, .height = ((float) screen_h) * 0.03f};
    SLUGmaker_ButtonLoad((Rectangle){.x = toolbar.zone.width * 0.0025f, .y = ((float) screen_h) * 0.0025f, .width = ((float) screen_h) * 0.025f, .height = ((float) screen_h) * 0.025f},6,&toolbar.save);
    return toolbar;
}

int8_t SLUGmaker_ToolBarResize(float factor_x, float factor_y, SLUGmaker_Toolbar *toolbar)
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

int8_t SLUGmaker_ToolBarButtonPressed(SLUGmaker_Toolbar *toolbar)
{
    if(toolbar == NULL)
        return -1;

    SLUGmaker_ButtonPressed(&(toolbar->save));

    //GuiSetTooltip(NULL);
    return 0;
}

int8_t SLUGmaker_ToolBar(SLUGmaker_Toolbar *toolbar)
{
    if(toolbar == NULL)
        return -1;

    GuiPanel(toolbar->zone, NULL);

    int8_t err = SLUGmaker_ToolBarButtonPressed(toolbar);
    if(err < 0)
        return err;

    return 0;
}
