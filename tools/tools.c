#include "tools.h"

#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <stdlib.h>

#include "hand.h"
#include "line.h"
#include "rectSelect.h"
#include "zoom.h"
#include "brush.h"

#include "../DR-viewport.h"
#include "../DR-vars.h"
#include "../file/file.h"


DR_Tools * ToolsHead=NULL;
DR_Tools * ToolUsing=NULL;
SDL_Cursor * arrowCursor=NULL;

int DR_InitTools()
{
    DR_Tools * hand=NULL;
    DR_Tools * line=NULL;
    DR_Tools * rectSelect=NULL;
    DR_Tools * zoom=NULL;
    DR_Tools * brush=NULL;

    hand=DR_InitTool();
    if(hand==NULL)
    {
        return 0;
    }
    strcpy(hand->name,"hand");

    hand->icon=IMG_Load("icons\\hand.png");
    if(hand->icon==NULL)
    {
        //free(hand);
        return 0;
    }
    arrowCursor=SDL_GetCursor();

    hand->SetTool=TOOL_SetHand;
    hand->UnSetTool=TOOL_UnSetLine;
    hand->UseTool=TOOL_MoveHand;

    hand->SetTool(hand);
    ToolUsing=hand;//defualt tool for now


    //line tool
    line=DR_InitTool();
    if(line==NULL)
    {
        return 0;
    }
    strcpy(line->name,"line");

    line->icon=IMG_Load("icons\\line.png");
    if(line->icon==NULL)
    {
        return 0;
    }
    line->SetTool=TOOL_SetLine;
    line->UnSetTool=TOOL_UnSetLine;
    line->UseTool=TOOL_UseLine;

    //rectangle selection tool
    rectSelect=DR_InitTool();
    if(rectSelect==NULL)
    {
        return 0;
    }
    strcpy(rectSelect->name,"rectangular selection");

    rectSelect->icon=IMG_Load("icons\\rectSelect.png");
    if(rectSelect->icon==NULL)
    {
        return 0;
    }
    rectSelect->SetTool=TOOL_SetRectSelect;
    rectSelect->UnSetTool=TOOL_UnsSetRectSelect;
    rectSelect->UseTool=TOOL_RectSelect;

    //zoom tool
    zoom=DR_InitTool();
    if(zoom==NULL)
    {
        return 0;
    }
    strcpy(zoom->name,"zoom");

    zoom->icon=IMG_Load("icons\\zoom.png");
    if(zoom->icon==NULL)
    {
        return 0;
    }
    zoom->SetTool=TOOL_SetZoom;
    zoom->UnSetTool=TOOL_UnSetZoom;
    zoom->UseTool=TOOL_UseZoom;

    //brush tool
    brush=DR_InitTool();
    if(brush==NULL)
    {
        return 0;
    }
    strcpy(brush->name,"brush");
    brush->icon=IMG_Load("icons\\brush.png");
    if(brush->icon==NULL)
    {
        return 0;
    }
    brush->SetTool=TOOL_SetBrush;
    brush->UnSetTool=TOOL_UnSetBrush;
    brush->UseTool=TOOL_UseBrush;

    return 1;
}

DR_Tools * DR_InitTool()
{
    DR_Tools * currentTool=NULL;

    DR_Tools *tool=(DR_Tools *) malloc(sizeof(DR_Tools));
    if(tool==NULL)
    {
        return NULL;
    }
    if(ToolsHead==NULL)
    {
        ToolsHead=tool;
    }
    else
    {
        currentTool=ToolsHead;
        while(currentTool->next)
        {
            currentTool=currentTool->next;
        }
        currentTool->next=tool;
    }
    tool->next=NULL;
    return tool;
}

void DR_SetTool(DR_Tools * tool)
{
    ToolUsing=tool;
}

DR_Tools * DR_GetTool()
{
    return ToolUsing;
}

DR_Tools * DR_GetToolHead()
{
    return ToolsHead;
}

