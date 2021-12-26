#include "zoom.h"

#include <SDL.h>
#include <SDL_image.h>

#include "../DR-vars.h"
#include "toolOption.h"
#include "../file/file.h"
#include "../DR-viewport.h"

SDL_Cursor * zoomIn=NULL;
SDL_Cursor * zoomout=NULL;
int ZoomValue=ZOOMIN;

int TOOL_SetZoom(DR_Tools * zoom)
{
    SDL_Surface * zoomInSurface=NULL;
    SDL_Surface * zoomOutSurface=NULL;

    zoomInSurface = IMG_Load("cursor\\zoomin.png");
    if(zoomInSurface==NULL)
    {
        return 0;
    }
    zoomOutSurface= IMG_Load("cursor\\zoomout.png");
    if(zoomOutSurface==NULL)
    {
        SDL_FreeSurface(zoomInSurface);
        return 0;
    }
    zoomIn = SDL_CreateColorCursor(zoomInSurface, 8, 8);
    zoomout= SDL_CreateColorCursor(zoomOutSurface,8, 8);
    if(ZoomValue==ZOOMIN)
    {
        zoom->cursor=zoomIn;
    }
    else
    {
        zoom->cursor=zoomout;
    }

    //SDL_SetCursor(zoom->cursor);

    SDL_FreeSurface(zoomInSurface);
    SDL_FreeSurface(zoomOutSurface);

    //init tool option
//puts("z");
    TOOLOPTION_CreateOptionSet("zoom",TOOL_SET_RADIO,TOOL_SetValueZoom);
//puts("a");
    TOOLOPTION_CreateRadioOption("zoom","Zoom In","icons\\zoomin.png",ZOOMIN,ZoomValue==ZOOMIN);
//puts("b");
    TOOLOPTION_CreateRadioOption("zoom","Zoom Out","icons\\zoomout.png",ZOOMOUT,ZoomValue==ZOOMOUT);
//puts("c");
    TOOLOPTION_CreateOptionBar();
//puts("d");
    return 1;

}

void TOOL_UseZoom(int x,int y)
{
    DR_MainViewport * currentPort=DR_FindViewport("drawp");
    int dx,dy;
    int oldWidth,oldHieght;

    if(ZoomValue==ZOOMIN)
    {
        if(mainFile->zoom<1000)
        {
            if(mainFile->zoom<100)
            {
                if(mainFile->zoom<50)
                {
                    if(mainFile->zoom<25)
                    {
                        mainFile->zoom=25;
                    }
                    else
                    {
                        mainFile->zoom=50;
                    }
                }
                else
                {
                    mainFile->zoom=100;
                }
            }
            else
            {
                mainFile->zoom= ((mainFile->zoom/100)+1)*100;
            }
        }
        else
        {
                return;
        }

    }
    else if(ZoomValue==ZOOMOUT)
    {
        if(mainFile->zoom>100)
        {
            if(mainFile->zoom%100==0)
            {
                mainFile->zoom-=100;
            }
            else
            {
                mainFile->zoom-=(mainFile->zoom%100);
            }
        }
        else
        {
            if(mainFile->zoom>50)
            {
                mainFile->zoom=50;
            }
            else if(mainFile->zoom>25)
            {
                mainFile->zoom=25;
            }
            else if(mainFile->zoom>10)
            {
                mainFile->zoom=10;
            }
            else
            {
                return;
            }
        }
    }

    //calculate rect
    //coordinate for drawing panel
    x -= currentPort->rect.x;
    y -= currentPort->rect.y;
    //the new distance
    dx=x-mainFile->rect.x;
    dy=y-mainFile->rect.y;
   //save old dimension
    oldWidth=mainFile->rect.w;
    oldHieght=mainFile->rect.h;

    mainFile->rect.h=mainFile->h*mainFile->zoom/100;
    mainFile->rect.w=mainFile->w*mainFile->zoom/100;

    dx=(mainFile->rect.w*dx)/oldWidth;
    dy=(mainFile->rect.h*dy)/oldHieght;
    //change distance to coordinate
    mainFile->rect.x=x-dx;
    mainFile->rect.y=y-dy;

    //update view
    TOOL_UpdateTexture(&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
    DF_CheckFileRect();
    DF_UpdateFile();
}


void TOOL_UnSetZoom(DR_Tools * zoom)
{
    SDL_FreeCursor(zoomIn);
    SDL_FreeCursor(zoomout);

    TOOLOPTION_DestroyOptionBar();

    zoom->cursor=NULL;

    zoomIn=NULL;
    zoomout=NULL;
}

void TOOL_SetValueZoom(int value)
{
    ZoomValue=value;
    if(ZoomValue==ZOOMIN)
        DR_GetTool()->cursor=zoomIn;
    else
        DR_GetTool()->cursor=zoomout;
}
