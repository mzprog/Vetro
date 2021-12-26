#include "toolbar.h"

#include <SDL.h>
#include <string.h>

#include "tools.h"
#include "../DR-viewport.h"
#include "../core/sys.h"

int DR_CreateToolBar()//update
{
    SDL_Rect rect;

    DR_MainViewport * currentPort = DR_FindViewport("toolbar");
    DR_Tools * currentTool=DR_GetToolHead();
    DR_Tools * ToolUsing=DR_GetTool();

    if(currentTool==NULL)
    {
        return 0;
    }

    if(!currentPort)
    {
        return 0;
    }

    rect.x=0;
    rect.y=0;
    rect.h=25;
    rect.w=25;



    while(currentTool)
    {
        if(currentTool==ToolUsing)
        {
            SDL_FillRect(currentPort->surface,&rect,SYS_RGBA(0x20,0x20,0x20,0xff));
        }
        if(currentTool->icon==NULL)
        {
            puts("no icon");
        }
        SDL_BlitSurface(currentTool->icon,NULL,currentPort->surface,&rect);
        currentTool->rect.x=rect.x;
        currentTool->rect.y=rect.y;
        currentTool->rect.w=rect.w;
        currentTool->rect.h=rect.h;

        rect.x+=25;
        currentTool=currentTool->next;
    }

    return 1;
}

int DR_UpdateToolBar()
{
    DR_MainViewport * currentPort = DR_FindViewport("toolbar");

    if(!currentPort)
    {
        return 0;
    }
    SDL_FillRect(currentPort->surface,NULL,currentPort->viewport->bgcolor);
    DR_CreateToolBar();

    return 1;
}

int DR_ToolBarClicked(int x,int y)
{
    DR_MainViewport * currentPort = DR_FindViewport("toolbar");
    DR_Tools * currentTool=DR_GetToolHead();
    DR_Tools * ToolUsing=DR_GetTool();


    if(!currentPort)
    {
        return 0;
    }

    x-=currentPort->rect.x;
    y-=currentPort->rect.y;

    while(currentTool && !DR_IconClicked(currentTool,x,y))
    {
        currentTool=currentTool->next;
    }
    if(!currentTool)
    {
        return 1;
    }

    if(currentTool == ToolUsing)
    {
        return 0;
    }

    ToolUsing->UnSetTool(ToolUsing);
    currentTool->SetTool(currentTool);
    DR_SetTool(currentTool);
    DR_UpdateToolBar();

    return 1;
}

int DR_IconClicked(DR_Tools * tool, int x, int y)
{
    if(tool->rect.x<x && tool->rect.x+tool->rect.w>x)
   {
        if(tool->rect.y<y && tool->rect.y+tool->rect.h>y)
        {
            return 1;
        }
   }
    return 0;
}