void DR_SetToolCursor()
{
    SDL_SetCursor(ToolUsing->cursor);
}
void DR_SetArrowCursor()
{
    SDL_SetCursor(arrowCursor);
}
void DR_ExecuteEvent(int x, int y)
{
puts("before using tool");
    ToolUsing->UseTool(x,y);
puts("after using tool");
}

SDL_Texture * TOOL_CreateLineTexture(SDL_Surface * surface,SDL_Rect * srect,SDL_Rect * drect)
{
    DR_MainViewport * currentPort=DR_FindViewport("drawp");
    SDL_Texture * texture=SDL_CreateTextureFromSurface(renderer,surface);

    float scale= mainFile->zoom/100.0;


    if(texture==NULL)
    {
        return NULL;
    }

    if(currentPort==NULL)
    {
        SDL_DestroyTexture(texture);
        return NULL;
    }
    //src rect
    if(mainFile->rect.x<0)
    {
        srect->x= - mainFile->rect.x;
        srect->w=   mainFile->rect.w - srect->x;
        drect->x=   currentPort->rect.x;
        drect->w=   mainFile->rect.w - srect->x;
    }
    else
    {
        srect->x = 0;
        srect->w = mainFile->rect.w;
        drect->x = currentPort->rect.x + mainFile->rect.x;//calculate
        drect->w = mainFile->rect.w;
    }

    if(mainFile->rect.y<0)
    {
        srect->y = - mainFile->rect.y;
        srect->h =   mainFile->rect.h - srect->y;
        drect->y =   currentPort->rect.y;
        drect->h =   mainFile->rect.h - srect->y;
    }
    else
    {
        srect->y = 0;
        srect->h = mainFile->rect.h;
        drect->y = currentPort->rect.y + mainFile->rect.y;//calculate
        drect->h = mainFile->rect.h;
    }
    if(mainFile->rect.x + mainFile->rect.w > currentPort->rect.w-15)
    {
        srect->w = currentPort->rect.w-15 - mainFile->rect.x;
        drect->w = srect->w;
    }
    if(mainFile->rect.y + mainFile->rect.h > currentPort->rect.h)
    {
        srect->h = currentPort->rect.h-15 - mainFile->rect.y;
        drect->h = srect->h;
    }
    srect->x/=scale;
    srect->y/=scale;
    srect->w/=scale;
    srect->h/=scale;

    return texture;
}
void TOOL_UpdateTexture(SDL_Rect * srect,SDL_Rect * drect)
{
    DR_MainViewport * currentPort=DR_FindViewport("drawp");
    float scale= mainFile->zoom/100.0;
    SDL_Rect port;

    if(currentPort==NULL)
    {
        return;
    }

    port.w=currentPort->rect.w - 15;
    port.h=currentPort->rect.h - 15;

    if(mainFile->rect.x<0)
    {
        srect->x= - mainFile->rect.x;
        srect->w=   mainFile->rect.w - srect->x;
        drect->x=   currentPort->rect.x;
        drect->w=   mainFile->rect.w - srect->x;
    }
    else
    {
        srect->x = 0;
        srect->w = mainFile->rect.w;
        drect->x = currentPort->rect.x + mainFile->rect.x;//calculate
        drect->w = mainFile->rect.w;
    }

    if(mainFile->rect.y<0)
    {
        srect->y = - mainFile->rect.y;
        srect->h =   mainFile->rect.h - srect->y;
        drect->y =   currentPort->rect.y;
        drect->h =   mainFile->rect.h - srect->y;
    }
    else
    {
        srect->y = 0;
        srect->h = mainFile->rect.h;
        drect->y = currentPort->rect.y + mainFile->rect.y;//calculate
        drect->h = mainFile->rect.h;
    }
    if(mainFile->rect.x + mainFile->rect.w > port.w)
    {
        srect->w = port.w - mainFile->rect.x;
        drect->w = srect->w;
    }
    if(mainFile->rect.y + mainFile->rect.h > port.h)
    {
        srect->h = port.h - mainFile->rect.y;
        drect->h = srect->h;
    }
    srect->x/=scale;
    srect->y/=scale;
    srect->w/=scale;
    srect->h/=scale;
}

